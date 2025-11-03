/*  Gtk+ User Interface Builder
 *  Copyright (C) 1998  Damon Chaplin
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

/* #include <gtk/gtkfeatures.h> */
#include "namelist.h"
#include "clist_compat.h"


extern GtkWidget *main_window;

/*extern gboolean GPRINT;*/


typedef enum
{
  GladeEscCloses,
  GladeEscDestroys
} GladeEscAction;



/* This shows a simple dialog box with a label and an 'OK' button.
   Example usage:
    glade_util_show_message_box ("Error saving file");
 */
void	    
glade_util_show_message_box	(gchar		    *message);


/*
 * Filename Utility Functions.
 */
/* Free the returned values when you're finished with them. */
gchar*	  
glade_util_make_absolute_path	(const gchar	    *dir,
				 const gchar	    *file);

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
GtkWidget*
glade_util_create_dialog_with_buttons (const gchar * message,
				       gint nbuttons,
				       const gchar * buttons[],
				       gint default_button,
				       GCallback signal_handlers[],
				       gpointer data);

/* GtkWidget* */
/* glade_util_create_dialog_with_buttons (const gchar * message, */
/* 				       gint nbuttons, */
/* 				       const gchar * buttons[], */
/* 				       gint default_button, */
/* 				       GtkSignalFunc signal_handlers[], */
/* 				       gpointer data); */

/* This event handler performs the selected GladeEscAction when Esc is
   pressed in a dialog.
   It is not necessary for GnomeDialogs. */
gint 
glade_util_check_key_is_esc (GtkWidget *widget,
				  GdkEvent *event,
				  gpointer data);

gint	  
glade_util_close_window		(GtkWidget	*widget);



void
on_confirm_revert(GtkWidget *dialogo,gpointer file);

gchar *
xfr_toggle_get_state(GtkWidget *toggle);

void
xfr_toggle_set_value(GtkWidget *toggle,gchar *value);


GtkWidget *
get_notebook_child                     (GtkWidget       *notebook,
                                        gint             page_num);

void 
xfr_combo_set_rows(GtkWidget *combo,gchar *text[]);

void
xfr_combo_set_value(GtkWidget *,gchar *widname,gint item);

void
xfr_combo_set_char_value(GtkWidget *combo,gint item);

gint
xfr_combo_get_value(GtkWidget *refwidget,gchar *widname);

gchar *
xfr_option_menu_get_char_value(GtkWidget *refwidget,gchar *widname);

gint
xfr_option_menu_get_value(GtkWidget *refwidget,gchar *widname);

void
xfr_clist_put_row(GtkWidget *clist,GList **glist);

void
xfr_set_text(GtkWidget *refwidget,gchar *widgetname,Vardesc *vardesc);


/* Idem for arrays */
void
xfr_set_text_n(GtkWidget *ref,gchar *widname,Vardesc *vardesc,gint nval);

gchar *
xfr_get_text(GtkWidget *ref,gchar *widname);

void
xfr_spin_set_value(gchar *widname,Vardesc *vardesc);



/* Given a state number and its partition number displays short text describing
 * this state, in the form: "name:energy(Jpi)"
 *  who=1 Proj. Returns "namep:energy(Jpi)"
 *  who=2 Targ. Returns "namet:energy(Jpi)"
 *  who=3 Both  Returns "namep:energy(Jpi)+namet:energy(Jpi)" 
 *  who NOT IMPLEMENTED by now!!
 *
 * "somewidget" is any widget belonging to the main_window
 */
gchar *
state_desc(GtkWidget *somewidget,
	   gint part_number,
	   gint state,
	   guint who);


gint
get_partition_number_from_kp(GtkCList *clist,gint kpot);

GList *
get_partitions_list(GtkCList *clist);

/* Old signature - see clist_compat.h for new GTK-4 signature */
/* gchar *xfr_clist_get_text(GtkCList *clist,gint fil,gint col); */

void
xfr_print(const gchar *formato,...);




void
xfr_clist_to_clist(GtkCList *clist1, 
		   GtkCList *clist2);
