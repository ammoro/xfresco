/*
 * clist_compat.c - GtkCList compatibility layer implementation
 *
 * Provides GtkCList-like API on top of GTK-4's GtkColumnView
 */

#include "clist_compat.h"
#include <string.h>

/* Key for storing GListStore in ColumnView */
#define CLIST_STORE_KEY "clist-store"
#define CLIST_SELECTION_KEY "clist-selection"
#define CLIST_NCOLS_KEY "clist-ncols"

/* Row object type */
#define XFR_TYPE_CLIST_ROW (xfr_clist_row_get_type())
G_DECLARE_FINAL_TYPE(XfrCListRow, xfr_clist_row, XFR, CLIST_ROW, GObject)

struct _XfrCListRow {
    GObject parent_instance;
    gchar **columns;
    gint n_columns;
    gpointer row_data;
};

G_DEFINE_TYPE(XfrCListRow, xfr_clist_row, G_TYPE_OBJECT)

static void
xfr_clist_row_finalize(GObject *object)
{
    XfrCListRow *row = XFR_CLIST_ROW(object);

    if (row->columns) {
        for (gint i = 0; i < row->n_columns; i++) {
            g_free(row->columns[i]);
        }
        g_free(row->columns);
    }

    G_OBJECT_CLASS(xfr_clist_row_parent_class)->finalize(object);
}

static void
xfr_clist_row_class_init(XfrCListRowClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = xfr_clist_row_finalize;
}

static void
xfr_clist_row_init(XfrCListRow *row)
{
    row->columns = NULL;
    row->n_columns = 0;
    row->row_data = NULL;
}

static XfrCListRow*
xfr_clist_row_new(gint n_columns, gchar *text[])
{
    XfrCListRow *row = g_object_new(XFR_TYPE_CLIST_ROW, NULL);
    row->n_columns = n_columns;
    row->columns = g_new0(gchar*, n_columns);

    for (gint i = 0; i < n_columns; i++) {
        row->columns[i] = g_strdup(text && text[i] ? text[i] : "");
    }

    return row;
}

/* Setup function for cell factories */
static void
setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    gtk_list_item_set_child(list_item, label);
}

/* Bind function for cell factories */
static void
bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data)
{
    gint column = GPOINTER_TO_INT(user_data);
    GtkWidget *label = gtk_list_item_get_child(list_item);
    XfrCListRow *row = gtk_list_item_get_item(list_item);

    if (row && row->columns && column < row->n_columns) {
        gtk_label_set_text(GTK_LABEL(label), row->columns[column]);
    }
}

/* Create a new CList-compatible ColumnView */
GtkWidget*
xfr_clist_new(gint columns)
{
    GtkWidget *column_view = gtk_column_view_new(NULL);
    GListStore *store = g_list_store_new(XFR_TYPE_CLIST_ROW);
    GtkSingleSelection *selection = gtk_single_selection_new(G_LIST_MODEL(store));

    gtk_column_view_set_model(GTK_COLUMN_VIEW(column_view),
                               GTK_SELECTION_MODEL(selection));

    /* Store references */
    g_object_set_data_full(G_OBJECT(column_view), CLIST_STORE_KEY,
                           g_object_ref(store), g_object_unref);
    g_object_set_data_full(G_OBJECT(column_view), CLIST_SELECTION_KEY,
                           g_object_ref(selection), g_object_unref);
    g_object_set_data(G_OBJECT(column_view), CLIST_NCOLS_KEY,
                      GINT_TO_POINTER(columns));

    /* Create columns */
    for (gint i = 0; i < columns; i++) {
        GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
        g_signal_connect(factory, "setup", G_CALLBACK(setup_listitem_cb), NULL);
        g_signal_connect(factory, "bind", G_CALLBACK(bind_listitem_cb),
                        GINT_TO_POINTER(i));

        GtkColumnViewColumn *col = gtk_column_view_column_new(NULL, factory);
        gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view), col);
    }

    g_object_unref(store);
    g_object_unref(selection);

    return column_view;
}

/* Append a row */
gint
xfr_clist_append(GtkColumnView *clist, gchar *text[])
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    gint n_columns = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(clist), CLIST_NCOLS_KEY));

    XfrCListRow *row = xfr_clist_row_new(n_columns, text);
    g_list_store_append(store, row);
    g_object_unref(row);

    return g_list_model_get_n_items(G_LIST_MODEL(store)) - 1;
}

