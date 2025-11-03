/*  Gtk+ XFRESCO
 *  Copyright (C) 1999-2003  Antonio M. Moro
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <gtk/gtk.h>

#include "clist_compat.h"
#include "utils.h"
#include "support.h"
#include "xfresco.h"

#define MAX_COL_CLIST 40


extern gboolean GPRINT;


/*
extern GtkWidget *lookup_widget(GtkWidget *,gchar *);
*/
extern GtkWidget *main_window;


/* This shows a simple dialog box with a label and an 'OK' button.
   Example usage:
   glade_util_show_message_box ("Error saving file");
   */
void
glade_util_show_message_box (gchar * message)
{
  GtkWidget *dialog, *label, *button;

  dialog = gtk_dialog_new ();
  /* gtk_window_position removed in GTK-3 */

  /* gtk_window_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER); */
  /* gtk_container_border_width removed in GTK-3 - use CSS margins */

  /* gtk_container_border_width (GTK_CONTAINER (dialog), 5); */

  label = gtk_label_new (message);
  /* gtk_misc_set_padding removed in GTK-4 - use CSS padding */

  /* gtk_misc_set_padding (GTK_MISC (label), 20, 20); */
  /* gtk_box_pack_start removed in GTK-4 - use gtk_box_append */
  /* gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area(GTK_DIALOG(dialog))), label, */
  /* TRUE, TRUE, 0); */
  gtk_widget_show (label);

  button = gtk_button_new_with_label ("OK");
  /* gtk_widget_set_usize removed in GTK-2 - use gtk_widget_set_size_request */

  /* gtk_widget_set_usize (button, 80, -1); */
  /* gtk_box_pack_start removed in GTK-4 - use gtk_box_append */
  /* gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area(GTK_DIALOG(dialog))), button, */
  /* FALSE, FALSE, 14); */
  /* GTK_WIDGET_SET_FLAGS removed in GTK-3 */

  /* GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT); */
  /* gtk_widget_grab_default removed in GTK-4 - use gtk_window_set_default_widget */

  /* gtk_widget_grab_default (button); */
  gtk_widget_show (button);

  g_signal_connect_object(G_OBJECT (button), "clicked",
			     G_CALLBACK (gtk_window_destroy), G_OBJECT(dialog), 0);
  gtk_widget_show (dialog);

}


/* This shows a dialog box with a message and a number of buttons.
 * Signal handlers can be supplied for any of the buttons.
 * NOTE: The dialog is automatically destroyed when any button is clicked.
 * default_button specifies the default button, numbered from 1..
 * data is passed to the signal handler.

   Example usage:
     GtkWidget *dialog;
     gchar *buttons[] = { "Yes", "No", "Cancel" };
     GtkSignalFunc signal_handlers[] = { on_yes, on_no, NULL };

     dialog = glade_util_create_dialog_with_buttons ("Do you want to save the current project?", 3, buttons, 3, signal_handlers, NULL);
     gtk_widget_show (dialog);
 */
/* GtkWidget* */
/* glade_util_create_dialog_with_buttons (const gchar * message, */
/* 				       gint nbuttons, */
/* 				       const gchar * buttons[], */
/* 				       gint default_button, */
/* 				       GtkSignalFunc signal_handlers[], */
/* 				       gpointer data) */

