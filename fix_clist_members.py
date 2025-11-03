#!/usr/bin/env python3
"""
Fix direct CList member accesses in callbacks.c
Replaces ->rows, ->selection, ->columns with function calls
"""

import re

def fix_member_accesses(content):
    """Replace CList member accesses with function calls"""

    # Pattern 1: clist->rows → xfr_clist_get_rows(clist)
    # Handle various patterns like: GTK_CLIST(foo)->rows, foo->rows, (foo)->rows
    content = re.sub(
        r'GTK_CLIST\((\w+)\)->rows',
        r'xfr_clist_get_rows(GTK_COLUMN_VIEW(\1))',
        content
    )
    content = re.sub(
        r'\((\w+)\)->rows',
        r'xfr_clist_get_rows(\1)',
        content
    )
    content = re.sub(
        r'(\w+)->rows(?!_)',  # Avoid matching things like "rows_data"
        r'xfr_clist_get_rows(\1)',
        content
    )

    # Pattern 2: clist->selection → xfr_clist_get_selection(clist)
    content = re.sub(
        r'GTK_CLIST\((\w+)\)->selection',
        r'xfr_clist_get_selection(GTK_COLUMN_VIEW(\1))',
        content
    )
    content = re.sub(
        r'(\w+)->selection(?!_)',
        r'xfr_clist_get_selection(\1)',
        content
    )

    # Pattern 3: clist->columns → xfr_clist_get_columns(clist)
    content = re.sub(
        r'GTK_CLIST\((\w+)\)->columns',
        r'xfr_clist_get_columns(GTK_COLUMN_VIEW(\1))',
        content
    )
    content = re.sub(
        r'(\w+)->columns(?!_)',
        r'xfr_clist_get_columns(\1)',
        content
    )

    return content

def main():
    input_file = '/Users/jinlei/code/xfresco/src/callbacks.c'
    output_file = '/Users/jinlei/code/xfresco/src/callbacks.c.new'

    with open(input_file, 'r') as f:
        content = f.read()

    print(f"Processing {input_file}...")
    print(f"Original size: {len(content)} bytes")

    content = fix_member_accesses(content)

    print(f"Fixed size: {len(content)} bytes")

    with open(output_file, 'w') as f:
        f.write(content)

    print(f"Output written to {output_file}")
    print("Review and replace callbacks.c if satisfied")

if __name__ == '__main__':
    main()
