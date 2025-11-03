#!/usr/bin/env python3
"""
Convert gtk_table_attach calls from GTK-2 to gtk_grid_attach for GTK-4.

This script reads the backup file to extract original gtk_table_attach calls
and converts them to gtk_grid_attach format, then applies them to interface.c.
"""

import re
import sys

def parse_table_attach(lines, start_idx):
    """
    Parse a gtk_table_attach call that may span multiple lines.
    Returns (widget_name, parent_table, left, right, top, bottom, xoptions, yoptions, end_idx)
    """
    # Find the complete gtk_table_attach call
    call_text = ""
    paren_count = 0
    i = start_idx
    started = False

    while i < len(lines):
        line = lines[i]
        for char in line:
            if char == '(':
                paren_count += 1
                started = True
            elif char == ')':
                paren_count -= 1
            call_text += char
            if started and paren_count == 0:
                break
        if started and paren_count == 0:
            break
        call_text += " "
        i += 1

    # Extract parameters using regex
    # gtk_table_attach (GTK_TABLE (table_name), widget, left, right, top, bottom, xopts, yopts, xpad, ypad)
    pattern = r'gtk_table_attach\s*\(\s*GTK_TABLE\s*\(\s*(\w+)\s*\)\s*,\s*(\w+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,'
    match = re.search(pattern, call_text)

    if not match:
        return None

    parent_table = match.group(1)
    widget_name = match.group(2)
    left = int(match.group(3))
    right = int(match.group(4))
    top = int(match.group(5))
    bottom = int(match.group(6))

    # Extract options - they're cast to GtkAttachOptions
    xoptions = "0"
    yoptions = "0"

    # Look for GTK_EXPAND, GTK_SHRINK, GTK_FILL flags
    if "GTK_EXPAND" in call_text.split(',')[6] if len(call_text.split(',')) > 6 else "":
        xoptions = "GTK_EXPAND"
    if len(call_text.split(',')) > 7 and "GTK_EXPAND" in call_text.split(',')[7]:
        yoptions = "GTK_EXPAND"

    return {
        'widget': widget_name,
        'parent': parent_table,
        'left': left,
        'right': right,
        'top': top,
        'bottom': bottom,
        'xoptions': xoptions,
        'yoptions': yoptions,
        'end_idx': i
    }

def generate_grid_attach(attach_info):
    """Generate gtk_grid_attach code from parsed attach info."""
    widget = attach_info['widget']
    parent = attach_info['parent']
    left = attach_info['left']
    top = attach_info['top']
    width = attach_info['right'] - attach_info['left']
    height = attach_info['bottom'] - attach_info['top']

    code = f"  gtk_grid_attach (GTK_GRID ({parent}), {widget}, {left}, {top}, {width}, {height});\n"

    # Add expand properties if needed
    if "GTK_EXPAND" in attach_info['xoptions']:
        code += f"  gtk_widget_set_hexpand ({widget}, TRUE);\n"
    if "GTK_EXPAND" in attach_info['yoptions']:
        code += f"  gtk_widget_set_vexpand ({widget}, TRUE);\n"

    return code

def main():
    # Read the backup file to extract original gtk_table_attach calls
    print("Reading backup file...")
    with open('/Users/jinlei/code/xfresco/src/interface.c.bak', 'r') as f:
        backup_lines = f.readlines()

    # Extract all gtk_table_attach calls from backup
    print("Extracting gtk_table_attach calls from backup...")
    attach_calls = {}
    i = 0
    while i < len(backup_lines):
        if 'gtk_table_attach' in backup_lines[i] and 'GTK_TABLE' in backup_lines[i]:
            info = parse_table_attach(backup_lines, i)
            if info:
                widget = info['widget']
                attach_calls[widget] = info
                i = info['end_idx'] + 1
            else:
                i += 1
        else:
            i += 1

    print(f"Found {len(attach_calls)} gtk_table_attach calls")

    # Read current interface.c
    print("Reading current interface.c...")
    with open('/Users/jinlei/code/xfresco/src/interface.c', 'r') as f:
        current_lines = f.readlines()

    # Find and replace TODO comments with actual grid_attach calls
    print("Converting gtk_table_attach to gtk_grid_attach...")
    new_lines = []
    conversions = 0

    i = 0
    while i < len(current_lines):
        line = current_lines[i]

        # Check if this is a TODO comment for table_attach
        if '/* gtk_table_attach removed - TODO: convert to gtk_grid_attach */' in line:
            # Look backwards to find the widget name
            widget_name = None
            for j in range(i-1, max(0, i-10), -1):
                # Look for patterns like "widget = gtk_entry_new();" or "gtk_widget_show (widget);"
                widget_match = re.search(r'\b(\w+)\s*=\s*gtk_\w+', current_lines[j])
                if not widget_match:
                    widget_match = re.search(r'gtk_widget_show\s*\(\s*(\w+)\s*\)', current_lines[j])
                if not widget_match:
                    widget_match = re.search(r'gtk_\w+\s*\([^,]*,\s*(\w+)\s*[,\)]', current_lines[j])

                if widget_match:
                    widget_name = widget_match.group(1)
                    if widget_name in attach_calls:
                        break

            if widget_name and widget_name in attach_calls:
                # Replace the TODO comment with actual grid_attach code
                new_lines.append(generate_grid_attach(attach_calls[widget_name]))
                conversions += 1
            else:
                # Keep the TODO comment if we can't find the widget
                new_lines.append(line)
        else:
            new_lines.append(line)

        i += 1

    # Write the modified interface.c
    print(f"Writing updated interface.c with {conversions} conversions...")
    with open('/Users/jinlei/code/xfresco/src/interface.c', 'w') as f:
        f.writelines(new_lines)

    print(f"Done! Converted {conversions} gtk_table_attach calls to gtk_grid_attach")
    return 0

if __name__ == '__main__':
    sys.exit(main())