GtkWidget*
glade_util_create_dialog_with_buttons (const gchar * message,
				       gint nbuttons,
				       const gchar * buttons[],
				       gint default_button,
				       GCallback signal_handlers[],
				       gpointer data)
{
  GtkWidget *dialog, *hbox, *label, *button, *bbox;
  int i;

  dialog = gtk_dialog_new ();
  /* gtk_window_set_position removed in GTK-3 */

  /* gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER); */
  /* gtk_container_set_border_width removed in GTK-3 - use CSS margins */

  /* gtk_container_set_border_width (GTK_CONTAINER (dialog), 5); */

  hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  /* gtk_container_set_border_width removed in GTK-3 - use CSS margins */

  /* gtk_container_set_border_width (GTK_CONTAINER (hbox), 20); */
  /* gtk_box_pack_start removed in GTK-4 - use gtk_box_append */
  /* gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area(GTK_DIALOG(dialog))), hbox, */
  /* TRUE, TRUE, 0); */
  gtk_widget_show (hbox);

  label = gtk_label_new (message);
  /* gtk_box_pack_start removed in GTK-4 - use gtk_box_append */
  /* gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0); */
  gtk_widget_show (label);

  /* GtkButtonBox removed in GTK-4 - use GtkBox with orientation */


  bbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  /* gtk_button_box_set_layout removed in GTK-4 */

  /* gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END); */
  gtk_box_set_spacing (GTK_BOX (bbox), 10);
  /* gtk_box_pack_start removed in GTK-4 - use gtk_box_append */
  /* gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area(GTK_DIALOG(dialog))), bbox, */
  /* FALSE, TRUE, 0); */
  gtk_widget_show (bbox);

  for (i = 0; i < nbuttons; i++)
    {
      /* button = gtk_button_new_with_label (_(buttons[i]));*/
      button = gtk_button_new_with_label (buttons[i]);
      /* gtk_container_add removed in GTK-4 - use specific methods */

      gtk_box_append(GTK_BOX(bbox), button);
      /* GTK_WIDGET_SET_FLAGS removed in GTK-3 */

      /* GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT); */
      if (i == default_button - 1) {
	/* gtk_widget_grab_default removed in GTK-4 - use gtk_window_set_default_widget */

	/* gtk_widget_grab_default (button); */
	gtk_widget_grab_focus (button);
      }
      gtk_widget_show (button);

      if (signal_handlers[i])
	g_signal_connect (G_OBJECT (button), "clicked", signal_handlers[i],
			    data);
      
      g_signal_connect_object(G_OBJECT (button), "clicked",
				 G_CALLBACK (gtk_window_destroy), G_OBJECT(dialog), 0);

      /* g_signal_connect (G_OBJECT (dialog), "key_press_event", */
/* 			  G_CALLBACK (glade_util_check_key_is_esc), */
/* 			  GINT_TO_POINTER (GladeEscDestroys)); */
    }
  return dialog;
}


/* gint glade_util_check_key_is_esc (GtkWidget *widget, */
/* 				  GdkEventKey *event, */
/* 				  gpointer data) */
/* { */
/*   g_return_val_if_fail (GTK_IS_WINDOW (widget), FALSE); */
  
/*   if (event->keyval == GDK_Escape) */
/*     { */
/*       GladeEscAction action = GPOINTER_TO_INT (data); */
  
/*       if (action == GladeEscCloses) */
/*         { */
/*   	glade_util_close_window (widget); */
/*   	return TRUE; */
/*         } */
/*       else if (action == GladeEscDestroys) */
/*         {  */
/*   	gtk_window_destroy (widget); */
/*   	return TRUE; */
/*         } */
/*       else */
/*         return FALSE; */
/*     } */
/*   else */
/*     return FALSE; */
/* } */

/* gdk_window_get_root_origin and gtk_widget_set_uposition removed in GTK-4 */
gint
glade_util_close_window (GtkWidget * widget)
{
  /* TODO: Reimplement with modern GTK-4 positioning */
  gtk_widget_hide (widget);
  return TRUE;
}



/* This turns a relative pathname into an absolute one based on the given
   base directory (which MUST be absolute).
   e.g. "/home/damon" + "../dave/test" -> "/home/dave/test"
   The returned path should be freed when no longer needed. */
