/*
 * clist_compat.h - GtkCList compatibility layer for GTK-4
 *
 * This header provides a compatibility API that wraps GTK-4's GtkColumnView
 * to provide similar functionality to the deprecated GtkCList widget.
 *
 * This allows existing code using gtk_clist_* APIs to work with minimal changes.
 */

#ifndef CLIST_COMPAT_H
#define CLIST_COMPAT_H

#include <gtk/gtk.h>

/* Compatibility macros */
#define GtkCList GtkColumnView
#define GTK_CLIST(obj) GTK_COLUMN_VIEW(obj)
#define GTK_IS_CLIST(obj) GTK_IS_COLUMN_VIEW(obj)

/* CList-like structure to hold data for each row */
typedef struct {
    gchar **columns;  /* Array of column strings */
    gint n_columns;
    gpointer row_data;  /* Custom row data */
} CListRow;

/* Compatibility functions */

/* Create a new CList-compatible ColumnView */
GtkWidget* xfr_clist_new(gint columns);

/* Append a row with text */
gint xfr_clist_append(GtkColumnView *clist, gchar *text[]);

/* Insert a row at position */
gint xfr_clist_insert(GtkColumnView *clist, gint row, gchar *text[]);

/* Remove a row */
void xfr_clist_remove(GtkColumnView *clist, gint row);

/* Clear all rows */
void xfr_clist_clear(GtkColumnView *clist);

/* Get text from a cell */
gboolean xfr_clist_get_text(GtkColumnView *clist, gint row, gint column, gchar **text);

/* Set text in a cell */
void xfr_clist_set_text(GtkColumnView *clist, gint row, gint column, const gchar *text);

/* Select a row */
void xfr_clist_select_row(GtkColumnView *clist, gint row, gint column);

/* Swap two rows */
void xfr_clist_swap_rows(GtkColumnView *clist, gint row1, gint row2);

/* Get/set row data */
gpointer xfr_clist_get_row_data(GtkColumnView *clist, gint row);
void xfr_clist_set_row_data(GtkColumnView *clist, gint row, gpointer data);

/* Get number of rows */
gint xfr_clist_get_rows(GtkColumnView *clist);

/* Get selected row (returns -1 if none) */
gint xfr_clist_get_selected_row(GtkColumnView *clist);

/* Get selected rows as GList (for compatibility with ->selection member) */
GList* xfr_clist_get_selection(GtkColumnView *clist);

/* Get number of columns */
gint xfr_clist_get_columns(GtkColumnView *clist);

/* Set column width */
void xfr_clist_set_column_width(GtkColumnView *clist, gint column, gint width);

/* Show column titles */
void xfr_clist_column_titles_show(GtkColumnView *clist);

/* Set column widget (header widget) */
void xfr_clist_set_column_widget(GtkColumnView *clist, gint column, GtkWidget *widget);

/* Set up a selection callback (replaces select_row signal) */
void xfr_clist_set_selection_callback(GtkColumnView *clist, GCallback callback, gpointer user_data);

/* Macro replacements for common operations */
#define gtk_clist_new(cols) xfr_clist_new(cols)
#define gtk_clist_append(clist, text) xfr_clist_append(GTK_COLUMN_VIEW(clist), text)
#define gtk_clist_insert(clist, row, text) xfr_clist_insert(GTK_COLUMN_VIEW(clist), row, text)
#define gtk_clist_remove(clist, row) xfr_clist_remove(GTK_COLUMN_VIEW(clist), row)
#define gtk_clist_clear(clist) xfr_clist_clear(GTK_COLUMN_VIEW(clist))
#define gtk_clist_get_text(clist, row, col, text) xfr_clist_get_text(GTK_COLUMN_VIEW(clist), row, col, text)
#define gtk_clist_set_text(clist, row, col, text) xfr_clist_set_text(GTK_COLUMN_VIEW(clist), row, col, text)
#define gtk_clist_select_row(clist, row, col) xfr_clist_select_row(GTK_COLUMN_VIEW(clist), row, col)
#define gtk_clist_swap_rows(clist, row1, row2) xfr_clist_swap_rows(GTK_COLUMN_VIEW(clist), row1, row2)
#define gtk_clist_get_row_data(clist, row) xfr_clist_get_row_data(GTK_COLUMN_VIEW(clist), row)
#define gtk_clist_set_row_data(clist, row, data) xfr_clist_set_row_data(GTK_COLUMN_VIEW(clist), row, data)
#define gtk_clist_set_column_width(clist, col, width) xfr_clist_set_column_width(GTK_COLUMN_VIEW(clist), col, width)
#define gtk_clist_column_titles_show(clist) xfr_clist_column_titles_show(GTK_COLUMN_VIEW(clist))
#define gtk_clist_set_column_widget(clist, col, widget) xfr_clist_set_column_widget(GTK_COLUMN_VIEW(clist), col, widget)

#endif /* CLIST_COMPAT_H */