/* Insert a row */
gint
xfr_clist_insert(GtkColumnView *clist, gint position, gchar *text[])
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    gint n_columns = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(clist), CLIST_NCOLS_KEY));

    XfrCListRow *row = xfr_clist_row_new(n_columns, text);
    g_list_store_insert(store, position, row);
    g_object_unref(row);

    return position;
}

/* Remove a row */
void
xfr_clist_remove(GtkColumnView *clist, gint row)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    g_list_store_remove(store, row);
}

/* Clear all rows */
void
xfr_clist_clear(GtkColumnView *clist)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    g_list_store_remove_all(store);
}

/* Get text from a cell */
gboolean
xfr_clist_get_text(GtkColumnView *clist, gint row, gint column, gchar **text)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    XfrCListRow *row_obj = g_list_model_get_item(G_LIST_MODEL(store), row);

    if (!row_obj)
        return FALSE;

    if (column >= row_obj->n_columns) {
        g_object_unref(row_obj);
        return FALSE;
    }

    *text = row_obj->columns[column];
    g_object_unref(row_obj);
    return TRUE;
}

/* Set text in a cell */
void
xfr_clist_set_text(GtkColumnView *clist, gint row, gint column, const gchar *text)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    XfrCListRow *row_obj = g_list_model_get_item(G_LIST_MODEL(store), row);

    if (!row_obj)
        return;

    if (column < row_obj->n_columns) {
        g_free(row_obj->columns[column]);
        row_obj->columns[column] = g_strdup(text ? text : "");

        /* Notify of change */
        g_list_store_remove(store, row);
        g_list_store_insert(store, row, row_obj);
    }

    g_object_unref(row_obj);
}

/* Select a row */
void
xfr_clist_select_row(GtkColumnView *clist, gint row, gint column)
{
    GtkSingleSelection *selection = g_object_get_data(G_OBJECT(clist),
                                                       CLIST_SELECTION_KEY);
    gtk_single_selection_set_selected(selection, row);
}

/* Swap two rows */
void
xfr_clist_swap_rows(GtkColumnView *clist, gint row1, gint row2)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);

    XfrCListRow *obj1 = g_list_model_get_item(G_LIST_MODEL(store), row1);
    XfrCListRow *obj2 = g_list_model_get_item(G_LIST_MODEL(store), row2);

    if (!obj1 || !obj2) {
        if (obj1) g_object_unref(obj1);
        if (obj2) g_object_unref(obj2);
        return;
    }

    /* Remove both (higher index first) */
    gint first = row1 < row2 ? row1 : row2;
    gint second = row1 < row2 ? row2 : row1;
    XfrCListRow *first_obj = (row1 < row2) ? obj1 : obj2;
    XfrCListRow *second_obj = (row1 < row2) ? obj2 : obj1;

    g_list_store_remove(store, second);
    g_list_store_remove(store, first);

    /* Re-insert in swapped order */
    g_list_store_insert(store, first, second_obj);
    g_list_store_insert(store, second, first_obj);

    g_object_unref(obj1);
    g_object_unref(obj2);
}

/* Get row data */
gpointer
xfr_clist_get_row_data(GtkColumnView *clist, gint row)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    XfrCListRow *row_obj = g_list_model_get_item(G_LIST_MODEL(store), row);

    if (!row_obj)
        return NULL;

    gpointer data = row_obj->row_data;
    g_object_unref(row_obj);
    return data;
}

/* Set row data */
void
xfr_clist_set_row_data(GtkColumnView *clist, gint row, gpointer data)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    XfrCListRow *row_obj = g_list_model_get_item(G_LIST_MODEL(store), row);

    if (!row_obj)
        return;

    row_obj->row_data = data;
    g_object_unref(row_obj);
}

/* Get number of rows */
gint
xfr_clist_get_rows(GtkColumnView *clist)
{
    GListStore *store = g_object_get_data(G_OBJECT(clist), CLIST_STORE_KEY);
    return g_list_model_get_n_items(G_LIST_MODEL(store));
}

/* Get selected row */
gint
xfr_clist_get_selected_row(GtkColumnView *clist)
{
    GtkSingleSelection *selection = g_object_get_data(G_OBJECT(clist),
                                                       CLIST_SELECTION_KEY);
    return gtk_single_selection_get_selected(selection);
}