gchar*
glade_util_make_absolute_path (const gchar *dir, const gchar *file)
{
  gint dir_pos, file_pos, len, root_pos = 0;
  gchar *path;

  g_return_val_if_fail (dir != NULL, NULL);

  if (file == NULL || file[0] == '\0')
    return g_strdup (dir);

  if (g_path_is_absolute (file))
    return g_strdup (file);

  /* For windows if dir has a drive set, e.g. "C:\", we never delete that. */
#ifdef NATIVE_WIN32
  if (isalpha (dir[0]) && dir[1] == ':' && dir[2] == G_DIR_SEPARATOR)
    root_pos = 2;
#endif

  /* Start at last character in dir. */
  dir_pos = strlen (dir) - 1;

  /* First we make sure we skip any '/' at the end of dir. */
  if (dir_pos > root_pos && dir[dir_pos] == G_DIR_SEPARATOR)
    dir_pos--;

  /* Now for each '..' in file, we step back one component in dir, and
     forward one component in file. */
  file_pos = 0;
  for (;;)
    {
      /* Skip './' */
      if (file[file_pos] == '.' && file[file_pos + 1] == G_DIR_SEPARATOR)
	file_pos += 2;

      else if (file[file_pos] == '.' && file[file_pos + 1] == '.'
	       && (file[file_pos + 2] == G_DIR_SEPARATOR
		   || file[file_pos + 2] == '\0'))
	{
	  while (dir_pos > root_pos && dir[dir_pos] != G_DIR_SEPARATOR)
	    dir_pos--;
	  if (dir_pos > root_pos)
	    dir_pos--;

	  if (file[file_pos + 2] == G_DIR_SEPARATOR)
	    file_pos += 3;
	  else
	    file_pos += 2;
	}

      else
	break;
    }

  /* Now concatenate the parts of dir and file together. */
  if (dir_pos > root_pos)
    dir_pos++;
  len = dir_pos + 1 + (strlen (file) - file_pos) + 1;
  path = g_malloc (len); 
  strncpy (path, dir, dir_pos);
  path[dir_pos] = G_DIR_SEPARATOR;
  strcpy (path + dir_pos + 1, file + file_pos);
  return path;
}

/*Given the number of a certain page on a notebook, returns a 
  pointer to the clist contained on this page*/ 
GtkWidget *
get_notebook_child                     (GtkWidget       *notebook,
                                        gint             page_num)
{
  GtkNotebookPage *page;
  GtkWidget *notebook_page,*nieto;
  GList *children;
  
  g_print("In function get_notebook_child (utils.c):\n");
  if (page_num>gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook))){
    g_print("WARNING:Page %i does not exist in notebook\n",page_num);
  }

  /* not needed in gtk2.0 
     page = (GtkNotebookPage*) g_list_nth (GTK_NOTEBOOK (notebook)->children, page_num)->data; 
     notebook_page = page->child; */

  /* In GTK2.0 we can use:
   GtkWidget* gtk_notebook_get_nth_page (GtkNotebook *notebook,gint page_num); */

  notebook_page= gtk_notebook_get_nth_page(GTK_NOTEBOOK (notebook),page_num);

  if (GTK_IS_CLIST(notebook_page))
    {
       g_print("Clist found in notebook\n");
      return(notebook_page);
    }
  else
    {
      /* gtk_container_children removed in GTK-3 */

      children=NULL; /* TODO: Use gtk_widget_get_first_child() iteration */
      g_print("Recuperado children\n");
      while(children){
	nieto=children->data;
	if (GTK_IS_CLIST(nieto)){
	  g_print("Clist found in notebook\n");
	  return(nieto);
	}
	children=children->next;
      }
      g_print("\nWARNING!!:Could not find clist on notebook\n");
    }
  return(notebook_page);
}


/*******************************************************/
/*                  TOGGLE BUTTONS                     */
/*******************************************************/

/* Given a toggle button widget, returns the string "T" or "F"
 * depending on the state of the button
 */ 
gchar *
xfr_toggle_get_state(GtkWidget *pwf)
{
  gchar *value;
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pwf)))
      value=g_strdup("T");
  else
      value=g_strdup("F");
 return(value);
}

/*If value="false" sets the toggle button pressed*/
void
xfr_toggle_set_value(GtkWidget *toggle,gchar *value)
{  
  if (g_strncasecmp(value,"F",1)==0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),FALSE);
  else
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle),TRUE);
  
}




/*******************************************************/
/*                    COMBO BOX                        */
/*******************************************************/

