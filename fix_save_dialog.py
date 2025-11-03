#!/usr/bin/env python3
"""
Fix save dialog - convert from placeholder gtk_window_new to GtkFileChooserDialog
"""

import re

def fix_save_filesel_function(content):
    """Replace create_save_filesel placeholder with GtkFileChooserDialog"""

    # Find the entire create_save_filesel function
    pattern = r'(GtkWidget\*\ncreate_save_filesel \(void\)\n\{.*?^  return save_filesel;\n\})'

    replacement = '''GtkWidget*
create_save_filesel (void)
{
  GtkWidget *save_filesel;
  GtkFileFilter *filter_all, *filter_fresco;

  /* Use GtkFileChooserDialog for GTK-4 */
  save_filesel = gtk_file_chooser_dialog_new (
    _("Save project"),
    GTK_WINDOW (main_window),
    GTK_FILE_CHOOSER_ACTION_SAVE,
    _("_Cancel"), GTK_RESPONSE_CANCEL,
    _("_Save"), GTK_RESPONSE_ACCEPT,
    NULL);

  /* Set modal to prevent interaction with main window */
  gtk_window_set_modal (GTK_WINDOW (save_filesel), TRUE);

  /* Enable overwrite confirmation */
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (save_filesel), TRUE);

  /* Configure file chooser for better usability */
  gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER (save_filesel), FALSE);
  gtk_file_chooser_set_create_folders (GTK_FILE_CHOOSER (save_filesel), TRUE);

  /* Set a reasonable default size for the dialog */
  gtk_window_set_default_size (GTK_WINDOW (save_filesel), 800, 600);

  /* Add "All Files" first so it's the default */
  filter_all = gtk_file_filter_new ();
  gtk_file_filter_set_name (filter_all, "All Files");
  gtk_file_filter_add_pattern (filter_all, "*");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (save_filesel), filter_all);

  /* Add FRESCO-specific filter with both uppercase and lowercase patterns */
  filter_fresco = gtk_file_filter_new ();
  gtk_file_filter_set_name (filter_fresco, "FRESCO Files (*.in, *.dat, *.fresco)");
  gtk_file_filter_add_pattern (filter_fresco, "*.in");
  gtk_file_filter_add_pattern (filter_fresco, "*.IN");
  gtk_file_filter_add_pattern (filter_fresco, "*.dat");
  gtk_file_filter_add_pattern (filter_fresco, "*.DAT");
  gtk_file_filter_add_pattern (filter_fresco, "*.fresco");
  gtk_file_filter_add_pattern (filter_fresco, "*.FRESCO");
  gtk_file_filter_add_pattern (filter_fresco, "*.Fresco");
  gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (save_filesel), filter_fresco);

  /* Connect response signal */
  g_signal_connect (save_filesel, "response",
                    G_CALLBACK (on_save_filesel_response),
                    NULL);

  GLADE_HOOKUP_OBJECT_NO_REF (save_filesel, save_filesel, "save_filesel");

  return save_filesel;
}'''

    new_content = re.sub(pattern, replacement, content, flags=re.MULTILINE | re.DOTALL)

    if new_content == content:
        print("Warning: Pattern not found, trying alternative approach...")
        return content

    print("Fixed create_save_filesel function")
    return new_content