/* Get selected rows as GList (for compatibility with ->selection member) */
GList*
xfr_clist_get_selection(GtkColumnView *clist)
{
    GtkSingleSelection *selection = g_object_get_data(G_OBJECT(clist),
                                                       CLIST_SELECTION_KEY);
    guint selected = gtk_single_selection_get_selected(selection);

    if (selected == GTK_INVALID_LIST_POSITION)
        return NULL;

    /* Return a GList with a single element (the row number) */
    return g_list_append(NULL, GUINT_TO_POINTER(selected));
}

/* Get number of columns */
gint
xfr_clist_get_columns(GtkColumnView *clist)
{
    return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(clist), CLIST_NCOLS_KEY));
}

/* Set column width */
void
xfr_clist_set_column_width(GtkColumnView *clist, gint column, gint width)
{
    GListModel *columns = gtk_column_view_get_columns(clist);
    GtkColumnViewColumn *col = g_list_model_get_item(columns, column);

    if (col) {
        gtk_column_view_column_set_fixed_width(col, width);
        g_object_unref(col);
    }
}

/* Show column titles */
void
xfr_clist_column_titles_show(GtkColumnView *clist)
{
    /* In GTK-4 GtkColumnView, headers are always shown by default */
    /* This function is kept for API compatibility but doesn't need to do anything */
}

/* Set column widget (header widget) */
void
xfr_clist_set_column_widget(GtkColumnView *clist, gint column, GtkWidget *widget)
{
    GListModel *columns = gtk_column_view_get_columns(clist);
    GtkColumnViewColumn *col = g_list_model_get_item(columns, column);

    if (col) {
        /* In GTK-4, column headers are managed differently */
        /* We'll extract the label text from the widget if it's a label */
        if (GTK_IS_LABEL(widget)) {
            const gchar *text = gtk_label_get_text(GTK_LABEL(widget));
            gtk_column_view_column_set_title(col, text);
        }
        g_object_unref(col);
    }
}

/* Selection callback support - structure to hold callback info */
typedef struct {
    GCallback callback;
    gpointer user_data;
    GtkWidget *clist_widget;
} CListSelectionCallbackData;

/* Wrapper that converts GtkSelectionModel::selection-changed to old GtkCList::select_row format */
static void
selection_changed_wrapper(GtkSelectionModel *model,
                          guint position,
                          guint n_items,
                          gpointer user_data)
{
    CListSelectionCallbackData *data = (CListSelectionCallbackData *)user_data;
    guint selected;

    /* Get the currently selected row */
    if (GTK_IS_SINGLE_SELECTION(model)) {
        selected = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION(model));

        /* If there's a valid selection, call the original callback */
        if (selected != GTK_INVALID_LIST_POSITION) {
            /* Old GtkCList select_row callback signature:
             * void callback(GtkCList *clist, gint row, gint column, gpointer user_data)
             * We pass -1 for column since GtkCList selection didn't specify column */
            typedef void (*SelectRowCallback)(GtkWidget*, gint, gint, gpointer);
            SelectRowCallback cb = (SelectRowCallback)data->callback;
            cb(data->clist_widget, (gint)selected, -1, data->user_data);
        }
    }
}

/* Free the callback data when the widget is destroyed */
static void
free_selection_callback_data(gpointer data, GClosure *closure)
{
    g_free(data);
}

/* Set up a selection callback for a CList (GtkColumnView)
 * This function connects the GTK-4 selection-changed signal to work like
 * the old GtkCList select_row signal */
void
xfr_clist_set_selection_callback(GtkColumnView *clist,
                                  GCallback callback,
                                  gpointer user_data)
{
    GtkSingleSelection *selection;
    CListSelectionCallbackData *cb_data;

    g_return_if_fail(GTK_IS_COLUMN_VIEW(clist));
    g_return_if_fail(callback != NULL);

    /* Get the selection model */
    selection = g_object_get_data(G_OBJECT(clist), CLIST_SELECTION_KEY);
    if (!selection) {
        g_warning("xfr_clist_set_selection_callback: Could not get selection model");
        return;
    }

    /* Allocate callback data structure */
    cb_data = g_new0(CListSelectionCallbackData, 1);
    cb_data->callback = callback;
    cb_data->user_data = user_data;
    cb_data->clist_widget = GTK_WIDGET(clist);

    /* Connect the selection-changed signal with our wrapper */
    g_signal_connect_data(selection, "selection-changed",
                          G_CALLBACK(selection_changed_wrapper),
                          cb_data,
                          free_selection_callback_data,
                          0);
}