/* GtkCombo and GtkList removed in GTK-4 - TODO: Rewrite with GtkDropDown */
/* void
xfr_combo_set_rows(GtkWidget *combo,gchar *text[]){
  gint i=0;
  GList *glist=NULL;
  while(text[i]){
    glist=g_list_append(glist,text[i]);
  }
  gtk_combo_set_popdown_strings(GTK_COMBO(combo),glist);
  g_list_free(glist);
} */


/*Selects an item on a combo box with name "comboname".
 * GtkCombo removed in GTK-4 - TODO: Rewrite with GtkDropDown */
/* void
xfr_combo_set_value(GtkWidget *refwidget,gchar *comboname,gint item){
     GtkWidget *combo;
     GtkWidget *combolist;
     GList *selist,*glist;

     combo=lookup_widget(refwidget,comboname);
     g_return_if_fail(combo!=NULL);
     combolist=GTK_COMBO(combo)->list;
     g_return_if_fail(combolist!=NULL);

     glist=GTK_LIST(combolist)->children;
     if(glist==NULL) {
       g_print("WARNING!!: combo %s empty\n",comboname);
       return;
     }
     if (item>g_list_length(glist)) {
       g_print("WARNING!!:on combo %s item %i, but there are only %i\n",
	       comboname,item,g_list_length(glist));
       return;
     }
     gtk_list_select_item(GTK_LIST(combolist),item);
     selist=GTK_LIST(combolist)->selection;
} */

/* Given a number, it selects the item of a combo_box whose label starts
 * with this number
 * GtkCombo removed in GTK-4 - TODO: Rewrite with GtkDropDown */
/* void
xfr_combo_set_char_value(GtkWidget *combo,gint item){
     GtkWidget *combolist;
     GList *glist;
     gint i=0;

     g_return_if_fail(combo!=NULL);
     combolist=GTK_COMBO(combo)->list;
     g_return_if_fail(combolist!=NULL);

     glist=GTK_LIST(combolist)->children;
     if(glist==NULL) return;

     while(glist){
       GList *child;
       gchar *buffer,*text;
       GtkLabel *label;
       GtkItem *gtkitem;
       gint n;

       gtkitem=GTK_ITEM(glist->data);
       child=gtk_container_children(GTK_CONTAINER(gtkitem));
       label=GTK_LABEL(child->data);
       gtk_label_get(label,&text);
       buffer=g_strndup(text,2);
       n=atoi(buffer);

       if (n==item){
	 gtk_list_select_item(GTK_LIST(combolist),i);
       }
       glist = glist->next;
       i++;
     }
} */

/* Returns the index of the item actually selected on a combo with name
 * "comboname" or -1 if there is no selection.
 * The first item is numbered 0
 * GtkCombo removed in GTK-4 - TODO: Rewrite with GtkDropDown */
/* gint
xfr_combo_get_value(GtkWidget *refwidget,gchar *comboname){
  gint i=0;
  GtkWidget *combo=lookup_widget(refwidget,comboname);
  GtkWidget *combolist;
  GList *glist=NULL;
  GList *selist;


  g_return_val_if_fail(combo!=NULL,-1);
  g_return_val_if_fail(GTK_IS_COMBO(combo),-1);

  combolist=GTK_COMBO(combo)->list;
  g_return_val_if_fail(combolist!=NULL,-1);

  glist=GTK_LIST(combolist)->children;
  selist=GTK_LIST(combolist)->selection;

  if (selist==NULL) return(-1);

  while (glist){
    if ((glist->data) == (selist->data)) return(i);
    i++;
    glist = glist->next;
  }
  return (-1);
} */

/* GTK-4 GtkDropDown implementation */
void xfr_combo_set_value(GtkWidget *refwidget, gchar *comboname, gint item) {
    GtkWidget *dropdown = lookup_widget(refwidget, comboname);
    if (dropdown == NULL) {
        g_warning("xfr_combo_set_value: Could not find widget '%s'", comboname);
        return;
    }

    if (GTK_IS_DROP_DOWN(dropdown)) {
        gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown), item);
    } else if (GTK_IS_COMBO_BOX(dropdown)) {
        gtk_combo_box_set_active(GTK_COMBO_BOX(dropdown), item);
    } else {
        g_warning("xfr_combo_set_value: Widget '%s' is not a GtkDropDown or GtkComboBox", comboname);
    }
}