def fix_save_callbacks(content):
    """Fix the save dialog callbacks to use GtkFileChooserDialog API"""

    # Replace on_save_filesel_ok_button_clicked with on_save_filesel_response
    pattern1 = r'void\non_save_filesel_ok_button_clicked\s+\(GtkButton\s+\*button,\s*gpointer\s+user_data\)\n\{.*?\n\}'

    replacement1 = '''void
on_save_filesel_response               (GtkDialog       *dialog,
                                        gint             response_id,
                                        gpointer         user_data)
{
  GtkWidget *main_window;
  GFile *file;
  gchar *filename;

  if (response_id == GTK_RESPONSE_ACCEPT) {
    main_window = g_object_get_data (G_OBJECT (dialog), "main_window");
    file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));

    if (file) {
      filename = g_file_get_path (file);
      g_print("\\nSaving file %s...\\n", filename);

      current_filename = g_strdup(filename);
      real_save_file (main_window, filename);

      g_free (filename);
      g_object_unref (file);
    }
  }

  /* Hide dialog instead of destroying it for better performance on re-open */
  gtk_widget_set_visible (GTK_WIDGET (dialog), FALSE);
}'''

    new_content = re.sub(pattern1, replacement1, content, flags=re.MULTILINE | re.DOTALL)

    # Remove the old cancel button callback (no longer needed)
    pattern2 = r'void\non_save_filesel_cancel_button_clicked\s+\(GtkButton\s+\*button,\s*gpointer\s+user_data\)\n\{.*?\n\}\n\n'
    new_content = re.sub(pattern2, '', new_content, flags=re.MULTILINE | re.DOTALL)

    print("Fixed save dialog callbacks")
    return new_content

def add_callback_declaration(content):
    """Add declaration for on_save_filesel_response in callbacks.h"""

    # Find where on_save_filesel_ok_button_clicked is declared
    pattern = r'void\s+on_save_filesel_ok_button_clicked\s+\(GtkButton\s+\*button,\s*gpointer\s+user_data\);'

    replacement = '''void
on_save_filesel_response               (GtkDialog       *dialog,
                                        gint             response_id,
                                        gpointer         user_data);'''

    new_content = re.sub(pattern, replacement, content)

    # Remove cancel button declaration
    pattern2 = r'void\s+on_save_filesel_cancel_button_clicked\s+\(GtkButton\s+\*button,\s*gpointer\s+user_data\);\s*'
    new_content = re.sub(pattern2, '', new_content)

    print("Updated callback declarations in callbacks.h")
    return new_content

def fix_save_activate_callback(content):
    """Fix on_Save_as_activate to use gtk_window_present instead of gtk_widget_show"""

    # Find on_Save_as_activate function
    pattern = r'(void\s+on_Save_as_activate.*?\{[^}]*?)(gtk_widget_show\s*\(\s*save_filesel\s*\)\s*;)'

    replacement = r'\1gtk_window_present (GTK_WINDOW(save_filesel));  /* GTK-4: use gtk_window_present instead of gtk_widget_show */'

    new_content = re.sub(pattern, replacement, content, flags=re.MULTILINE | re.DOTALL)

    print("Fixed on_Save_as_activate to use gtk_window_present")
    return new_content

def main():
    # Fix interface.c
    print("Reading src/interface.c...")
    with open('src/interface.c', 'r') as f:
        interface_content = f.read()

    print("Fixing create_save_filesel function...")
    interface_content = fix_save_filesel_function(interface_content)

    print("Writing updated interface.c...")
    with open('src/interface.c', 'w') as f:
        f.write(interface_content)

    # Fix callbacks.c
    print("\nReading src/callbacks.c...")
    with open('src/callbacks.c', 'r') as f:
        callbacks_content = f.read()

    print("Fixing save dialog callbacks...")
    callbacks_content = fix_save_callbacks(callbacks_content)

    print("Fixing on_Save_as_activate callback...")
    callbacks_content = fix_save_activate_callback(callbacks_content)

    print("Writing updated callbacks.c...")
    with open('src/callbacks.c', 'w') as f:
        f.write(callbacks_content)

    # Fix callbacks.h
    print("\nReading src/callbacks.h...")
    with open('src/callbacks.h', 'r') as f:
        callbacks_h_content = f.read()

    print("Adding callback declarations...")
    callbacks_h_content = add_callback_declaration(callbacks_h_content)

    print("Writing updated callbacks.h...")
    with open('src/callbacks.h', 'w') as f:
        f.write(callbacks_h_content)

    print("\n✓ Done! Save dialog has been converted to GtkFileChooserDialog.")
    print("Rebuild with: make clean && make")

if __name__ == '__main__':
    main()
