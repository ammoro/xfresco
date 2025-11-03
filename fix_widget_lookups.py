#!/usr/bin/env python3
"""
Fix widget lookup warnings - change over_* widget lookups from clist to main_window
All over_* widgets are registered with main_window, not with clist
"""

import re

def fix_over_widget_lookups(content):
    """Replace lookup_widget(GTK_WIDGET(clist),"over_*") with lookup_widget(main_window,"over_*")"""

    # Pattern: lookup_widget(GTK_WIDGET(clist),"over_
    pattern = r'lookup_widget\(GTK_WIDGET\(clist\),"(over_\w+)"\)'

    def replace_lookup(match):
        widget_name = match.group(1)
        return f'lookup_widget(main_window,"{widget_name}")'

    new_content = re.sub(pattern, replace_lookup, content)

    # Count how many replacements were made
    matches = re.findall(pattern, content)
    print(f"Fixed {len(matches)} widget lookups")
    if matches:
        # Count unique widgets
        unique_widgets = set(matches)
        print(f"Unique widgets fixed: {len(unique_widgets)}")
        for widget in sorted(unique_widgets):
            count = matches.count(widget)
            print(f"  - {widget}: {count} occurrence(s)")

    return new_content

def main():
    input_file = 'src/callbacks.c'

    print(f"Reading {input_file}...")
    with open(input_file, 'r') as f:
        content = f.read()

    print("Fixing over_* widget lookups...")
    content = fix_over_widget_lookups(content)

    print(f"Writing fixed content to {input_file}...")
    with open(input_file, 'w') as f:
        f.write(content)

    print("\n✓ Done! Widget lookup warnings should now be fixed.")
    print("Rebuild with: make clean && make")

if __name__ == '__main__':
    main()