void xfr_combo_set_char_value(GtkWidget *dropdown, gint item) {
    if (dropdown == NULL) {
        g_warning("xfr_combo_set_char_value: NULL widget");
        return;
    }

    if (GTK_IS_DROP_DOWN(dropdown)) {
        GtkStringList *list = GTK_STRING_LIST(gtk_drop_down_get_model(GTK_DROP_DOWN(dropdown)));
        guint n_items = g_list_model_get_n_items(G_LIST_MODEL(list));

        /* Find item whose label starts with the given number */
        for (guint i = 0; i < n_items; i++) {
            const gchar *str = gtk_string_list_get_string(list, i);
            if (str && atoi(str) == item) {
                gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown), i);
                return;
            }
        }
        g_warning("xfr_combo_set_char_value: Item starting with %i not found", item);
    } else if (GTK_IS_COMBO_BOX(dropdown)) {
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(dropdown));
        GtkTreeIter iter;
        gint i = 0;

        if (gtk_tree_model_get_iter_first(model, &iter)) {
            do {
                gchar *text;
                gtk_tree_model_get(model, &iter, 0, &text, -1);
                if (text && atoi(text) == item) {
                    gtk_combo_box_set_active(GTK_COMBO_BOX(dropdown), i);
                    g_free(text);
                    return;
                }
                g_free(text);
                i++;
            } while (gtk_tree_model_iter_next(model, &iter));
        }
        g_warning("xfr_combo_set_char_value: Item starting with %i not found", item);
    } else {
        g_warning("xfr_combo_set_char_value: Widget is not a GtkDropDown or GtkComboBox");
    }
}

gint xfr_combo_get_value(GtkWidget *refwidget, gchar *comboname) {
    GtkWidget *dropdown = lookup_widget(refwidget, comboname);
    if (dropdown == NULL) {
        g_warning("xfr_combo_get_value: Could not find widget '%s'", comboname);
        return 0;
    }

    if (GTK_IS_DROP_DOWN(dropdown)) {
        return gtk_drop_down_get_selected(GTK_DROP_DOWN(dropdown));
    } else if (GTK_IS_COMBO_BOX(dropdown)) {
        return gtk_combo_box_get_active(GTK_COMBO_BOX(dropdown));
    } else {
        g_warning("xfr_combo_get_value: Widget '%s' is not a GtkDropDown or GtkComboBox", comboname);
        return 0;
    }
}

gchar* xfr_option_menu_get_char_value(GtkWidget *refwidget, gchar *widname) {
    gint value = xfr_option_menu_get_value(refwidget, widname);
    return g_strdup_printf("%i", value);
}

gint xfr_option_menu_get_value(GtkWidget *refwidget, gchar *widname) {
    GtkWidget *dropdown = lookup_widget(refwidget, widname);
    if (dropdown == NULL) {
        g_warning("xfr_option_menu_get_value: Could not find widget '%s'", widname);
        return 0;
    }

    if (GTK_IS_DROP_DOWN(dropdown)) {
        return gtk_drop_down_get_selected(GTK_DROP_DOWN(dropdown));
    } else if (GTK_IS_COMBO_BOX(dropdown)) {
        return gtk_combo_box_get_active(GTK_COMBO_BOX(dropdown));
    } else {
        g_warning("xfr_option_menu_get_value: Widget '%s' is not a GtkDropDown or GtkComboBox", widname);
        return 0;
    }
}

/* Idem for a option_menu
 * GtkOptionMenu removed in GTK-4 - TODO: Rewrite with GtkDropDown */
/* gint
xfr_option_menu_get_value(GtkWidget *refwidget,gchar *widname){
  GtkWidget *optmenu=lookup_widget(refwidget,widname);
  GtkMenu *menu_shell=GTK_MENU(GTK_OPTION_MENU(optmenu)->menu);
  GtkWidget *active_item;
  gint item_index;


  active_item=gtk_menu_get_active(GTK_MENU(menu_shell));
  item_index=g_list_index(((GtkMenuShell *)menu_shell)->children,active_item);
  return(item_index);
} */

