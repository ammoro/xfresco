#!/usr/bin/env python3
"""
Add simple GTK-4 compatible menubar using GtkButton + GtkPopover.
This approach doesn't require GtkApplication - works with plain gtk_init().
"""

import re

def add_menubar_code(content):
    """Add simple GTK-4 menubar to interface.c"""

    # Find the location where menubar is set to NULL
    pattern = r'(/\* menubar = gtk_menu_bar_new\.\.\.; \*/) menubar = NULL;'

    gtk4_menubar_code = '''/* GTK-4 Menu using GtkMenuButton + GtkPopover (no GtkApplication needed) */
  menubar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_widget_set_visible(menubar, TRUE);

  /* File menu */
  GtkWidget *file_btn = gtk_menu_button_new();
  gtk_menu_button_set_label(GTK_MENU_BUTTON(file_btn), "File");
  gtk_widget_set_visible(file_btn, TRUE);
  gtk_box_append(GTK_BOX(menubar), file_btn);

  GtkWidget *file_popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_widget_set_margin_top(file_popover_box, 6);
  gtk_widget_set_margin_bottom(file_popover_box, 6);
  gtk_widget_set_margin_start(file_popover_box, 6);
  gtk_widget_set_margin_end(file_popover_box, 6);

  GtkWidget *file_new_btn = gtk_button_new_with_label("New");
  gtk_widget_set_visible(file_new_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_new_btn);

  GtkWidget *file_open_btn = gtk_button_new_with_label("Open");
  gtk_widget_set_visible(file_open_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_open_btn);

  GtkWidget *file_import_btn = gtk_button_new_with_label("Import");
  gtk_widget_set_visible(file_import_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_import_btn);

  GtkWidget *file_print_btn = gtk_button_new_with_label("Print");
  gtk_widget_set_visible(file_print_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_print_btn);

  GtkWidget *file_revert_btn = gtk_button_new_with_label("Revert");
  gtk_widget_set_visible(file_revert_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_revert_btn);

  GtkWidget *file_save_btn = gtk_button_new_with_label("Save");
  gtk_widget_set_visible(file_save_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_save_btn);

  GtkWidget *file_saveas_btn = gtk_button_new_with_label("Save As");
  gtk_widget_set_visible(file_saveas_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_saveas_btn);

  GtkWidget *file_sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_set_visible(file_sep, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_sep);

  GtkWidget *file_exit_btn = gtk_button_new_with_label("Exit");
  gtk_widget_set_visible(file_exit_btn, TRUE);
  gtk_box_append(GTK_BOX(file_popover_box), file_exit_btn);

  GtkWidget *file_popover = gtk_popover_new();
  gtk_popover_set_child(GTK_POPOVER(file_popover), file_popover_box);
  gtk_menu_button_set_popover(GTK_MENU_BUTTON(file_btn), file_popover);

  /* Edit menu */
  GtkWidget *edit_btn = gtk_menu_button_new();
  gtk_menu_button_set_label(GTK_MENU_BUTTON(edit_btn), "Edit");
  gtk_widget_set_visible(edit_btn, TRUE);
  gtk_box_append(GTK_BOX(menubar), edit_btn);

  GtkWidget *edit_popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_widget_set_margin_top(edit_popover_box, 6);
  gtk_widget_set_margin_bottom(edit_popover_box, 6);
  gtk_widget_set_margin_start(edit_popover_box, 6);
  gtk_widget_set_margin_end(edit_popover_box, 6);

  GtkWidget *edit_showinput_btn = gtk_button_new_with_label("Show Input");
  gtk_widget_set_visible(edit_showinput_btn, TRUE);
  gtk_box_append(GTK_BOX(edit_popover_box), edit_showinput_btn);

  GtkWidget *edit_popover = gtk_popover_new();
  gtk_popover_set_child(GTK_POPOVER(edit_popover), edit_popover_box);
  gtk_menu_button_set_popover(GTK_MENU_BUTTON(edit_btn), edit_popover);

  /* Run menu */
  GtkWidget *run_btn = gtk_menu_button_new();
  gtk_menu_button_set_label(GTK_MENU_BUTTON(run_btn), "Run");
  gtk_widget_set_visible(run_btn, TRUE);
  gtk_box_append(GTK_BOX(menubar), run_btn);

  GtkWidget *run_popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_widget_set_margin_top(run_popover_box, 6);
  gtk_widget_set_margin_bottom(run_popover_box, 6);
  gtk_widget_set_margin_start(run_popover_box, 6);
  gtk_widget_set_margin_end(run_popover_box, 6);

  GtkWidget *run_options_btn = gtk_button_new_with_label("Run Options");
  gtk_widget_set_visible(run_options_btn, TRUE);
  gtk_box_append(GTK_BOX(run_popover_box), run_options_btn);

  GtkWidget *run_run_btn = gtk_button_new_with_label("Run");
  gtk_widget_set_visible(run_run_btn, TRUE);
  gtk_box_append(GTK_BOX(run_popover_box), run_run_btn);

  GtkWidget *run_kill_btn = gtk_button_new_with_label("Kill Current");
  gtk_widget_set_visible(run_kill_btn, TRUE);
  gtk_box_append(GTK_BOX(run_popover_box), run_kill_btn);

  GtkWidget *run_popover = gtk_popover_new();
  gtk_popover_set_child(GTK_POPOVER(run_popover), run_popover_box);
  gtk_menu_button_set_popover(GTK_MENU_BUTTON(run_btn), run_popover);

  /* Options menu */
  GtkWidget *options_btn = gtk_menu_button_new();
  gtk_menu_button_set_label(GTK_MENU_BUTTON(options_btn), "Options");
  gtk_widget_set_visible(options_btn, TRUE);
  gtk_box_append(GTK_BOX(menubar), options_btn);

  GtkWidget *options_popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_widget_set_margin_top(options_popover_box, 6);
  gtk_widget_set_margin_bottom(options_popover_box, 6);
  gtk_widget_set_margin_start(options_popover_box, 6);
  gtk_widget_set_margin_end(options_popover_box, 6);

  GtkWidget *options_stdout_btn = gtk_button_new_with_label("Check stdout");
  gtk_widget_set_visible(options_stdout_btn, TRUE);
  gtk_box_append(GTK_BOX(options_popover_box), options_stdout_btn);

  GtkWidget *options_files_btn = gtk_button_new_with_label("Files");
  gtk_widget_set_visible(options_files_btn, TRUE);
  gtk_box_append(GTK_BOX(options_popover_box), options_files_btn);

  GtkWidget *options_popover = gtk_popover_new();
  gtk_popover_set_child(GTK_POPOVER(options_popover), options_popover_box);
  gtk_menu_button_set_popover(GTK_MENU_BUTTON(options_btn), options_popover);

  /* About menu */
  GtkWidget *about_btn = gtk_menu_button_new();
  gtk_menu_button_set_label(GTK_MENU_BUTTON(about_btn), "About");
  gtk_widget_set_visible(about_btn, TRUE);
  gtk_box_append(GTK_BOX(menubar), about_btn);

  GtkWidget *about_popover_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_widget_set_margin_top(about_popover_box, 6);
  gtk_widget_set_margin_bottom(about_popover_box, 6);
  gtk_widget_set_margin_start(about_popover_box, 6);
  gtk_widget_set_margin_end(about_popover_box, 6);

  GtkWidget *about_about_btn = gtk_button_new_with_label("About");
  gtk_widget_set_visible(about_about_btn, TRUE);
  gtk_box_append(GTK_BOX(about_popover_box), about_about_btn);

  GtkWidget *about_version_btn = gtk_button_new_with_label("Version");
  gtk_widget_set_visible(about_version_btn, TRUE);
  gtk_box_append(GTK_BOX(about_popover_box), about_version_btn);

  GtkWidget *about_popover = gtk_popover_new();
  gtk_popover_set_child(GTK_POPOVER(about_popover), about_popover_box);
  gtk_menu_button_set_popover(GTK_MENU_BUTTON(about_btn), about_popover);'''

    new_content = re.sub(pattern, gtk4_menubar_code, content)

    # Fix the gtk_box_append line to actually add menubar to main_vbox
    pattern2 = r'/\* gtk_box_append \(GTK_BOX \(main_vbox\), menubar\); - NULL widget \*/'
    replacement2 = 'gtk_box_append (GTK_BOX (main_vbox), menubar);'
    new_content = re.sub(pattern2, replacement2, new_content)

    # Remove the old gtk_widget_show line
    pattern3 = r'/\* gtk_widget_show \(menubar\); - NULL widget \*/'
    replacement3 = '/* menubar visibility set inline */'
    new_content = re.sub(pattern3, replacement3, new_content)

    return new_content

