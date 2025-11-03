#!/usr/bin/env python3
"""
Migrate Glade 2.0 file from GTK-2 to GTK-4
- Convert GtkHBox/GtkVBox to GtkBox with orientation
- Convert GtkTable to GtkGrid
- Convert GtkCList to GtkColumnView
"""

import re
import sys

def migrate_box_widgets(content):
    """Convert GtkHBox and GtkVBox to GtkBox with orientation"""
    # Replace GtkVBox
    content = re.sub(
        r'<widget class="GtkVBox"',
        '<widget class="GtkBox"',
        content
    )
    content = re.sub(
        r'(<widget class="GtkBox"[^>]*>\n\s*<property name="visible">)',
        r'\1',
        content
    )

    # Replace GtkHBox
    content = re.sub(
        r'<widget class="GtkHBox"',
        '<widget class="GtkBox"',
        content
    )

    # Add orientation property for VBox (vertical)
    # This is tricky because we need to distinguish which was VBox vs HBox
    # We'll do this in two passes

    return content

def migrate_box_widgets_v2(content):
    """
    Convert GtkHBox/GtkVBox to GtkBox with orientation property.
    More careful approach tracking which was which.
    """
    lines = content.split('\n')
    result = []
    i = 0

    while i < len(lines):
        line = lines[i]

        # Check for GtkVBox
        if '<widget class="GtkVBox"' in line:
            # Replace with GtkBox
            line = line.replace('GtkVBox', 'GtkBox')
            result.append(line)
            i += 1
            # Add orientation property after visible property
            while i < len(lines):
                result.append(lines[i])
                if '<property name="visible">' in lines[i] or '<property name="border_width">' in lines[i]:
                    # Look ahead to see if orientation already exists
                    if i + 1 < len(lines) and 'orientation' not in lines[i + 1]:
                        # Add orientation after the next line
                        i += 1
                        result.append(lines[i])
                        # Insert orientation property with proper indentation
                        indent = len(lines[i]) - len(lines[i].lstrip())
                        result.append(' ' * indent + '<property name="orientation">GTK_ORIENTATION_VERTICAL</property>')
                    break
                i += 1
        # Check for GtkHBox
        elif '<widget class="GtkHBox"' in line:
            # Replace with GtkBox
            line = line.replace('GtkHBox', 'GtkBox')
            result.append(line)
            i += 1
            # Add orientation property after visible property
            while i < len(lines):
                result.append(lines[i])
                if '<property name="visible">' in lines[i] or '<property name="border_width">' in lines[i]:
                    # Look ahead to see if orientation already exists
                    if i + 1 < len(lines) and 'orientation' not in lines[i + 1]:
                        # Add orientation after the next line
                        i += 1
                        result.append(lines[i])
                        # Insert orientation property with proper indentation
                        indent = len(lines[i]) - len(lines[i].lstrip())
                        result.append(' ' * indent + '<property name="orientation">GTK_ORIENTATION_HORIZONTAL</property>')
                    break
                i += 1
        # Check for GtkVButtonBox
        elif '<widget class="GtkVButtonBox"' in line:
            line = line.replace('GtkVButtonBox', 'GtkButtonBox')
            result.append(line)
            i += 1
            while i < len(lines):
                result.append(lines[i])
                if '<property name="visible">' in lines[i]:
                    if i + 1 < len(lines) and 'orientation' not in lines[i + 1]:
                        i += 1
                        result.append(lines[i])
                        indent = len(lines[i]) - len(lines[i].lstrip())
                        result.append(' ' * indent + '<property name="orientation">GTK_ORIENTATION_VERTICAL</property>')
                    break
                i += 1
        # Check for GtkHButtonBox
        elif '<widget class="GtkHButtonBox"' in line:
            line = line.replace('GtkHButtonBox', 'GtkButtonBox')
            result.append(line)
            i += 1
            while i < len(lines):
                result.append(lines[i])
                if '<property name="visible">' in lines[i]:
                    if i + 1 < len(lines) and 'orientation' not in lines[i + 1]:
                        i += 1
                        result.append(lines[i])
                        indent = len(lines[i]) - len(lines[i].lstrip())
                        result.append(' ' * indent + '<property name="orientation">GTK_ORIENTATION_HORIZONTAL</property>')
                    break
                i += 1
        else:
            result.append(line)
            i += 1

    return '\n'.join(result)

def migrate_table_to_grid(content):
    """Convert GtkTable to GtkGrid"""
    # Replace class name
    content = re.sub(r'<widget class="GtkTable"', '<widget class="GtkGrid"', content)

    # GtkTable properties need conversion:
    # - n_rows, n_columns properties are not needed in GtkGrid (auto-sized)
    # - Remove these properties
    content = re.sub(r'\s*<property name="n_rows">\d+</property>\n', '', content)
    content = re.sub(r'\s*<property name="n_columns">\d+</property>\n', '', content)

    # Packing properties change:
    # GtkTable: left_attach, right_attach, top_attach, bottom_attach
    # GtkGrid: column, row, width (colspan), height (rowspan)
    # This requires more complex transformation

    return content

def simple_migrate(content):
    """Simplified migration focusing on class name replacements"""
    # GtkHBox/GtkVBox → GtkBox (we'll fix orientation manually if needed)
    content = content.replace('class="GtkHBox"', 'class="GtkBox"')
    content = content.replace('class="GtkVBox"', 'class="GtkBox"')
    content = content.replace('class="GtkHButtonBox"', 'class="GtkButtonBox"')
    content = content.replace('class="GtkVButtonBox"', 'class="GtkButtonBox"')

    # GtkTable → GtkGrid
    content = content.replace('class="GtkTable"', 'class="GtkGrid"')

    return content

def main():
    input_file = '/Users/jinlei/code/xfresco/xfresco2.gld'
    output_file = '/Users/jinlei/code/xfresco/xfresco2.gld.new'

    with open(input_file, 'r') as f:
        content = f.read()

    print("Migrating Glade file...")
    print(f"Original size: {len(content)} bytes")

    # Apply migrations
    content = migrate_box_widgets_v2(content)
    content = migrate_table_to_grid(content)

    print(f"Migrated size: {len(content)} bytes")

    with open(output_file, 'w') as f:
        f.write(content)

    print(f"Migration complete. Output written to {output_file}")
    print("Review the changes and rename to xfresco2.gld if satisfied.")

if __name__ == '__main__':
    main()