/* GtkOptionMenu removed in GTK-4 - TODO: Rewrite with GtkDropDown */
/* gchar *
xfr_option_menu_get_char_value(GtkWidget *ref,gchar *widname){
  GtkWidget *optmenu=lookup_widget(ref,widname);
  GtkMenu *menu_shell=GTK_MENU(GTK_OPTION_MENU(optmenu)->menu);
  GtkWidget *active_item;
  gint item_index;
  gchar *value;

  active_item=gtk_menu_get_active(GTK_MENU(menu_shell));
  item_index=g_list_index(((GtkMenuShell *)menu_shell)->children,active_item);
  value=g_strdup_printf("%i",item_index);
  return(value);
} */


/*Sets the elements of a row on a clist*/
void
xfr_clist_put_row(GtkWidget *clist,GList **glist){
  gint i=0;
  gchar *row[MAX_COL_CLIST];
  GList *list=*glist;

  /* g_print("\nNumero de columnas:%i",GTK_CLIST(clist)->columns);*/

  g_return_if_fail(clist!=NULL);

  /*Freeze clist*/
  /* gtk_clist_freeze removed in GTK-4 */

  /* gtk_clist_freeze
  ; */

  while(list){
    if (list){
      row[i]=(gchar *)list->data;
      list=list->next;
    }
    i++;
  }
  /*Remove elements from glist*/
  while(*glist){    
    *glist=g_list_remove_link(*glist,*glist);
  } 
  g_list_free(list);
  /*Append row*/
  gtk_clist_append(GTK_CLIST(clist),row);

  /*Unfreeze clist*/
  /* gtk_clist_thaw removed in GTK-4 */

  /* gtk_clist_thaw
  ; */
  
}


/* Dumps the content of clist1 to clist2
 * Previously deletes the content of clist2
 */
void
xfr_clist_to_clist(GtkCList *clist1, GtkCList *clist2){
  gint i,j;
  gint rows,columns;
  gchar *text;
  gchar *blankrow[xfr_clist_get_columns(clist1)];
  
  if (clist1==NULL) return;


  /* gtk_clist_freeze removed in GTK-4 */



  /* gtk_clist_freeze


  ; */

  /*Delete the content of clist2. It also remove the rows*/
  if (clist2){
    gtk_clist_clear(clist2);
    /* gtk_clist_freeze removed in GTK-4 */

    /* gtk_clist_freeze
    ; */
  }

   
/*   g_print("\nclist2 tiene %i rows y %i columnas",xfr_clist_get_rows(clist2),xfr_clist_get_columns(clist2)); */
/*   g_print("\nclist1 tiene %i rows y %i columnas",xfr_clist_get_rows(clist1),xfr_clist_get_columns(clist1)); */
  

  rows=xfr_clist_get_rows(clist1);
  columns=xfr_clist_get_columns(clist1);
  
  if(columns!=xfr_clist_get_columns(clist2)){
    g_print("\nIn function xfr_clist_to_clist: \n");
    g_print("clis1 & clist2 have different number of columns\n."); 
    g_print("Please, report this bug to moro@us.es. \n");
    return;
  }
     
  for(i=0;i<columns;i++){
    blankrow[i]="";
  }
  
  /*Fill clist2 with blank rows.*/
   for(i=0;i<rows;i++){
    gtk_clist_append(clist2,blankrow);
  }
  
  /*Copy content of clist1 to clist2*/
  for(i=0;i<rows;i++){
    for(j=0;j<columns;j++){
      gtk_clist_get_text(clist1,i,j,&text);
      gtk_clist_set_text(clist2,i,j,text);
    }
  }
  /* gtk_clist_thaw removed in GTK-4 */

  /* gtk_clist_thaw
  ; */
  /* gtk_clist_thaw removed in GTK-4 */

  /* gtk_clist_thaw
  ; */
}



/* This sets the text on a entry-widget using the string 
 * contained on a passed  Vardesc structure.
 * The entry widget is retrieved from the pointer 
 * data of a reference widget "ref"
 */
