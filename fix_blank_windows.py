#!/usr/bin/env python3
"""
Fix blank windows - replace incorrect GTK_IS_BOX checks with gtk_window_set_child
In GTK-4, windows are not boxes. You cannot use gtk_box_append to add content.
You must use gtk_window_set_child() instead.
"""

import re

def fix_window_child_setting(content):
    """Replace GTK_IS_BOX(window_*) checks with gtk_window_set_child()"""

    # Pattern to find:
    # if (GTK_IS_BOX(window_name) && GTK_IS_WIDGET(vbox_name)) {
    #   gtk_box_append (GTK_BOX (window_name), vbox_name);
    # }

    # This pattern captures window creation and incorrect box append
    pattern = r'if \(GTK_IS_BOX\((window_\w+)\) && GTK_IS_WIDGET\((\w+)\)\) \{\s*gtk_box_append \(GTK_BOX \(\1\), \2\);\s*\}'

    def replace_with_set_child(match):
        window_name = match.group(1)
        vbox_name = match.group(2)

        # GTK-4 proper way: use gtk_window_set_child
        return f'gtk_window_set_child (GTK_WINDOW ({window_name}), {vbox_name});  /* GTK-4: windows use set_child, not box_append */'

    new_content = re.sub(pattern, replace_with_set_child, content, flags=re.MULTILINE)

    # Count how many replacements were made
    matches = re.findall(pattern, content, flags=re.MULTILINE)
    print(f"Fixed {len(matches)} window child settings")
    if matches:
        for window, vbox in matches:
            print(f"  - {window}: {vbox}")

    return new_content

def main():
    input_file = 'src/interface.c'

    print(f"Reading {input_file}...")
    with open(input_file, 'r') as f:
        content = f.read()

    print("Fixing window child settings...")
    content = fix_window_child_setting(content)

    print(f"Writing fixed content to {input_file}...")
    with open(input_file, 'w') as f:
        f.write(content)

    print("\n✓ Done! All blank windows should now display correctly.")
    print("Rebuild with: make clean && make")

if __name__ == '__main__':
    main()