def add_signal_connections(content):
    """Add signal connections for menu buttons at the end of create_main_window"""

    # Find the GLADE_HOOKUP_OBJECT for menubar
    pattern = r'(  GLADE_HOOKUP_OBJECT \(main_window, menubar, "menubar"\);)'

    signal_code = r'''\1

  /* Connect menu button signals to existing callbacks */
  g_signal_connect((gpointer) file_new_btn, "clicked", G_CALLBACK(on_New_activate), NULL);
  g_signal_connect((gpointer) file_open_btn, "clicked", G_CALLBACK(on_Open_activate), NULL);
  g_signal_connect((gpointer) file_import_btn, "clicked", G_CALLBACK(on_import_activate), NULL);
  g_signal_connect((gpointer) file_print_btn, "clicked", G_CALLBACK(on_print_activate), NULL);
  g_signal_connect((gpointer) file_revert_btn, "clicked", G_CALLBACK(on_revert_activate), NULL);
  g_signal_connect((gpointer) file_save_btn, "clicked", G_CALLBACK(on_Save_activate), NULL);
  g_signal_connect((gpointer) file_saveas_btn, "clicked", G_CALLBACK(on_Save_as_activate), NULL);
  g_signal_connect((gpointer) file_exit_btn, "clicked", G_CALLBACK(on_Exit_activate), NULL);
  g_signal_connect((gpointer) edit_showinput_btn, "clicked", G_CALLBACK(on_Show_input_activate), NULL);
  g_signal_connect((gpointer) run_options_btn, "clicked", G_CALLBACK(on_Run_options_activate), NULL);
  g_signal_connect((gpointer) run_run_btn, "clicked", G_CALLBACK(on_Run_activate), NULL);
  g_signal_connect((gpointer) run_kill_btn, "clicked", G_CALLBACK(on_kill_current_activate), NULL);
  g_signal_connect((gpointer) options_stdout_btn, "clicked", G_CALLBACK(on_check_stdout_activate), NULL);
  g_signal_connect((gpointer) options_files_btn, "clicked", G_CALLBACK(on_files_activate), NULL);
  g_signal_connect((gpointer) about_about_btn, "clicked", G_CALLBACK(on_About_activate), NULL);
  g_signal_connect((gpointer) about_version_btn, "clicked", G_CALLBACK(on_Version_activate), NULL);'''

    new_content = re.sub(pattern, signal_code, content)
    return new_content

def main():
    input_file = 'src/interface.c'

    print(f"Reading {input_file}...")
    with open(input_file, 'r') as f:
        content = f.read()

    print("Adding GTK-4 menubar code...")
    content = add_menubar_code(content)

    print("Adding signal connections...")
    content = add_signal_connections(content)

    print(f"Writing updated content to {input_file}...")
    with open(input_file, 'w') as f:
        f.write(content)

    print("\nDone! Simple GTK-4 menubar has been added.")
    print("The menubar uses GtkMenuButton + GtkPopover and doesn't require GtkApplication.")

if __name__ == '__main__':
    main()