void
xfr_set_text(GtkWidget *ref,gchar *widname,Vardesc *vardesc){
  GtkWidget *widget=lookup_widget(ref,widname);
  /* g_print("Antes de set text\n");*/
  gtk_editable_set_text(GTK_EDITABLE(widget),valof(vardesc));
  /* g_print("Despues de set text\n"); */
}

/* Idem for arrays */
void
xfr_set_text_n(GtkWidget *ref,gchar *widname,Vardesc *vardesc,gint nval){
  GtkWidget *widget=lookup_widget(ref,widname);
  /* g_print("Antes de set text\n");*/
  gtk_editable_set_text(GTK_EDITABLE(widget),valof_n(vardesc,nval));
  /* g_print("Despues de set text\n"); */
}



/*Returns a COPY of the text from GTK_ENTRY type widgets*/
gchar *
xfr_get_text(GtkWidget *ref,gchar *widname){
  GtkWidget *widget=lookup_widget(ref,widname);
  gchar *entry=g_strdup(gtk_editable_get_text(GTK_EDITABLE(widget)));
  return(entry);
}




/* Given a state number and its partition number displays short text describing
 * this state, in the form: "name:energy(Jpi)"
 *  who=1 Proj. Returns "namep:energy(Jpi)"
 *  who=2 Targ. Returns "namet:energy(Jpi)"
 *  who=3 Both  Returns "namep:energy(Jpi)+namet:energy(Jpi)" 
 *
 * "somewidget" is any widget belonging to the main_window
 */
gchar *
state_desc(GtkWidget *somewidget,
	   gint part_number,
	   gint state,
	   guint who){
  
  /*Order of columns in partition clist*/
  enum{NAMEP,MASSP,ZP,NAMET,MASST,ZT,QVAL};
  
  /*Order of columns in state clist*/
  enum {
    JP,COPYP,BANDP,EP,KKP,TP,CPOT,
    JT,COPYT,BANDT,ET,KTT,TT,EXCH,IGNORE,INFAM,OUTFAM
  };
  
  gchar *buffer;
  gchar *namep,*namet;/*Name*/
  gchar *ep,*et;      /*Energy*/
  gchar *jp,*jt;      /*Spin*/
  gchar *bandp,*bandt;/*Parity*/
  gchar *signp,*signt;
  
  /* Partition clist */
  GtkCList *part_clist=GTK_CLIST(lookup_widget(somewidget,"part_clist"));
  GtkCList *states_clist=GTK_CLIST(gtk_clist_get_row_data(part_clist,\
							  part_number-1));
  
  if (part_number<1 || state < 1) return("not found");
  

  g_assert(part_clist!=NULL);
  g_assert(GTK_IS_CLIST(part_clist));

  g_assert(states_clist!=NULL);

  /* 25/04/05 */
  /* g_assert(GTK_IS_CLIST(states_clist)); */
 
  if (states_clist == NULL){
    g_warning("State %i from partition %i could not be identified. Maybe partition %i is missing?", \
     state,part_number+1,part_number+1);
  }

  /*Rows start at 0 so partition=1 is row=0, etc*/
  part_number--;

  /*Get projectile and target names for this partition*/ 
  gtk_clist_get_text(part_clist,part_number,NAMEP,&namep);
  gtk_clist_get_text(part_clist,part_number,NAMET,&namet);
   
  /*Get energy of projectile and target for this state*/
  if (state>xfr_clist_get_rows(states_clist)){
    g_warning("State %i does not exist on partition %i",state,part_number+1);
    return("not found");
  }

  /*Rows start at zero*/
  state--;

  gtk_clist_get_text(states_clist,state,EP,&ep);
  gtk_clist_get_text(states_clist,state,ET,&et);

  /*Get spin & partity of projectile and target*/
  gtk_clist_get_text(states_clist,state,JP,&jp);
  gtk_clist_get_text(states_clist,state,JT,&jt);

  gtk_clist_get_text(states_clist,state,BANDP,&bandp);
  gtk_clist_get_text(states_clist,state,BANDT,&bandt);

  /* if (atoi(bandp)>0) */ 
  
    signp=atoi(bandp)>0?g_strdup("+"):g_strdup("-");
    signt=atoi(bandt)>0?g_strdup("+"):g_strdup("-");
 
  
  switch(who){
  case 1:
    buffer=g_strconcat(namep," (",jp,signp,") ",ep,NULL);
    break;
  case 2:
    buffer=g_strconcat(namet," (",jt,signt,") ",et,NULL);
    break;
  case 3:
    buffer=g_strconcat(namep," (",ep,")+",namet," (",et,")",NULL);
    break;
  default:
    break;
  }
  /* g_print("On state_desc: description=%s\n",buffer);*/
  return(buffer);
}

GList *
get_partitions_list(GtkCList *clist){
  GList *glist=NULL;
  gint rows=xfr_clist_get_rows(clist);  /*Number of partitions*/
  gint i=0;

  while(i<rows){
    gchar *namep,*namet,*text;
    
    /*Get proj.& target names from partitions clist*/
    gtk_clist_get_text(clist,i,0,&namep); /*Proj. name is on column 0*/
    gtk_clist_get_text(clist,i,3,&namet); /*Targ. name is on column 3*/

    /*Description text*/
    text=g_strdup_printf("%i : %s+%s",i+1,namep,namet);
    /*  g_print("On get_partitions_list text=%s\n",text);*/
    glist=g_list_append(glist,text);
    i++;
  }
return(glist);
}



/* Get state that contains this value of kp and its partition */
/* Number of pages in states_nb = number of rows in part_clist */
gint
get_partition_number_from_kp(GtkCList *clist,gint kp){
  enum {IB,IBDESC,IA,IADESC,K,STR};
  enum {
    JP,COPYP,BANDP,EP,KKP,TP,CPOT,
    JT,COPYT,BANDT,ET,KTT,TT,FEXCH,IGNORE
  };
  gint state;
  gchar *text;
  gint pages=xfr_clist_get_rows(clist);
  GtkCList *states_clist;
  gint part_number;
  
  if (clist==NULL){
    g_warning("error retrieving partition number from kp");
    return(-1);
  }
  
  /* g_print("En get_part... kp=%i\n",kp); */
  /* g_print("CPOT en col=%i\n",CPOT); */ 
  /* g_print("\nNumero de particiones=%i\n",pages); */
  for(part_number=1;part_number<=pages;part_number++){
    /*states_clist=get_notebook_child(states_nb,part_number);*/
    states_clist=GTK_CLIST(gtk_clist_get_row_data(GTK_CLIST(clist),
						  part_number-1));
    
    for (state=1;state<=xfr_clist_get_rows(states_clist);state++){
      gtk_clist_get_text(GTK_CLIST(states_clist),state,CPOT,&text);
      /* g_print("on partition %i state %i cpot=%s\n",part_number,state,text); */
      if (atoi(text)==kp){
	/* g_print("partition %i found!!\n",part_number); */
	return(part_number);
      }
    }
  }
  return(-1);
}




/* Returns the text in row "fila" and column "columna" of "clist" */
/* Original function signature changed to match clist_compat.h */
gchar *
xfr_clist_get_text_old(GtkCList *clist,gint fila,gint columna){
  gchar *cell[xfr_clist_get_columns(clist)];
  if (gtk_clist_get_text(clist,fila,columna,&cell[columna])==1){
    gchar *buffer=g_strdup(cell[columna]);
    return(buffer);
  }
  else{
    gchar *buffer=g_strdup("error");
    const gchar *name=gtk_widget_get_name (GTK_WIDGET(clist));
    g_print("\n**ERROR**: Could not retrieve text from clist %s !\n",name);
    return(buffer);
  }
   
}



/* If GPRINT==TRUE parse the argument(s) with g_print */
void
xfr_print(const gchar *formato,...){
  va_list args;
  gchar *string;
  
  if(GPRINT==TRUE){
    va_start(args, formato);
    string=g_strdup_vprintf(formato,args);
    va_end(args);
    g_print("%s", string);
    g_free(string);
  }  
}
