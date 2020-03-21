/*  Note: You are free to use whatever license you want.
    Eventually you will be able to edit it within Glade. */

/*  xfresco
 *  Copyright (C) <1999-2006> <Antonio M. Moro>
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
#include <unistd.h>
/*#include <sys/wait.h>*/
#include <string.h>

#include <gtk/gtk.h>

#include "support.h"
#include "utils.h"
#include "interface.h"
#include "callbacks.h"
#include "openfile.h"
#include "savefile.h"




/*************** Borrowed from Glade ********/

/* Complicated dialog boxes can be slow to create, so we create them once and
   keep pointers to them in static variables so we can reuse them. */
extern GtkWidget *open_filesel;
extern GtkWidget *save_filesel;

/* Number of warnings when reading input file */
  gint nwarnings=0;

/* This flag is set to TRUE if the file has been changed. We don't actually
   use it here, but the program could be extended to prompt to save changes. */
extern gboolean file_changed ;

/* A key used to store pointers to the main window. */
extern const gchar *MainWindowKey;

GtkWidget *main_window; 

extern gchar *current_filename;

extern gboolean GPRINT;

extern void open_file_ok(GtkWidget *, GtkFileSelection *);
extern void save_file_ok(GtkWidget *, GtkFileSelection *);

static GQuark             quark_uline_accel_group = 0;

enum{OPEN_NAMELIST,
       IMPORT_OLD,
       OPEN_NEW}; 

guint open_mode;


GtkWidget *qscale_clist=NULL;

/* extern GtkWidget *qscale_clist; */



/* gint */
/* main (int argc, char *argv[]) */
/* { */
/*   /\* */
/*   windows 1 declared as global, so this line must be omitted: */
/*   GtkWidget *window1;*\/ */
/*   GtkWidget *window_ccwf; */
/*   GtkWidget *window_constants; */
/*   GtkWidget *window_jbord; */
/*   /\*GtkWidget *window_bins;*\/ */
/*   GtkWidget *window_Rmatrix; */
/*   GtkWidget *window_files; */
  
 

/*    guint context; */
/*   GtkWidget *status; */

/*   /\*guint32 delay=10000;*\/ */

/*   gtk_set_locale (); */
/*   gtk_init (&argc, &argv); */

/*   /\*Resouce file*\/  */
/*   gtk_rc_parse("xfresco.rc"); */

/*   /\* */
/*    * The following code was added by Glade to create one of each component */
/*    * (except popup menus), just so that you see something after building */
/*    * the project. Delete any components that you don't want shown initially. */
/*    *\/ */
/*   main_window = create_main_window (); */

/*   /\* This is automatically output by Glade to set the directories containing */
/*      the pixmaps. The first directory is the place where pixmaps will be */
/*      installed. The second directory is where the uninstalled pixmaps are, */
/*      so the pixmaps can still be found without installing the application. */
/*      Note that for Gnome applications, pixmaps are handled differently, and */
/*      the pixmaps must be installed. PACKAGE_DATA_DIR and PACKAGE_SOURCE_DIR */
/*      are set by configure.in and must appear in acconfig.h *\/ */
/*   add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps"); */

/*   /\* Commented in version 2.0 *\/ */
/*   /\*   add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps"); *\/ */
	
  
/*   /\*Dialog window with CCWF parameters*\/ */
/*   window_ccwf=create_window_ccwf(); */

/*   window_constants=create_window_constants(); */
/*   window_jbord=create_window_jbord(); */
/*   /\*window_bins=create_window_bins();*\/ */
/*   window_Rmatrix=create_window_Rmatrix(); */
/*   window_files=create_window_files(); */
  

/*   gtk_object_set_data(GTK_OBJECT(main_window), */
/* 		      "window_ccwf",window_ccwf); */
/*   gtk_object_set_data(GTK_OBJECT(main_window), */
/* 		      "window_constants",window_constants); */
/*   gtk_object_set_data(GTK_OBJECT(main_window), */
/* 		    "window_jbord",window_jbord); */
/*   /\*gtk_object_set_data(GTK_OBJECT(main_window), */
/*     "window_bins",window_bins);*\/ */
/*   gtk_object_set_data(GTK_OBJECT(main_window), */
/* 		     "window_Rmatrix",window_Rmatrix); */
/*   gtk_object_set_data(GTK_OBJECT(main_window), */
/* 		     "window_files",window_files); */

/*   status=lookup_widget(main_window,"statusbar"); */
/*   context=gtk_statusbar_get_context_id(GTK_STATUSBAR(status), */
/* 				       "main"); */
/*   gtk_statusbar_push( GTK_STATUSBAR(status), context,"OK"); */

/*   /\*Some extra signals that I don't know how to include with glade *\/ */
/*   /\*  add_extra_signals(main_window);*\/ */

/*   /\* Attach popup menu to clists *\/ */
/*   create_popup(); */

/*   gtk_widget_show (main_window); */

/*   parse_command_line (main_window,argc, argv); */


  

/*   /\*   */
/*   gtk_timeout_add(delay, */
/*                   timeout_callback, */
/*                   main_window); */
/* 		  *\/ */
 


/*   gtk_main (); */
/*   return 0; */
/* } */

void
create_popup(){
  GtkWidget *menuitem;
  GtkWidget *pot_clist, *menu_pot;
  pot_clist=lookup_widget(main_window,"pot_clist");

  /* Poput menu attatched to pot_clist */
   menu_pot = gtk_menu_new ();
   menuitem=BuildMenuItem ( "Delete", 0, NULL,menu_pot, NULL );
   gtk_signal_connect(GTK_OBJECT(menuitem),
		      "activate",
		      GTK_SIGNAL_FUNC(popup_delete),
		      NULL);
   menuitem=BuildMenuItem ( "Move up", 0, NULL, menu_pot, NULL );
   gtk_signal_connect(GTK_OBJECT(menuitem),
		      "activate",
		      GTK_SIGNAL_FUNC(popup_moveup),
		      NULL);
   menuitem=BuildMenuItem ( "Move down", 0, NULL, menu_pot, NULL );
   gtk_signal_connect(GTK_OBJECT(menuitem),
		      "activate",
		      GTK_SIGNAL_FUNC(popup_movedown),
		      NULL);

  

  /* Catch ANY event that occurs on the pot_clist - we'll narrow it down 
    in the handler */
    gtk_signal_connect( GTK_OBJECT( pot_clist ), "event"
                                           , GTK_SIGNAL_FUNC ( MousePressed )
                                           , GTK_OBJECT ( menu_pot ) );

}



void
popup_delete(GtkWidget *widget,gpointer data){
  gint row,i;
  GtkWidget *clist=lookup_widget(main_window,"pot_clist");
  GList *selection=GTK_CLIST(clist)->selection;
  static gchar *text[MAXCOLS];
  static gint flag=0;

 /*Row selected on step clist*/
  if (!selection) return;
  row=GPOINTER_TO_INT(selection->data);
  
/*   if (flag==1){ */
/*     g_print("text[1]=%s",text[i]); */
/*     flag=0; */
/*   } */
/*   else{ */
    g_print("Getting data from %d columns",GTK_CLIST(clist)->columns);
    for (i=0;i<GTK_CLIST(clist)->columns;i++){
      gtk_clist_get_text(GTK_CLIST(clist),row,i,&text[i]);
      flag=1;
/*     } */
  }


  if (!selection) return;
  gtk_clist_remove(GTK_CLIST(clist),row);
  gtk_clist_select_row(GTK_CLIST(clist),row==0?0:row-1,0);
  
}


void
popup_moveup(GtkWidget *widget,gpointer data){
  gint row;
  GtkWidget *clist=lookup_widget(main_window,"pot_clist");
  GList *selection=GTK_CLIST(clist)->selection;

  /*Row selected on step clist*/
  if (!selection) return;
  row=GPOINTER_TO_INT(selection->data);
  if (row==0) return;
  
  gtk_clist_swap_rows (GTK_CLIST(clist),row,row-1);
  gtk_clist_select_row(GTK_CLIST(clist),row-1,0);

}

void
popup_movedown(GtkWidget *widget,gpointer data){
  gint row;
  GtkWidget *clist=lookup_widget(main_window,"pot_clist");
  GList *selection=GTK_CLIST(clist)->selection;

  /*Row selected on step clist*/
  if (!selection) return;
  row=GPOINTER_TO_INT(selection->data);
  if (row==(GTK_CLIST(clist)->rows)) return;
  gtk_clist_swap_rows (GTK_CLIST(clist),row,row+1);
  
}


gint
timeout_callback(gpointer mw){
  GtkWidget *statusbar=lookup_widget(GTK_WIDGET(mw),"statusbar");
  gchar *buffer="OK";
  gtk_statusbar_push( GTK_STATUSBAR(statusbar), 1, buffer);
  /* g_print("\nLlamada a timeout"); */
  return(0);
}



/***************************************************************
 *                        MENUBAR                              *
 ***************************************************************/

void
on_file_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{  
  /*Not used*/
}


void
on_Open_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         data)
{
  open_mode=OPEN_NAMELIST;
  /*Create file selector if it hasn't been created yet*/
  if (open_filesel == NULL)
    open_filesel = create_open_filesel ();
  
  /*Attach main_window to it*/
  gtk_object_set_data (GTK_OBJECT (open_filesel),\
		       "main_window", main_window);
  
  gtk_widget_show (open_filesel);
  gdk_window_raise (open_filesel->window);
}

void
on_import_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  open_mode=IMPORT_OLD;

  if (open_filesel == NULL)
    open_filesel = create_open_filesel ();
  
  /*Attach main_window to it*/
  gtk_object_set_data (GTK_OBJECT (open_filesel), "main_window", main_window);
  
  gtk_widget_show (open_filesel);
  gdk_window_raise (open_filesel->window);
  return;
}



void
on_revert_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *dialog;
  const gchar *buttons[] = {"OK","Cancel"};
  GCallback handlers[] = {G_CALLBACK(on_confirm_revert), NULL };
 /* GtkSignalFunc handlers[] = {on_confirm_revert, NULL }; */

  if(!current_filename){
    glade_util_show_message_box("There is no filename to revert");
    return;
  }

  dialog = glade_util_create_dialog_with_buttons ("Abandon current buffer?",
						  2, 
						  buttons, 2, 
						  handlers,
						  current_filename);

  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
  gtk_widget_show (dialog);

}


void
on_print_activate                     (GtkMenuItem     *menuitem,
				       gpointer         user_data){
/*   gchar *buffer=g_print("a2ps -Plp %s",current_filename); */
/*   if (system(buffer)==0){ */
/*     g_print("Printing %s",current_filename); */
/*   }else{ */
/*     g_print("Print command failed!\n"); */
/*   } */
}
  

void
on_confirm_revert(GtkWidget *dialogo,gpointer file){
  real_open_file(main_window,file);
}


void
on_New_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gchar *filename=".empty";
  FILE *f;
  gchar *buffer=g_strdup("EMPTY PROJECT\n NAMELIST\n &fresco /  &partition /  &pot / &overlap / &coupling /");
  GtkWidget *main_window=lookup_widget(GTK_WIDGET(menuitem),"main_window");
  /*gtk_statusbar_push( GTK_STATUSBAR(statusbar), 1, buffer);*/

  open_mode=OPEN_NEW;
  if ((f=fopen(filename,"w"))==NULL)
    {
      glade_util_show_message_box ("Error opening new file");
      g_print("\nERROR creating new file!");
      return;
    }
  /* g_print("buffer en new=%s",buffer); */
  fputs(buffer,f);
  fclose(f); 
  real_open_file(main_window,filename);
  remove(filename);   
}


/* Kill current run... dirty trick */
/** asıl kodu callbacks.c 'ye aktardım. run_file() ile ilişkili olduğu için.**/
void
on_kill_current_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
kill_current_run (main_window); 
}

void
on_About_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  /*Does nothing*/
}

void
on_run_activate                      (GtkMenuItem     *menuitem,
				      gpointer         user_data)
{
  
}



/***************************************************************************
 * Open File Selection Dialog.
 ***************************************************************************/

void
on_open_filesel_ok_button_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *filesel, *main_window;
  const gchar *filename;
  gchar *buffer;
  guint itest; /*value returned by system when it is succesfully executed*/
  gchar *echotest;
  /*gchar *tmpfile;*/

  filesel = gtk_widget_get_toplevel (GTK_WIDGET (button));
  main_window = gtk_object_get_data (GTK_OBJECT (filesel), "main_window");
  gtk_widget_hide (filesel);
  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
  g_print("\nOpening file %s...\n",filename);
  switch(open_mode){

  case OPEN_NAMELIST:
    real_open_file (main_window, filename);
    break;

  case IMPORT_OLD:
    /* I have to:
     1. Convert selected file to namelist style and save on tmp file
     2. Read this file
     3. Delete tmp file
     */    
    /* g_print("Creating tmp file..."); */
    /* tmpnam(tmpfile); */
    g_print("ok\n");
    current_filename=g_strdup_printf("%s-nl",filename);
    buffer=g_strdup_printf("fr2nl<%s>%s",filename,current_filename);
    echotest=g_strdup_printf("echo %s",buffer);

    itest=system(echotest);
    /*g_print("prueba system devuelve %i",system("echo hola"));*/
    if (system(buffer)!=itest) 
      g_print("**ERROR**:fr2nl failed!!\n");
    else
      real_open_file(main_window,current_filename);
    break;

  case OPEN_NEW:
    break;
  }
}


void
on_open_filesel_cancel_button_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{
   gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

/***************************************************************************
 * Save File Selection Dialog.
 ***************************************************************************/
void
on_Save_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *main_window;

  main_window = lookup_widget (GTK_WIDGET (menuitem), "main_window");

  /* If the current document doesn't have a filename yet, we call save_as
     to show the file selection dialog. */
  if (current_filename == NULL)
    save_as (main_window);
  else
    real_save_file (main_window, current_filename);
}


void
on_Save_as_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *main_window;

  main_window = lookup_widget (GTK_WIDGET (menuitem), "main_window");
  /*g_print("On on_Save_as_activate\n");*/
  save_as (main_window);
}

void
on_save_filesel_ok_button_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *filesel, *main_window;
  const gchar *filename;

  filesel = gtk_widget_get_toplevel (GTK_WIDGET (button));
  main_window = gtk_object_get_data (GTK_OBJECT (filesel), "main_window");
  gtk_widget_hide (filesel);
  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
  current_filename=g_strdup(filename);
  real_save_file (main_window, filename);
  /* g_free(filename);*/
}


void
on_save_filesel_cancel_button_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_Exit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

  GtkWidget *dialog;
  const gchar *buttons[] = {"OK","Cancel"};
  GCallback handlers[] = {G_CALLBACK(on_confirm_exit), NULL };

  /*  GtkSignalFunc handlers[] = {on_confirm_exit, NULL };*/

  dialog = glade_util_create_dialog_with_buttons ("Do you really want to quit?",
						  2, 
						  buttons, 2, 
						  handlers,
						  NULL);

  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
  gtk_widget_show (dialog);

}

void
on_confirm_exit(GtkWidget *button,gpointer data){
   gtk_main_quit();
}

  
void
on_main_window_destroy                 (GtkObject       *object,
                                        gpointer         user_data)
{
  gtk_main_quit();
  exit(0);
}


void
on_Edit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
 
}


void
on_Show_input_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  /*gchar *command=g_strdup_printf("less %s",current_filename);*/
  glade_util_show_message_box("Not implemented yet. Sorry!");
  /*system(command);*/
  return;
}



gboolean
on_open_filesel_delete_ev              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}



void
on_Run_options_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  glade_util_show_message_box("Not implemented yet. Sorry!");
  return; 
}

/** asıl on_Run_activate'i savefile.c ye aktardım. Böylece save+run'ı birleştirmiş oldum.**/
void
on_Run_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *main_window;

  main_window = lookup_widget (GTK_WIDGET (menuitem), "main_window");

  /* If the current document doesn't have a filename yet, we call save_as
     to show the file selection dialog. */
  if (current_filename == NULL)
    save_as (main_window);
  else
    {
     real_save_file (main_window, current_filename);
     run_file(main_window, current_filename);
    }
}

void
on_General_params_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *main_window=lookup_widget(GTK_WIDGET(menuitem),"main_window");
  GtkWidget *window_constants=lookup_widget(main_window,"window_constants");
 
  gtk_widget_show (window_constants);
  gdk_window_raise (window_constants->window);
}

void
on_Version_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *window_about=create_window_about();
  gtk_widget_show(window_about);
}



/*****************************************************************/
/**                           PARTITIONS                        **/
/*****************************************************************/
void
on_partitions_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gint i;
  gchar *text[COLUMNS_PARTITIONS];
  GtkWidget *pnucleus=lookup_widget(main_window,"pnucleus");
  GtkWidget *pmass=lookup_widget(main_window,"pmass");
  GtkWidget *pZ=lookup_widget(main_window,"pZ");
  GtkWidget *tnucleus=lookup_widget(main_window,"tnucleus");
  GtkWidget *tmass=lookup_widget(main_window,"tmass");
  GtkWidget *tZ=lookup_widget(main_window,"tZ");
  GtkWidget *qvalue=lookup_widget(main_window,"qvalue");
  GtkWidget *pwf=lookup_widget(main_window,"check_pwf");
  GtkWidget *check_nex=lookup_widget(main_window,"check_nex");
  GtkWidget *readstates=lookup_widget(main_window,"readstates");
  
  /*GtkNotebook *states_nb=GTK_NOTEBOOK(lookup_widget(main_window,"states_nb"));*/
  GtkCList *data_clist=GTK_CLIST(gtk_clist_get_row_data(clist,row));
  GtkCList *states_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(clist),"states_clist"));

 

  for (i=0;i<COLUMNS_PARTITIONS;i++){
      gtk_clist_get_text(GTK_CLIST(clist),row,i,&text[i]);
    }

  /*Put this text on the corresponding entries*/
  gtk_entry_set_text(GTK_ENTRY(pnucleus),text[PNUCLEUS]);
  gtk_entry_set_text(GTK_ENTRY(pmass),text[PMASS]);
  gtk_entry_set_text(GTK_ENTRY(pZ),text[PZ]);
  gtk_entry_set_text(GTK_ENTRY(tnucleus),text[TNUCLEUS]);
  gtk_entry_set_text(GTK_ENTRY(tmass),text[TMASS]);
  gtk_entry_set_text(GTK_ENTRY(tZ),text[TZ]);
  gtk_entry_set_text(GTK_ENTRY(qvalue),text[QVALUE]);
  gtk_entry_set_text(GTK_ENTRY(readstates),text[READSTATES]);

  if (g_strcasecmp(text[PWF],"T")==0) xfr_toggle_set_value(pwf,"TRUE");
  else xfr_toggle_set_value(pwf,"FALSE");

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_nex),
			       g_strcasecmp(text[NEX],"T")!=0?TRUE:FALSE);
    

  gtk_clist_clear(GTK_CLIST(states_clist));
  if (data_clist!=NULL){
    xfr_clist_to_clist(data_clist,states_clist);
  }
  /* Select first row on states_clist */
  if (GTK_CLIST(states_clist)->rows>0)
    gtk_clist_select_row(GTK_CLIST(states_clist),0,0);

}

void
on_button_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  gint row,icol,c1,c2,c3,c4,c5;

  GtkWidget *pnucleus=lookup_widget(main_window,"pnucleus");
  GtkWidget *pmass=lookup_widget(main_window,"pmass");
  GtkWidget *pZ=lookup_widget(main_window,"pZ");
  GtkWidget *tnucleus=lookup_widget(main_window,"tnucleus");
  GtkWidget *tmass=lookup_widget(main_window,"tmass");
  GtkWidget *tZ=lookup_widget(main_window,"tZ");
  GtkWidget *qvalue=lookup_widget(main_window,"qvalue");
  GtkWidget *pwf=lookup_widget(main_window,"check_pwf");  
  GtkWidget *nex=lookup_widget(main_window,"check_nex");
  GtkWidget *readstates=lookup_widget(main_window,"readstates");
 
  guint pwf_active=GTK_TOGGLE_BUTTON(pwf)->active;
  guint nex_active=GTK_TOGGLE_BUTTON(nex)->active;

  GtkCList *partition_clist=GTK_CLIST(lookup_widget(main_window,"part_clist"));
  GtkCList *data_clist;
  
  const gchar *entries[]={gtk_entry_get_text(GTK_ENTRY(pnucleus)),
		    gtk_entry_get_text(GTK_ENTRY(pmass)),
		    gtk_entry_get_text(GTK_ENTRY(pZ)),
		    gtk_entry_get_text(GTK_ENTRY(tnucleus)),
		    gtk_entry_get_text(GTK_ENTRY(tmass)),
		    gtk_entry_get_text(GTK_ENTRY(tZ)),
		    gtk_entry_get_text(GTK_ENTRY(qvalue)),
		    pwf_active?"T":"F",
		    nex_active?"F":"T",
		    gtk_entry_get_text(GTK_ENTRY(readstates)) };

  /* options identifyes the button which has been pressed*/
  gint option =GPOINTER_TO_INT(user_data);
  
  /*Get the row which is selected in the potential clist*/
  GList *selection=partition_clist->selection;
  if (!selection) 
      row=1;
  else
      row=GPOINTER_TO_INT(selection->data);

  switch(option){
  case INSERT:
    gtk_clist_insert(partition_clist,row,(gchar**) entries);
    data_clist=GTK_CLIST(gtk_clist_new(COLUMNS_STATES));
    /*Attach data_clist to the row of part_clist containing their partition*/
    gtk_clist_set_row_data(partition_clist,row,data_clist);
    break;
    
  case REPLACE:
    if (!selection) 
      {
	glade_util_show_message_box("No partition selected!");
	return;
      }
    for(icol=0;icol<COLUMNS_PARTITIONS;icol++)
      {
	gtk_clist_set_text(partition_clist,row,icol,entries[icol]);
      }
    break;

  case DELETE:
    if (!selection) return;
    gtk_clist_remove(partition_clist,row);
    gtk_clist_select_row(partition_clist,row==0?0:row-1,0);
    /*Remove the corresponding page in the states notebook*/
    /*gtk_notebook_remove_page(GTK_NOTEBOOK(states_nb),row+1);*/

    break;

  case ADD:
    gtk_clist_append(partition_clist,(gchar**)entries);
    data_clist=GTK_CLIST(gtk_clist_new(COLUMNS_STATES));
    gtk_clist_set_row_data(partition_clist,
			   (partition_clist->rows)-1,
			   data_clist);
    break;
  }
  /*Refresh combo's that contain list of partitions:
   *Now:combo_lampl,combo_ic1,combo_ic2,combo_icto,combo_ictfrom
   */  

  c1=xfr_combo_get_value(main_window,"combo_lampl");
  c2=xfr_combo_get_value(main_window,"combo_ic1");
  c3=xfr_combo_get_value(main_window,"combo_ic2");
  c4=xfr_combo_get_value(main_window,"combo_icto");
  c5=xfr_combo_get_value(main_window,"combo_ictfrom");

  gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,"combo_lampl")),
				get_partitions_list(partition_clist));
  gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,"combo_ic1")),
				get_partitions_list(partition_clist));
  gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,"combo_ic2")),
				get_partitions_list(partition_clist));
  gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,"combo_icto")),
				get_partitions_list(partition_clist));
  gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,"combo_ictfrom")),
				get_partitions_list(partition_clist));
  

  
  xfr_combo_set_value(main_window,"combo_lampl",c1);
  xfr_combo_set_value(main_window,"combo_ic1",c2);
  xfr_combo_set_value(main_window,"combo_ic2",c3);
  xfr_combo_set_value(main_window,"combo_icto",c4);
  xfr_combo_set_value(main_window,"combo_ictfrom",c5);
  

}
void
on_states_select_row               (GtkCList        *clist,
				    gint             row,
				    gint             column,
				    GdkEvent        *event,
				    gpointer         user_data)
{
  gint i;
  gchar *text[COLUMNS_STATES];

  enum {
    JP,COPYP,BANDP,EP,KKP,TP,CPOT,
    JT,COPYT,BANDT,ET,KKT,TT,FEXCH,IGNORE,INFAM,OUTFAM
  };

  GtkWidget *jp=lookup_widget(main_window,"jp");
  GtkWidget *copyp=lookup_widget(main_window,"copyp");
  GtkWidget *bandp=lookup_widget(main_window,"bandp");
  GtkWidget *ep=lookup_widget(main_window,"ep");
  GtkWidget *kkp=lookup_widget(main_window,"kkp");
  GtkWidget *tp=lookup_widget(main_window,"tp");
  GtkWidget *cpot=lookup_widget(main_window,"cpot");
  /* GtkWidget *pwf=lookup_widget(main_window,"pwf"); */
  GtkWidget *jt=lookup_widget(main_window,"jt");
  GtkWidget *copyt=lookup_widget(main_window,"copyt");
  GtkWidget *bandt=lookup_widget(main_window,"bandt");
  GtkWidget *et=lookup_widget(main_window,"et");
  GtkWidget *kkt=lookup_widget(main_window,"kkt");
  GtkWidget *tt=lookup_widget(main_window,"tt");
  /*GtkWidget *extra=lookup_widget(main_window,"extra");*/
  GtkWidget *fexch=lookup_widget(main_window,"check_fexch");
  GtkWidget *ignore=lookup_widget(main_window,"ignore");
  GtkWidget *opt_infam=lookup_widget(main_window,"opt_infam");
  GtkWidget *spin_infam=lookup_widget(main_window,"spin_infam");
  GtkWidget *opt_outfam=lookup_widget(main_window,"opt_outfam");
  GtkWidget *index_pot=lookup_widget(main_window,"entry_index_pot");

  for (i=0;i<COLUMNS_STATES;i++){
    gtk_clist_get_text(GTK_CLIST(clist),row,i,&text[i]);
    /* g_print("i=%i text=%s\n",i,text[i]); */
  }

  /*Set the controls to the values selected on the clist*/
  gtk_entry_set_text(GTK_ENTRY(jp),text[JP]);
  gtk_entry_set_text(GTK_ENTRY(copyp),text[COPYP]);
  gtk_entry_set_text(GTK_ENTRY(ep),text[EP]);
  gtk_entry_set_text(GTK_ENTRY(kkp),text[KKP]);
  gtk_entry_set_text(GTK_ENTRY(tp),text[TP]);
  gtk_entry_set_text(GTK_ENTRY(jt),text[JT]);
  gtk_entry_set_text(GTK_ENTRY(copyt),text[COPYT]);
  gtk_entry_set_text(GTK_ENTRY(et),text[ET]);
  gtk_entry_set_text(GTK_ENTRY(kkt),text[KKT]);
  gtk_entry_set_text(GTK_ENTRY(tt),text[TT]);
  gtk_entry_set_text(GTK_ENTRY(cpot),text[CPOT]);

  if (atoi(text[BANDP])==-1)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bandp),TRUE);
  else
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bandp),FALSE);

  if (atoi(text[BANDT])==-1)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bandt),TRUE);
  else
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bandt),FALSE);

		     
  /*xfr_toggle_set_value(bandp,text[BANDP]);
  xfr_toggle_set_value(bandt,text[BANDT]);
  xfr_toggle_set_value(pwf,text[PWF]); */
  /*xfr_toggle_set_value(extra,text[EXTRA]);*/
  xfr_toggle_set_value(fexch,text[FEXCH]);
  xfr_toggle_set_value(ignore,text[IGNORE]);

  
  /* INFAM/OUTFAM */
  if ((i=atoi(text[INFAM]))==0){
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_infam),0);
  }

  if (i>0)
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_infam),1);
  if (i<0)
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_infam),2);
  
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_infam),abs(i));
  /*g_print("infam=%i",i);*/

  i=atoi(text[OUTFAM]);
  gtk_option_menu_set_history(GTK_OPTION_MENU(opt_outfam),i);
  gtk_entry_set_text(GTK_ENTRY(index_pot),\
		      g_strdup_printf("%i",row+1));
 

}


void
on_button_states_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gint row,icol;
  /*GtkWidget *states_nb=lookup_widget(main_window,"states_nb");*/
  /*gint nbpage=gtk_notebook_get_current_page(GTK_NOTEBOOK(states_nb));*/
  
  /*GtkCList *states_clist=GTK_CLIST(get_notebook_child(states_nb,nbpage));*/
  /*Partition for this state*/
  GtkCList *part_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"part_clist"));
  GList *part_sel=part_clist->selection;
  gint part_row=GPOINTER_TO_INT(part_sel->data);

  GtkCList *data_clist=GTK_CLIST(gtk_clist_get_row_data(part_clist,part_row));
  GtkCList *states_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"states_clist"));

  GtkWidget *jp=lookup_widget(main_window,"jp");
  GtkWidget *copyp=lookup_widget(main_window,"copyp");
  GtkWidget *bandp=lookup_widget(main_window,"bandp");
  GtkWidget *ep=lookup_widget(main_window,"ep");
  GtkWidget *kkp=lookup_widget(main_window,"kkp");
  GtkWidget *tp=lookup_widget(main_window,"tp");
  GtkWidget *cpot=lookup_widget(main_window,"cpot");
  /* GtkWidget *pwf=lookup_widget(main_window,"pwf"); */
  GtkWidget *jt=lookup_widget(main_window,"jt");
  GtkWidget *copyt=lookup_widget(main_window,"copyt");
  GtkWidget *bandt=lookup_widget(main_window,"bandt");
  GtkWidget *et=lookup_widget(main_window,"et");
  GtkWidget *kkt=lookup_widget(main_window,"kkt");
  GtkWidget *tt=lookup_widget(main_window,"tt");
  /*GtkWidget *extra=lookup_widget(main_window,"extra");*/
  GtkWidget *fexch=lookup_widget(main_window,"check_fexch");
  GtkWidget *ignore=lookup_widget(main_window,"ignore");
  GtkWidget *opt_infam=lookup_widget(main_window,"opt_infam");  
  /*  GtkWidget *spin_infam=lookup_widget(main_window,"spin_infam");*/
  GtkWidget *opt_outfam=lookup_widget(main_window,"opt_outfam");
  
  
  /*Row...*/
  const gchar *entries[]={gtk_entry_get_text(GTK_ENTRY(jp)),
		    gtk_entry_get_text(GTK_ENTRY(copyp)),
		    g_strdup_printf("%i",on_band_get_value(bandp)),
		    gtk_entry_get_text(GTK_ENTRY(ep)),
		    gtk_entry_get_text(GTK_ENTRY(kkp)),
		    gtk_entry_get_text(GTK_ENTRY(tp)),
		    gtk_entry_get_text(GTK_ENTRY(cpot)),
		    gtk_entry_get_text(GTK_ENTRY(jt)),
		    gtk_entry_get_text(GTK_ENTRY(copyt)), 
		    g_strdup_printf("%i",on_band_get_value(bandt)),
		    gtk_entry_get_text(GTK_ENTRY(et)),
		    gtk_entry_get_text(GTK_ENTRY(kkt)),
		    gtk_entry_get_text(GTK_ENTRY(tt)),
		    xfr_toggle_get_state(fexch),
		    xfr_toggle_get_state(ignore),
		    g_strdup_printf("%i",on_infam_get_value(opt_infam)),
		    g_strdup_printf("%i",on_outfam_get_value(opt_outfam))};
                   


  /*Get the data of the selected row*/
  gint option =GPOINTER_TO_INT(user_data);
  GList *selection=states_clist->selection;
  GList *selection2=part_clist->selection;


  /* It does NOT work!!! Why? */ 
  if (!selection2){
    g_warning("No partition selected!\n");
    glade_util_show_message_box("No partition selected!");
    return;
  }
    

  if (!selection)
      row=1;
  else
      row=GPOINTER_TO_INT(selection->data);

  switch(option)
    {
    case INSERT:
      gtk_clist_insert(states_clist,row,(gchar**)entries);
      break;
      
    case REPLACE:
      if (!selection) 
	{
	  glade_util_show_message_box("No state selected!");
	  return;
	}
      for(icol=0;icol<COLUMNS_STATES;icol++)
	{
	  gtk_clist_set_text(states_clist,row,icol,entries[icol]);
	}
      break;
      
    case DELETE:
      if (!selection) return;
      gtk_clist_remove(states_clist,row);
      gtk_clist_select_row(states_clist,row-1,0);
      break;
      
    case ADD:
      gtk_clist_append(states_clist,(gchar**)entries);
      break;
    }

   /*Dump the content of step_clist to the list attached to the gpointer data
   *of the selected row in pot_clist,that is,the clist holding the potentials
   */
  xfr_clist_to_clist(GTK_CLIST(states_clist),GTK_CLIST(data_clist));
  
}

/*Returns +1 or -1 */
gint
on_band_get_value(GtkWidget *band){
  gint iband=GTK_TOGGLE_BUTTON(band)->active? -1:1;
  return(iband);
}

/*Toggle button bandp/bandt */
void
on_band_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
  /* GtkWidget *label=GTK_BUTTON(button)->child;*/
  guint active=GTK_TOGGLE_BUTTON(button)->active;

  /*gtk_label_set(GTK_LABEL(label),active? "-":"+");*/
  /*  gtk_button_set_label          (GtkButton      *button,
      const gchar    *label);*/
  gtk_button_set_label (GTK_BUTTON(button),active? "-":"+");
}


gint
on_infam_get_value(GtkWidget *opt){
  GtkWidget *spin=lookup_widget(main_window,"spin_infam");
  const gchar *infam=gtk_entry_get_text(GTK_ENTRY(spin));
  switch(xfr_option_menu_get_value(opt,"opt_infam")){
  case 0:
    return(0);
  case 1:
    return(atoi(infam));
  case 2:
    return(-atoi(infam));
  default:
    g_print("ERROR in option menu infam");
  }
return(-1);
}
	

gint
on_outfam_get_value(GtkWidget *opt){   
  switch(xfr_option_menu_get_value(opt,"opt_outfam")){
  case 0:
    return(0);
  case 1:
    return(1);
  case 2:
    return(-1);
  default:
    g_print("ERROR in option menu outfam");
  }
return(-1);
}

/*********************************************************************/
/*                              POTENTIALS                           */
/*********************************************************************/

void
on_pot_clist_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  GtkWidget *step_clist=lookup_widget(GTK_WIDGET(clist),"step_clist");
  GtkWidget *hbox_step=lookup_widget(GTK_WIDGET(clist),"hbox_step");
  gint i=0;
  gchar *buffer;
  GtkWidget *p[8];
  GtkWidget *lab_p[8];/*Labels of p1...p7*/
  gint type,kp;
  enum{KP,TYPE,SHAPE,ITT,P1,P2,P3,P4,P5,P6,P7,ADD};
  enum{IB,IBDESC,IA,IADESC,K,STR};

  GtkWidget *frame_states=lookup_widget(GTK_WIDGET(clist),"frame_states");
  GtkWidget *buttons_step=lookup_widget(GTK_WIDGET(clist),"buttons_step");
  GtkWidget *entry_kp=lookup_widget(GTK_WIDGET(clist),"spin_kp");
  GtkWidget *combo_shape=lookup_widget(GTK_WIDGET(clist),"combo_shape");
  GtkWidget *check_itt=lookup_widget(GTK_WIDGET(clist),"check_itt");
  GtkWidget *check_type=lookup_widget(GTK_WIDGET(clist),"check_type");

  p[1]=lookup_widget(GTK_WIDGET(clist),"entry_p1");
  p[2]=lookup_widget(GTK_WIDGET(clist),"entry_p2");
  p[3]=lookup_widget(GTK_WIDGET(clist),"entry_p3");
  p[4]=lookup_widget(GTK_WIDGET(clist),"entry_p4");
  p[5]=lookup_widget(GTK_WIDGET(clist),"entry_p5");
  p[6]=lookup_widget(GTK_WIDGET(clist),"entry_p6");
  p[7]=lookup_widget(GTK_WIDGET(clist),"entry_p7");

  lab_p[1]=lookup_widget(GTK_WIDGET(clist),"lab_p1");
  lab_p[2]=lookup_widget(GTK_WIDGET(clist),"lab_p2");
  lab_p[3]=lookup_widget(GTK_WIDGET(clist),"lab_p3");
  lab_p[4]=lookup_widget(GTK_WIDGET(clist),"lab_p4");
  lab_p[5]=lookup_widget(GTK_WIDGET(clist),"lab_p5");
  lab_p[6]=lookup_widget(GTK_WIDGET(clist),"lab_p6");
  

  /* Extract the values of the cell and put them on their corresponding
   * controls:
   * Columns order: kp,type,shape,itt,p1,p2,p3,p4,p5,p6,p7,type<0
   */
  gtk_clist_get_text(GTK_CLIST(clist),row,KP,&buffer); 
  gtk_entry_set_text(GTK_ENTRY(entry_kp),buffer);
  kp=abs(atoi(buffer));

  gtk_clist_get_text(GTK_CLIST(clist),row,TYPE,&buffer);
  type=abs(atoi(buffer));

  /* g_print("TYPE=%s selected\n",buffer); */
  xfr_combo_set_value(GTK_WIDGET(clist),"combo_type",abs(atoi(buffer)));
  /*TENGO QUE VER SI LA INFORMACION TYPE<0 VA A IR EN UNA COLUMNA SEPARADA*/
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_type),\
			       atoi(buffer)<0?TRUE:FALSE);

  gtk_clist_get_text(GTK_CLIST(clist),row,ADD,&buffer); 
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_type),\
			       g_strcasecmp(buffer,"T")==0?TRUE:FALSE);
  

  gtk_clist_get_text(GTK_CLIST(clist),row,SHAPE,&buffer);
  xfr_combo_set_char_value(combo_shape,atoi(buffer));
  /*xfr_combo_set_value("combo_shape",atoi(buffer));*/
  
  for(i=1;i<8;i++){
    gtk_clist_get_text(GTK_CLIST(clist),row,i+P1-1,&buffer);  
    /*g_print("\nText of p(%i)=%s",i,buffer);*/
    gtk_entry_set_text(GTK_ENTRY(p[i]),buffer);
  }

  gtk_clist_get_text(GTK_CLIST(clist),row,ITT,&buffer);
  xfr_toggle_set_value(check_itt,buffer);
  
  /*Labels on potential parameters p1... p7
    gtk_label_set_text(GTK_LABEL(lab_p[1]),"V");
  COMPLETAR !!!*/

   gtk_clist_clear(GTK_CLIST(step_clist));

  /* If any, show the corresponding clist step (couplings table)
   * This clist was attached on the gpointer data of the row 
   * for this potential */
  if (type>=12 && type<=17){
    GtkWidget *part_clist=lookup_widget(main_window,"part_clist");
    GtkWidget *data_clist=gtk_clist_get_row_data(clist,row);
    
    g_return_if_fail(part_clist!=NULL);


    /* Update information on columns IB-Desc and IA-Desc of data_clist */
    for(i=0;i<GTK_CLIST(data_clist)->rows;i++){
      gint state;
      guint who;
      gint pn=get_partition_number_from_kp(GTK_CLIST(part_clist),kp);
      switch(type){
      case 12: case 14:
	who=1; break;
      case 13: case 15:
	who=2; break;
      case 16: case 17:
	who=3; break;
      default:
	who=3;
	break;
      }
      gtk_clist_get_text(GTK_CLIST(data_clist),i,IB,&buffer);
      state=atoi(buffer);
      buffer=state_desc(part_clist,pn,state,who);
      gtk_clist_set_text(GTK_CLIST(data_clist),i,IBDESC,buffer);
      gtk_clist_get_text(GTK_CLIST(data_clist),i,IA,&buffer);
      state=atoi(buffer);
      buffer=state_desc(part_clist,pn,state,who);
      gtk_clist_set_text(GTK_CLIST(data_clist),i,IADESC,buffer);
    }
    
   
    /* Dump content of data_clist on step_clist */
    xfr_clist_to_clist(GTK_CLIST(data_clist),GTK_CLIST(step_clist));
    gtk_widget_set_sensitive(hbox_step,TRUE);
    gtk_widget_set_sensitive(frame_states,TRUE);
    gtk_widget_set_sensitive(step_clist,TRUE);
    gtk_widget_set_sensitive(buttons_step,TRUE);
    
    gtk_clist_select_row(GTK_CLIST(step_clist),0,0);
    
    
  }
  else{
    /*g_warning("namelist step could not be retrieved from row %i\n",row);*/
    gtk_widget_set_sensitive(hbox_step,FALSE);
    gtk_widget_set_sensitive(buttons_step,FALSE);
    gtk_widget_set_sensitive(step_clist,FALSE);
    gtk_clist_clear(GTK_CLIST(step_clist));
  }
}


void
on_button_potential_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  gint icol;
  gint row; /*Selected row*/
  GtkCList *pot_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"pot_clist"));

  /*Identify selected button*/
  gint option =GPOINTER_TO_INT(user_data);
  GtkWidget *step_clist;
  
  /*Get the data of the selected row*/
  GList *selection=pot_clist->selection;

  GtkWidget *spin_kp=lookup_widget(GTK_WIDGET(button),"spin_kp");
  GtkCombo *combo_type=GTK_COMBO(lookup_widget(GTK_WIDGET(button),\
				     "combo_type"));
  GtkWidget *check_type=lookup_widget(GTK_WIDGET(button),"check_type");
  GtkCombo *combo_shape=GTK_COMBO(lookup_widget(GTK_WIDGET(button),\
						"combo_shape"));
  GtkEntry *p1=GTK_ENTRY(lookup_widget(GTK_WIDGET(button),"entry_p1"));
  GtkEntry *p2=GTK_ENTRY(lookup_widget(GTK_WIDGET(button),"entry_p2"));
  GtkEntry *p3=GTK_ENTRY(lookup_widget(GTK_WIDGET(button),"entry_p3"));
  GtkEntry *p4=GTK_ENTRY(lookup_widget(GTK_WIDGET(button),"entry_p4"));
  GtkEntry *p5=GTK_ENTRY(lookup_widget(GTK_WIDGET(button),"entry_p5"));
  GtkEntry *p6=GTK_ENTRY(lookup_widget(GTK_WIDGET(button),"entry_p6"));
  GtkEntry *p7=GTK_ENTRY(lookup_widget(GTK_WIDGET(button),"entry_p7"));
  GtkWidget *itt=lookup_widget(GTK_WIDGET(button),"check_itt");
  gchar *type=g_strndup(gtk_entry_get_text(GTK_ENTRY(combo_type->entry)),2);
  
  const gchar *entries[]={
    gtk_entry_get_text(GTK_ENTRY (spin_kp)),
    type,
    g_strndup(gtk_entry_get_text(GTK_ENTRY(combo_shape->entry)),2),
    xfr_toggle_get_state(itt),
    gtk_entry_get_text(p1),
    gtk_entry_get_text(p2),
    gtk_entry_get_text(p3),
    gtk_entry_get_text(p4),
    gtk_entry_get_text(p5),
    gtk_entry_get_text(p6),
    gtk_entry_get_text(p7),
    xfr_toggle_get_state(check_type)
  };
  
  /*Row selected on step clist*/
  if (!selection){ 
    row=1;
  }
  else {
    row=GPOINTER_TO_INT(selection->data);
  }

  switch(option)
    {
    case INSERT:
      gtk_clist_insert(pot_clist,row,(gchar**)entries);
      /*For type 12-17 create a clist to hold "step" namelist (couplings)*/
      if (atoi(type)>=12 && atoi(type)<=17){
	step_clist=gtk_clist_new(COLUMNS_STEP);
	/*Attach the "step_clist" to this row of pot_clist*/
	gtk_clist_set_row_data(GTK_CLIST(pot_clist),row,step_clist);
      }
      break;
      
    case REPLACE:
      if (!selection) {
	glade_util_show_message_box("No state selected!");
	return;
      }
      for(icol=0;icol<COLUMNS_POT;icol++){
	gtk_clist_set_text(pot_clist,row,icol,entries[icol]);
      }
      if (atoi(type)>=12 && atoi(type)<=17){
	if (gtk_clist_get_row_data(pot_clist,row)==NULL){
	  GtkWidget *spin_ib=lookup_widget(main_window,"spin_ib");
	  GtkWidget *spin_ia=lookup_widget(main_window,"spin_ia");
	  GtkWidget *data_clist=gtk_clist_new(COLUMNS_STEP);

	  /*Attach the "data_clist" to this row of pot_clist*/
	  gtk_clist_set_row_data(GTK_CLIST(pot_clist),\
				 row,data_clist);
	  gtk_entry_set_text(GTK_ENTRY(spin_ib),"1");
	  gtk_entry_set_text(GTK_ENTRY(spin_ia),"1");
	}
	gtk_clist_select_row(GTK_CLIST(pot_clist),row,0);
      }
      break;
      
    case DELETE:
      if (!selection) return;
      gtk_clist_remove(pot_clist,row);
      gtk_clist_select_row(pot_clist,row==0?0:row-1,0);
      /* g_print("on pot_clist selecting row=%i",row==0?0:row-1);*/
      break;
      
    case ADD:
      gtk_clist_append(pot_clist,(gchar**)entries);
      
      /*For type 12-17 create a clist to hold "step" namelist (couplings)*/
      if (atoi(type)>=12 && atoi(type)<=17){
	GtkWidget *spin_ib=lookup_widget(main_window,"spin_ib");
	GtkWidget *spin_ia=lookup_widget(main_window,"spin_ia");
	step_clist=gtk_clist_new(COLUMNS_STEP);
	/*Attach the "step_clist" to this row of pot_clist*/
	gtk_clist_set_row_data(GTK_CLIST(pot_clist),\
			     (pot_clist->rows)-1,step_clist);
	gtk_clist_select_row(GTK_CLIST(pot_clist),(pot_clist->rows)-1,0);
	gtk_entry_set_text(GTK_ENTRY(spin_ib),"1");
	gtk_entry_set_text(GTK_ENTRY(spin_ia),"1");
      }
      break;
    
    }

}


void
on_step_clist_select_row               (GtkCList        *clist,
				    gint             row,
				    gint             column,
				    GdkEvent        *event,
				    gpointer         user_data)
{
  gint pot_row=0,kp;

  GtkWidget *step_ib=lookup_widget(GTK_WIDGET(clist),"spin_ib");
  GtkWidget *step_ia=lookup_widget(GTK_WIDGET(clist),"spin_ia");
  GtkWidget *step_k=lookup_widget(GTK_WIDGET(clist),"spin_k");
  GtkWidget *step_str=lookup_widget(GTK_WIDGET(clist),"entry_str");
  

  GtkWidget *pot_clist=lookup_widget(GTK_WIDGET(clist),"pot_clist");
  
  gchar *text;
  
  enum{IB,IBDESC,IA,IADESC,K,STR};
  enum {
    JP,COPYP,BANDP,EP,KKP,TP,CPOT,
    JT,COPYT,BANDT,ET,KTT,TT,FEXCH,IGNORE
  };
  
  
  /*Row selected in pot_clist*/
  GList *sel_pot=GTK_CLIST(pot_clist)->selection;
  pot_row=GPOINTER_TO_INT(sel_pot->data);

  /*Value of KP is in column 0*/
  gtk_clist_get_text(GTK_CLIST(pot_clist),pot_row,0,&text);
  kp=atoi(text);
  
  /*state IB*/
  gtk_clist_get_text(clist,row,IB,&text);
  gtk_entry_set_text(GTK_ENTRY(step_ib),text);

  /*state IA*/
  gtk_clist_get_text(clist,row,IA,&text);
  gtk_entry_set_text(GTK_ENTRY(step_ia),text);

  /*Multipolarity k*/
  gtk_clist_get_text(clist,row,K,&text);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(step_k),atoi(text));
  
  /*Strength STR*/
  gtk_clist_get_text(clist,row,STR,&text);
  gtk_entry_set_text(GTK_ENTRY(step_str),text);

}



void
on_button_step_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  gint potrow;
  gint row,icol;
  GtkWidget *step_ib=lookup_widget(GTK_WIDGET(button),"spin_ib");
  GtkWidget *step_ia=lookup_widget(GTK_WIDGET(button),"spin_ia");
  GtkWidget *step_k=lookup_widget(GTK_WIDGET(button),"spin_k");
  GtkWidget *step_str=lookup_widget(GTK_WIDGET(button),"entry_str");
  GtkWidget *ibdesc=lookup_widget(GTK_WIDGET(button),"entry_ib_desc");
  GtkWidget *iadesc=lookup_widget(GTK_WIDGET(button),"entry_ia_desc");
  

  GtkCList *step_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"step_clist"));
  GtkCList *pot_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"pot_clist"));
  GtkCList *data_clist;

  enum{IB,IBDESC,IA,IADESC,K,STR};
 
  const gchar *entries[]={
    gtk_entry_get_text(GTK_ENTRY (step_ib)),
    gtk_entry_get_text(GTK_ENTRY (ibdesc)),
    gtk_entry_get_text(GTK_ENTRY(step_ia)),
    gtk_entry_get_text(GTK_ENTRY (iadesc)),
    gtk_entry_get_text(GTK_ENTRY(step_k)),
    gtk_entry_get_text(GTK_ENTRY(step_str))};
    
  /*Identify selected button*/
  gint option =GPOINTER_TO_INT(user_data);

  /*Get the data of the selected row*/
  GList *selection=step_clist->selection;

  /*Selection on potential clist*/
  GList *listpot=pot_clist->selection;
  
  /*Checks */
  g_return_if_fail(step_ib!=NULL);
  g_return_if_fail(step_clist!=NULL);
  g_return_if_fail(GTK_IS_CLIST(step_clist));
  
  /*Row selected on step clist*/
  if (!selection){ 
    row=0;
  }
  else {
    row=GPOINTER_TO_INT(selection->data);
  }

  /*Idem for potential clist*/
  potrow=GPOINTER_TO_INT(listpot->data);

  /*Recover the data kept in the row of the pot_clist*/
  data_clist=GTK_CLIST(gtk_clist_get_row_data(pot_clist,potrow));
  g_return_if_fail(data_clist!=NULL);
  g_return_if_fail(GTK_IS_CLIST(data_clist));

  /*  for(i=0;i<4;i++){
    g_print("Entrie(%i)=<%s>\n",i,entries[i]);}
    */
  switch(option)
    {
    case INSERT:
      gtk_clist_insert(step_clist,row,(gchar**)entries);
      gtk_clist_select_row(step_clist,row,0);
      break;
      
    case REPLACE:
      if (!selection) {
	  glade_util_show_message_box("No state selected!");
	  return;
	}
      for(icol=0;icol<COLUMNS_STEP;icol++){
	gtk_clist_set_text(step_clist,row,icol,entries[icol]);
      }
      break;
      
    case DELETE:
      if (!selection) return;
      gtk_clist_remove(step_clist,row);
      gtk_clist_select_row(step_clist,row-1,0);
      break;
      
    case ADD:
      gtk_clist_append(step_clist,(gchar**)entries);
      gtk_clist_select_row(step_clist,step_clist->rows,0);
      break;
    }
  
  /*Dump the content of step_clist to the list attached to the gpointer data
   *of the selected row in pot_clist,that is,the clist holding the potentials
   */
  xfr_clist_to_clist(GTK_CLIST(step_clist),GTK_CLIST(data_clist));

}


/*************************************************************************/
/*                                 OVERLAP                               */
/*************************************************************************/
void
on_overlap_clist_select_row            (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  /*There are a lot of widgets on this page of notebook, so I change 
   *the method to
   *fill them using less code*/

  gint i;

  /*Columns in clist "overlap_clist"*/
  enum{KN1,KN2,IC1,IC2,IN,KIND,CH1,NN,L,
	 LMAX,SN,IA,J,IB,KBPOT,KRPOT,BE,
	 ISC,IPC,NFL,NAM,AMPL,DM,NK,ER};
  
  gint columns=clist->columns;

  /*GtkWidget *window_bins=lookup_widget(main_window,"window_bins");*/
  gchar *be=xfr_clist_get_text(clist,row,BE);
  GtkWidget *over_isc_bound=lookup_widget(main_window,"over_isc");
  /*  GtkWidget *over_isc_cont=lookup_widget(window_bins,"over_isc");*/
   GtkWidget *over_isc_cont=lookup_widget(main_window,"over_isc_cont");

  GtkWidget *widget_isc=atof(be)>0? over_isc_bound:over_isc_cont;

  GtkWidget *over[]={lookup_widget(GTK_WIDGET(clist),"over_kn1"),
		     lookup_widget(GTK_WIDGET(clist),"over_kn2"),
		     lookup_widget(GTK_WIDGET(clist),"combo_ic1"),
		     lookup_widget(GTK_WIDGET(clist),"combo_ic2"),
		     lookup_widget(GTK_WIDGET(clist),"over_in"),
		     lookup_widget(GTK_WIDGET(clist),"over_kind"),
		     lookup_widget(GTK_WIDGET(clist),"over_ch1"),
		     lookup_widget(GTK_WIDGET(clist),"over_nn"),
		     lookup_widget(GTK_WIDGET(clist),"over_l"),
		     lookup_widget(GTK_WIDGET(clist),"over_lmax"),
		     lookup_widget(GTK_WIDGET(clist),"over_sn"),
		     lookup_widget(GTK_WIDGET(clist),"over_ia"),
		     lookup_widget(GTK_WIDGET(clist),"over_j"),
		     lookup_widget(GTK_WIDGET(clist),"over_ib"),
		     lookup_widget(GTK_WIDGET(clist),"over_kbpot"),
		     lookup_widget(GTK_WIDGET(clist),"over_krpot"),
		     lookup_widget(GTK_WIDGET(clist),"over_be"),
		     widget_isc,
		     lookup_widget(GTK_WIDGET(clist),"over_ipc"),
		     lookup_widget(GTK_WIDGET(clist),"over_nfl"),
		     lookup_widget(GTK_WIDGET(clist),"over_nam"),
		     lookup_widget(GTK_WIDGET(clist),"over_ampl"),
		     lookup_widget(GTK_WIDGET(clist),"over_dm"),
		     lookup_widget(GTK_WIDGET(clist),"over_nk"),
		     lookup_widget(GTK_WIDGET(clist),"over_er")};
  
  
  GtkWidget *opt_nfl=lookup_widget(GTK_WIDGET(clist),"opt_nfl");
  GtkWidget *over_in=lookup_widget(GTK_WIDGET(clist),"over_in");
  GtkWidget *check_vary_be=lookup_widget(GTK_WIDGET(clist),"check_vary_be");
  /*GtkWidget *over_kind=lookup_widget(GTK_WIDGET(clist),"over_kind");*/
  GtkWidget *over_ipc=lookup_widget(GTK_WIDGET(clist),"over_ipc");
    GtkWidget *check_vforbins=lookup_widget(GTK_WIDGET(clist),"check_vforbins");


  gchar *cell[columns];
  
  /*I read the text on the columns of the clist and put them on their
   *corresponding widgets, when sensible. CHAPUZA !!*/
  for (i=0;i<columns;i++){
    gtk_clist_get_text(clist,row,i,&cell[i]);

    if (GTK_IS_ENTRY(over[i]) && over[i]!=NULL){
      if (i==NFL){
	gchar *buffer=g_strdup_printf("%i",abs(atoi(cell[i])));
	gtk_entry_set_text(GTK_ENTRY(over[i]),buffer);
        
      }
      else{    
	gtk_entry_set_text(GTK_ENTRY(over[i]),cell[i]);
     /* g_print("Pongo texto %s en entry:%s\n",cell[i],over[i]->name);*/
      }
    }

/* 11/04/08 */
     if (GTK_IS_ENTRY(over[i]) && over[i]!=NULL){
      if (i==ISC){
	gchar *buffer=g_strdup_printf("%i",abs(atoi(cell[i])));
	gtk_entry_set_text(GTK_ENTRY(over[i]),buffer);
       /* g_print("i=%i, BE, text=%s\n",i,buffer);*/
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_vforbins),
                               atoi(cell[i])<0?TRUE:FALSE);

      }
      else{    
	gtk_entry_set_text(GTK_ENTRY(over[i]),cell[i]);
     /* g_print("Pongo texto %s en entry:%s\n",cell[i],over[i]->name);*/
      }
    }


  }


  /******************* Fill other widgets ****************/

  

  /*Combo's ic1 & ic2*/
  xfr_combo_set_value(GTK_WIDGET(clist),"combo_ic1",atoi(cell[IC1])-1);
  xfr_combo_set_value(GTK_WIDGET(clist),"combo_ic2",atoi(cell[IC2])-1);
  /*  g_print("En select row, cell[ic1]=%i",atoi(cell[IC1]));*/

  /* Option menu "opt_nfl"
   * nfl<0: Write wf to nfl file
   * nfl>0: Read wf from nfl file
   */
  if (atoi(cell[NFL])<0) i=0; 
  else i=1;                  
  gtk_option_menu_set_history(GTK_OPTION_MENU(opt_nfl),i);
  

  /* Option menu  "over_in"
   * in=1->option=0=projectile
   * in=2->option=1=target
   */
  if (abs(atoi(cell[IN]))==1) i=0;
  else i=1;
  gtk_option_menu_set_history(GTK_OPTION_MENU(over_in),i);


  /*Check-button"check_vary_be"
   *be<0: Vary binding energy. ISC from window bins
   *be>0: Vary ISC component of potential KBPOT
   */  
  if (atof(be)>0){
    gtk_widget_set_sensitive(over_isc_bound,TRUE);
    gtk_widget_set_sensitive(over_isc_cont,FALSE);
    if (atoi(cell[ISC])==0) xfr_toggle_set_value(check_vary_be,"TRUE");
  /*  g_print("ISC=%i",atoi(cell[ISC])); */
  }else{
    gtk_widget_set_sensitive(over_isc_bound,FALSE);
    gtk_widget_set_sensitive(over_isc_cont,TRUE);
  }

  /*
  if (atof(cell[ISC])>0){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_vary_be),TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(over[ISC]),FALSE);
  }
  else{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_vary_be),FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(over[ISC]),TRUE);
  }
  */


  /*Combo "overkind" */
  xfr_combo_set_value(GTK_WIDGET(clist),"over_kind",atoi(cell[KIND]));				       
  /*Option menu "ipc" */
  gtk_option_menu_set_history(GTK_OPTION_MENU(over_ipc),atoi(cell[IPC]));
  
}

void
on_check_vary_be_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  guint isc;
  GtkWidget *over_isc=lookup_widget(GTK_WIDGET(button),"over_isc");
  if (GTK_TOGGLE_BUTTON(button)->active) isc=FALSE;
  else isc=TRUE;

  gtk_widget_set_sensitive(GTK_WIDGET(over_isc),isc);
}


void
on_button_overlap_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gint i,option,icol,row;
  GList *selection;

  /*Get overlap_clist */
  GtkCList *clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),\
				       "overlap_clist"));
 
  /*Columns order in "overlap_clist"*/  
  enum{KN1,KN2,IC1,IC2,IN,KIND,CH1,NN,L,LMAX,SN,\
	 IA,J,IB,KBPOT,KRPOT,BE,ISC,IPC,
	 NFL,NAM,AMPL,DM,NK,ER};
 
  /*Number of columns*/
  gint columns=clist->columns;
  /* GtkWidget *window_bins=lookup_widget(GTK_WIDGET(button),"window_bins");*/
  
  /* For continuum states isc is in window_bins <= deprecated */
  GtkWidget *over_be=lookup_widget(GTK_WIDGET(clist),"over_be");
  const gchar *be=gtk_entry_get_text(GTK_ENTRY(over_be));
  /*  GtkWidget *widget_isc=\
      lookup_widget(atof(be)>0? main_window:window_bins,"over_isc");*/
  GtkWidget *widget_isc=\
    lookup_widget(main_window,atof(be)>0? "over_isc":"over_isc_cont");

  GtkWidget *over[]={lookup_widget(GTK_WIDGET(clist),"over_kn1"),
		     lookup_widget(GTK_WIDGET(clist),"over_kn2"),
		     lookup_widget(GTK_WIDGET(clist),"combo_ic1"),
		     lookup_widget(GTK_WIDGET(clist),"combo_ic2"),
		     lookup_widget(GTK_WIDGET(clist),"over_in"),
		     lookup_widget(GTK_WIDGET(clist),"over_kind"),
		     lookup_widget(GTK_WIDGET(clist),"over_ch1"),
		     lookup_widget(GTK_WIDGET(clist),"over_nn"),
		     lookup_widget(GTK_WIDGET(clist),"over_l"),
		     lookup_widget(GTK_WIDGET(clist),"over_lmax"),
		     lookup_widget(GTK_WIDGET(clist),"over_sn"),
		     lookup_widget(GTK_WIDGET(clist),"over_ia"),
		     lookup_widget(GTK_WIDGET(clist),"over_j"),
		     lookup_widget(GTK_WIDGET(clist),"over_ib"),
		     lookup_widget(GTK_WIDGET(clist),"over_kbpot"),
		     lookup_widget(GTK_WIDGET(clist),"over_krpot"),
		     over_be,
		     widget_isc,
		     lookup_widget(GTK_WIDGET(clist),"over_ipc"),
		     lookup_widget(GTK_WIDGET(clist),"over_nfl"),
		     lookup_widget(GTK_WIDGET(clist),"over_nam"),
		     lookup_widget(GTK_WIDGET(clist),"over_ampl"),
		     lookup_widget(GTK_WIDGET(clist),"over_dm"),
		     lookup_widget(GTK_WIDGET(clist),"over_nk"),
		     lookup_widget(GTK_WIDGET(clist),"over_er")};
  
  
  /*  GtkWidget *opt_nfl=lookup_widget(GTK_WIDGET(clist),"opt_nfl");*/
  GtkWidget *check_vary_be=lookup_widget(GTK_WIDGET(clist),"check_vary_be");
  GtkWidget *check_vforbins=lookup_widget(GTK_WIDGET(clist),"check_vforbins");
  GtkWidget *over_kind=lookup_widget(GTK_WIDGET(clist),"over_kind");
  
  gchar *cell[columns];

  /*if (window_bins == NULL){
    window_bins = create_window_bins ();
    gtk_object_set_data(GTK_OBJECT(main_window),"window_bins",window_bins);
    }*/

  
  /* I read the text from the widgets and put them on cell[] array  CHAPUZA !!
   * This is valid for entries and spinbuttons
   */

  
  for (i=0;i<columns;i++){
    /*g_print("Overlap: i=%i...",i);*/
    if (GTK_IS_ENTRY(over[i]) && over[i]!=NULL){
      const gchar *buffer=gtk_entry_get_text(GTK_ENTRY(over[i]));
      /* g_print("...buffer ok...");*/
      switch (i){
      case NFL:
	if (xfr_option_menu_get_value(main_window,"opt_nfl")==0)
	  cell[NFL]=g_strdup_printf("-%s",buffer); /*nfl negative*/
	else
	  cell[NFL]=g_strdup(buffer);
	break;
     
      case ISC:
	if (GTK_TOGGLE_BUTTON(check_vary_be)->active && atof(be)>0){          
	 cell[ISC]=g_strdup("0"); 
	}
	else{
	  if (GTK_TOGGLE_BUTTON(check_vforbins)->active && atof(be)>0)
 	    cell[ISC]=g_strdup_printf("-%s",buffer); /*ISC negative*/
          else
            cell[ISC]=g_strdup(buffer);
       }
	break;
	
      default:
	cell[i]=g_strdup(buffer);
	break;
      }
    }
    /*g_print("...ok\n");*/
  }

  /*IC1, IC2*/
  cell[IC1]=g_strndup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(over[IC1])->entry)),2);
  cell[IC2]=g_strndup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(over[IC2])->entry)),2);


  /* Option menu  "over_in"
   * in=1->option=0=projectile
   * in=2->option=1=target
   */
  if (xfr_option_menu_get_value(main_window,"over_in")==0) 
    cell[IN]=g_strdup("1");
  else 
    cell[IN]=g_strdup("2");

  /*Combo "over_kind" */    
   if (GTK_COMBO(over_kind)->entry !=NULL){
     const gchar *buffer=gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(over_kind)->entry));
     cell[KIND]=g_strndup(buffer,1);
   }

  /*Option menu "ipc" */
  cell[IPC]=g_strdup_printf("%i",xfr_option_menu_get_value(main_window,"over_ipc"));  
  
  /*Identify selected button*/
  option =GPOINTER_TO_INT(user_data);

  /*Get the data of the selected row*/
  selection=clist->selection;

  /*Checks */
  g_return_if_fail(clist!=NULL);
  g_return_if_fail(GTK_IS_CLIST(clist));
  
  /*Row selected on clist*/
  if (!selection){ 
    g_print("selection empty en clist\n");
    row=1;
  }
  else {
    row=GPOINTER_TO_INT(selection->data);
  }

  switch(option)
    {
    case INSERT:
      gtk_clist_insert(clist,row,cell);
      gtk_clist_select_row(clist,row,0);
      break;
      
    case REPLACE:
      if (!selection) 
	{
	  glade_util_show_message_box("No state selected!");
	  return;
	}
      for(icol=0;icol<COLUMNS_OVERLAP;icol++)
	{
	  gtk_clist_set_text(clist,row,icol,cell[icol]);
	}
      gtk_clist_select_row(clist,row,0);
      break;
      
    case DELETE:
      if (!selection) return;
      gtk_clist_remove(clist,row);
      gtk_clist_select_row(clist,row-1,0);
      break;
      
    case ADD:
      gtk_clist_append(clist,cell);
      gtk_clist_select_row(clist,clist->rows,0);
      break;
    }
}





/****************************************************************
 *                          COUPLINGS                           *
 ****************************************************************/
void
on_coup_clist_select_row               (GtkCList        *clist,
					gint             row,
					gint             column,
					GdkEvent        *event,
					gpointer         user_data)
{
  gint i;
  gint iopt=0;
  GtkWidget *check_icto=lookup_widget(GTK_WIDGET(clist),"check_icto");
  GtkWidget *entry_rmax=lookup_widget(GTK_WIDGET(clist),"entry_rmax");
  GtkWidget *entry_jmax=lookup_widget(GTK_WIDGET(clist),"entry_jmax");
  GtkWidget *check_k8_ip2=lookup_widget(GTK_WIDGET(clist),"check_k8_ip2");
  GtkWidget *check_k7_ip1=lookup_widget(GTK_WIDGET(clist),"check_k7_ip1");
  GtkWidget *entry_k7_ip3=lookup_widget(GTK_WIDGET(clist),"entry_k7_ip3");
  GtkWidget *entry_k5_p1=lookup_widget(GTK_WIDGET(clist),"entry_k5_p1");
  GtkWidget *spin_q=lookup_widget(GTK_WIDGET(clist),"spin_q");
  GtkWidget *entry_k3_p1=lookup_widget(GTK_WIDGET(clist),"entry_k3_p1");
  GtkWidget *entry_k3_p2=lookup_widget(GTK_WIDGET(clist),"entry_k3_p2");
  /* GtkWidget *opt_k3_ip2=lookup_widget(GTK_WIDGET(clist),"opt_k3_ip2");*/
  GtkWidget *combo_k3_ip2=lookup_widget(GTK_WIDGET(clist),"combo_k3_ip2");
  GtkWidget *opt_k3_ip3=lookup_widget(GTK_WIDGET(clist),"opt_k3_ip3");
  GtkWidget *entry_k5_p2=lookup_widget(GTK_WIDGET(clist),"entry_k5_p2");
  GtkWidget *opt_k5_p2=lookup_widget(GTK_WIDGET(clist),"opt_k5_p2");
  GtkWidget *opt_k7_ip1=lookup_widget(GTK_WIDGET(clist),"opt_k7_ip1");
  GtkWidget *opt_k7_ip2=lookup_widget(GTK_WIDGET(clist),"opt_k7_ip2");
  GtkWidget *opt_k8_ip1=lookup_widget(GTK_WIDGET(clist),"opt_k8_ip1");
  GtkWidget *opt_k9_ip2=lookup_widget(GTK_WIDGET(clist),"opt_k9_ip2");
  GtkWidget *opt_k9_ip1=lookup_widget(GTK_WIDGET(clist),"opt_k9_ip1");
  GtkWidget *opt_k9_ip3=lookup_widget(GTK_WIDGET(clist),"opt_k9_ip3");
  GtkWidget *entry_k9_p1=lookup_widget(GTK_WIDGET(clist),"entry_k9_p1");
  GtkWidget *entry_k9_p2=lookup_widget(GTK_WIDGET(clist),"entry_k9_p2");
  GtkWidget *opt_q=lookup_widget(GTK_WIDGET(clist),"opt_q");
  GtkNotebook *nb_coup=GTK_NOTEBOOK(lookup_widget(GTK_WIDGET(clist),"nb_coup"));
  GtkNotebook *nb_cfp=GTK_NOTEBOOK(lookup_widget(GTK_WIDGET(clist),"nb_cfp"));
  GtkWidget *frame_cfp=lookup_widget(GTK_WIDGET(clist),"frame_cfp");
  GtkCList *data_clist=GTK_CLIST(gtk_clist_get_row_data(clist,row));
  GtkCList *inel_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(clist),"inel_clist"));
  GtkCList *cfp_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(clist),"cfp_clist"));
  GtkWidget *table_qscale=GTK_WIDGET(lookup_widget(GTK_WIDGET(clist),"table_qscale"));  /* NEW */

  
  enum{ICTO,ICTFROM,KIND,IP1,IP2,IP3,P1,P2,RMAX,JMAX};
  gchar *text[COLUMNS_COUP];
  gint kind,icto,ictfrom;
  
  xfr_print("coup_clist selected!!!\n");
  /*Read the elements of the selected row*/
  for (i=0;i<COLUMNS_COUP;i++){
    gtk_clist_get_text(GTK_CLIST(clist),row,i,&text[i]);
  }
  kind=atoi(text[KIND]);
  
  /*if icto<0  coupling in reverse direction is not included*/
  if ((icto=atoi(text[ICTO]))<0)
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_icto),TRUE);
  else
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_icto),FALSE);
  
  xfr_combo_set_value(GTK_WIDGET(clist),"combo_icto",abs(icto)-1);
  ictfrom=atoi(text[ICTFROM]);
  xfr_combo_set_value(GTK_WIDGET(clist),"combo_ictfrom",abs(ictfrom)-1);
  xfr_combo_set_value(GTK_WIDGET(clist),"combo_kind",kind-1);
  gtk_entry_set_text(GTK_ENTRY(entry_rmax),text[RMAX]);
  gtk_entry_set_text(GTK_ENTRY(entry_jmax),text[JMAX]);
  
  
  /* Fill the widgets on nb_coup with the values read from coup_clist*/
  switch(kind){
  case 3:case 4: /* Single-particle excitations */
    /*Show notebook "nb_coup" and select page 0*/
    gtk_widget_show(GTK_WIDGET(nb_coup));
   /*  gtk_clist_freeze(GTK_CLIST(inel_clist)); */
/*     gtk_clist_thaw(GTK_CLIST(cfp_clist)); */

    gtk_notebook_set_page(nb_coup,0);  
    
    

    /*Q multipoles*/
    gtk_entry_set_text(GTK_ENTRY(spin_q),\
		       g_strdup_printf("%i",abs(atoi(text[IP1]))));
    if (atoi(text[IP1])<0){
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_q),0);
    }
    else{
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_q),1);
    }
    
    /*IP2: Widget "opt_k3_ip2" (Coulomb/Nuclear)*/
    iopt=atoi(text[IP2]);
    /*gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip2),iopt);*/
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_k3_ip2),iopt);


    /*IP3: Widget "opt_k3_ip3" (Reorientation terms)*/
    switch(atoi(text[IP3])){
    case 0: 
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip3),0);
      break;
    case 1: 
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip3),1);
      break;
    case 2:
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip3),2);
      break; 
    case 3:
       gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip3),3);
      break; 
    case 4:
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip3),4);
      break; 
    case 5: 
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip3),5);
      break;
    case 10: case 11: case 12: case 13: case 14: case 15:
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k3_ip3),atoi(text[IP3])-4);
      break; 
    }

    
    /*P1=pot KP for frag.-targ. interact.: Widget "entry_k3_p1"*/
    gtk_entry_set_text(GTK_ENTRY(entry_k3_p1),text[P1]);
    
    /*P2=pot KP for core.-targ. interact. Widget "entry_k3_p2" */
    gtk_entry_set_text(GTK_ENTRY(entry_k3_p2),text[P2]);
    
    break;
    
  case 5: 
    /*Select page 1 on notebook "nb_coup"*/
    gtk_widget_show(GTK_WIDGET(nb_coup));
    gtk_notebook_set_page(nb_coup,1);
    
    /*IP1 ,IP2 not used*/
    
    /*P1=Do=ZR coupling constant. Widget:entry_k5_p1*/
    gtk_entry_set_text(GTK_ENTRY(entry_k5_p1),text[P1]);
    
    /*P2=FNRNG Efective finite-range param. Widget:entry_k5_p2*/
    gtk_entry_set_text(GTK_ENTRY(entry_k5_p2),text[P2]);
    
    break;
    
  case 6:
  /*Select page 1 on notebook "nb_coup"*/
  gtk_widget_show(GTK_WIDGET(nb_coup));
  gtk_notebook_set_page(nb_coup,1);
  gtk_widget_set_sensitive(GTK_WIDGET(frame_cfp),TRUE);
  
  /*IP1 ,IP2 not used*/
  
  /*D_o from proj. bound states */
  
  /*If IP3=1 FNRNG is read here*/
  if (atoi(text[IP3])==1){
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k5_p2),1);
  }
  else{
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k5_p2),0);
  }
  
  break;
  
  case 7: /*Finite range transfers*/
    /*Select page 2 on notebook "nb_coup"*/
    gtk_widget_show(GTK_WIDGET(nb_coup));
    gtk_notebook_set_page(nb_coup,2);
    
    /*IP1: Post/prior*/
    i=atoi(text[IP1]);
    switch(i){
    case 0: case -2:
      iopt=0; /*Post interaction*/
      break;
    case 1: case -1:
      iopt=1; /*Prior interaction*/
      break;
    default:
      if (i<-3){
	iopt=2;/*VCORE interaction*/
      }
      else{
	g_print("Wrong value of IP1 in kind=7 coupling!:IP1=%i",i);
      }
      break;
    }
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k7_ip1),iopt);

    /*IP1 (continued). Theta quadrature*/
    if (i<=-1) 
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_k7_ip1),TRUE);

    /*IP2: Remnant*/
    switch(atoi(text[IP2])){
    case 0: /*No remnant*/
      iopt=0;break;
    case 1:/*Full real*/
      iopt=1;break;
    case -1:/*Full complex*/
      iopt=2;break;
    case 2: /*Non-orthogonality remnant*/
      iopt=3;break;
    default:
      g_print("\nWrong IP2 value on coupling namelist %s",text[IP2]);
      break;
    }
    
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k7_ip2),iopt);

    /*IP3=KPCORE*/
    i=atoi(text[IP3]);
    if(i!=0){
      gtk_entry_set_text(GTK_ENTRY(entry_k7_ip3),text[IP3]);
    }
    else{
      /*GtkCList *part_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(clist),"part_clist"));*/
      /*GtkCList *states_clist=GTK_CLIST(gtk_clist_row_get_data(...));*/
      /*Use O.P. for the first pair of excited states in the partition of
       *projectile core
       *AQUI FALTA CODIGO..........
       */
    }
    break;
    
  case 8:
    gtk_widget_show(GTK_WIDGET(nb_coup));
    gtk_notebook_set_page(nb_coup,3);
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k8_ip1),
				abs(atoi(text[IP1])));
    /*IP1: Post/prior*/
    if (atoi(text[IP2])==0){
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_k8_ip2),FALSE);
      gtk_widget_set_sensitive(GTK_WIDGET(frame_cfp),FALSE);      
    }
    else{
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_k8_ip2),TRUE);
      gtk_notebook_set_page(GTK_NOTEBOOK(nb_cfp),1);
      gtk_widget_set_sensitive(GTK_WIDGET(frame_cfp),TRUE);
    }
    break;
    
  case 9: /*General spin transfer*/
    /*Select page 3 on notebook "nb_coup"*/
    gtk_widget_show(GTK_WIDGET(nb_coup));
    gtk_notebook_set_page(nb_coup,4);
    
    /*IP1 (local/non-local)*/
    if(atoi(text[IP1])==0) 
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k9_ip1),0);
    else
      gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k9_ip1),1);
    
    /*IP2 (Read real/imaginary/complex)*/
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k9_ip2),atoi(text[IP2]));
    
    /*IP3 ??*/

    switch (atoi(text[IP3])){
    case 0:
      iopt=0;break;
    case 1:
      iopt=1;break;
    case 2:
      iopt=2;break;
    case 3:
      iopt=3;break;
    case -1:
      iopt=4;break;
    case -2:
      iopt=5;break;
    case -3:
      iopt=6;break;
    default:
      g_print("\nWrong value for ip3 on kind 9 coupling. Ignored...");
      break;
    }
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_k9_ip3),iopt);
    gtk_entry_set_text(GTK_ENTRY(entry_k9_p1),text[P1]);
    gtk_entry_set_text(GTK_ENTRY(entry_k9_p2),text[P2]);
    
    
    break;    
    
  default: /*Kinds 1,2*/
    /*g_print("\nRow with kind=1 o 2 selected");*/
    gtk_widget_hide(GTK_WIDGET(nb_coup));
    break;
  }
  
 


  /* Dump content of qscale_clist to qscale_table */
    qscale_clist_to_table(GTK_CLIST(clist),row); 

 
  /* For kind=3-7 and 8 (if ip2>0) show cfp_clist 
   * For kind=1,2 show inel_clist
   * The inel and cfp information is attached to the row data pointer
   */ 
  data_clist=GTK_CLIST(gtk_clist_get_row_data(clist,row));
  g_return_if_fail(data_clist!=NULL);
  g_return_if_fail(GTK_IS_CLIST(data_clist));

  switch(kind){
  case 1:case 2:
    if (data_clist){
      gtk_notebook_set_page(nb_cfp,0);
      gtk_widget_set_sensitive(GTK_WIDGET(frame_cfp),TRUE);
      /*Copy the content of data_clist to inel_clist*/ 
      xfr_clist_to_clist(data_clist,inel_clist);
      if (GTK_CLIST(inel_clist)->rows>0)
	  gtk_clist_select_row(GTK_CLIST(inel_clist),0,0);
    }
    break;
    
  case 3: case 4:case 5: case 6: case 7:
    gtk_notebook_set_page(nb_cfp,1);
    gtk_widget_set_sensitive(GTK_WIDGET(frame_cfp),TRUE);
    if (data_clist){
      /*Copy the content of data_clist to cfp_clist*/ 
      xfr_clist_to_clist(data_clist,cfp_clist); 
      if (GTK_CLIST(cfp_clist)->rows>0)
	  gtk_clist_select_row(GTK_CLIST(cfp_clist),0,0);
    }
    break;
    
  case 8: /*If IP2>0 we have also a cfp_clist attached to this row*/
    gtk_notebook_set_page(nb_cfp,1); 
    if (atoi(text[IP2])!=0)
      gtk_widget_set_sensitive(GTK_WIDGET(frame_cfp),TRUE);

    if (data_clist!=NULL)
       xfr_clist_to_clist(data_clist,cfp_clist);  
    if (GTK_CLIST(cfp_clist)->rows>0)
	gtk_clist_select_row(GTK_CLIST(cfp_clist),0,0);
    break;
  }
}




/* Freeze text in qscale table */
/* NOT USED !!!!!!!! */
/* void */
/* xfr_qscale_table_freeze(GtkWidget *table){ */
/*   gint iq; */
/*   for(iq=0;iq<=QSCALEDIM;iq++){ */
/*     gint col=2*iq; */
/*     gchar *bufr=g_strdup_printf("q%i_nr",iq); */
/*     gchar *bufi=g_strdup_printf("q%i_ni",iq); */
/*     GtkWidget *entry1=lookup_widget(GTK_WIDGET(table),bufr); /\* table cell *\/ */
/*     GtkWidget *entry2= lookup_widget(GTK_WIDGET(table),bufi); /\* table cell *\/ */
/*     gtk_text_freeze( GTK_TEXT(entry1)); */
/*     gtk_text_freeze( GTK_TEXT(entry2)); */
/*   } */
/* } */



void
qscale_clist_to_table(GtkCList *coup_clist,gint coup_row){
  gint iq;
  for(iq=0;iq<=QSCALEDIM;iq++){
    gint col=2*iq;
    gchar *bufr=g_strdup_printf("q%i_nr",iq);
    gchar *bufi=g_strdup_printf("q%i_ni",iq);
    GtkWidget *entry1=lookup_widget(GTK_WIDGET(coup_clist),bufr); /* table cell */
    GtkWidget *entry2=lookup_widget(GTK_WIDGET(coup_clist),bufi); /* table cell */
    
    g_assert(GTK_IS_CLIST(qscale_clist));
    
    if (qscale_clist==NULL) g_print("qscale_clist is NULL!\n");
    
    if (entry1 && entry2 && qscale_clist){
      GtkWidget *entry=lookup_widget(GTK_WIDGET(coup_clist),bufr); /* table cell */
      /* Real part */
      gchar *text=xfr_clist_get_text(GTK_CLIST(qscale_clist),coup_row,2*iq);
      gtk_entry_set_text(GTK_ENTRY(entry),text);
      /* Imag part */
      entry=lookup_widget(GTK_WIDGET(coup_clist),bufi); /* table cell */
      text=g_strdup(xfr_clist_get_text(GTK_CLIST(qscale_clist),coup_row,2*iq+1));
      gtk_entry_set_text(GTK_ENTRY(entry),text);
    }
  }
}



void
on_button_coupling_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{  
  GtkWidget *combo_icto=lookup_widget(GTK_WIDGET(button),"combo_icto");
  GtkWidget *combo_ictfrom=lookup_widget(GTK_WIDGET(button),"combo_ictfrom");
  GtkWidget *check_icto=lookup_widget(GTK_WIDGET(button),"check_icto");
  GtkWidget *combo_kind=lookup_widget(GTK_WIDGET(button),"combo_kind");
  GtkWidget *entry_rmax=lookup_widget(GTK_WIDGET(button),"entry_rmax");
  GtkWidget *entry_jmax=lookup_widget(GTK_WIDGET(button),"entry_jmax");
  GtkWidget *check_k8_ip2=lookup_widget(GTK_WIDGET(button),"check_k8_ip2");
  GtkWidget *check_k7_ip1=lookup_widget(GTK_WIDGET(button),"check_k7_ip1");
  GtkWidget *entry_k7_ip3=lookup_widget(GTK_WIDGET(button),"entry_k7_ip3");
  GtkWidget *entry_k5_p1=lookup_widget(GTK_WIDGET(button),"entry_k5_p1");
  GtkWidget *entry_k5_p2=lookup_widget(GTK_WIDGET(button),"entry_k5_p2");
  GtkWidget *spin_q=lookup_widget(GTK_WIDGET(button),"spin_q");
  GtkWidget *entry_k3_p1=lookup_widget(GTK_WIDGET(button),"entry_k3_p1");
  GtkWidget *entry_k3_p2=lookup_widget(GTK_WIDGET(button),"entry_k3_p2");
  GtkWidget *entry_k9_p1=lookup_widget(GTK_WIDGET(button),"entry_k9_p1");
  GtkWidget *entry_k9_p2=lookup_widget(GTK_WIDGET(button),"entry_k9_p2");
  
  /*CList with COUPLING namelist*/
  GtkCList *coup_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"coup_clist"));
  GtkCList *inel_clist,*cfp_clist;

  /*Get the data of the selected row in coup_clist*/
  GList *selection=coup_clist->selection;  

  enum{ICTO,ICTFROM,KIND,IP1,IP2,IP3,P1,P2,RMAX,JMAX};
  gchar *text[COLUMNS_COUP];
  gchar *buffer;
  guint option=GPOINTER_TO_INT(user_data);
  guint ikind;
  gint iopt,iaux,row;
  gint ip3=0;

  for (iaux=0;iaux<COLUMNS_COUP;iaux++){
    text[iaux]=g_strdup("0");
  }

  /*Kind of coupling */ 
  buffer=g_strndup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo_kind)->entry)),1);
  text[KIND]=g_strdup(buffer);
  ikind=atoi(buffer);

  /*Get values in generic widgets (valid for all kinds)*/
  text[ICTO]=g_strndup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo_icto)->entry)),2);

  if (GTK_TOGGLE_BUTTON(check_icto)->active)
    text[ICTO]=g_strdup_printf("-%s",text[ICTO]);

  text[ICTFROM]=g_strndup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo_ictfrom)->entry)),2); 
  text[RMAX]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_rmax)));
  text[JMAX]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_jmax)));

  /*Get values in widgets for the selected kind*/
  switch (ikind){
  case 3:case 4: /* Single-particle excitations */
    /*Q multipoles*/
    text[IP1]=g_strdup(gtk_entry_get_text(GTK_ENTRY(spin_q)));

    iopt=xfr_option_menu_get_value(main_window,"opt_q");
    if (iopt==0)
      text[IP1]=g_strdup_printf("-%s",text[IP1]); /*Change sign to indicate only this mulpl*/    

    /*IP2: Widget "opt_k3_ip2" (Coulomb/Nuclear/Both)*/
    /* iopt=xfr_option_menu_get_value(main_window,"opt_k3_ip2");*/
    /*g_print("iopt=%i",iopt);*/
    iopt=gtk_combo_box_get_active(GTK_COMBO_BOX(lookup_widget(main_window,
							  "combo_k3_ip2")));
 
    text[IP2]=g_strdup_printf("%i",iopt);

    /*IP3: Widget "opt_k3_ip3" (Reorientation terms*/
    ip3=xfr_option_menu_get_value(main_window,"opt_k3_ip3");
    if (ip3>5) ip3=ip3+4;
    text[IP3]=g_strdup_printf("%i",ip3);

    /*P1=pot KP for frag.-targ. interact.: Widget "entry_k3_p1"*/
    text[P1]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k3_p1)));
 
    /*P2=pot KP for core.-targ. interact. Widget "entry_k3_p2" */
    text[P2]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k3_p2)));
    
    break;
    
  case 5:     
    /*IP1 ,IP2 not used*/
    text[IP1]=text[IP2]=g_strdup("0");

    /*P1=Do=ZR coupling constant. Widget:entry_k5_p1*/
    text[P1]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k5_p1)));
    
    /*P2=FNRNG Efective finite-range param. Widget:entry_k5_p2*/
    text[P2]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k5_p2)));
    
    break;
    
  case 6:  
  /*IP1 ,IP2 not used*/
    text[IP1]=text[IP2]=g_strdup("");

  /*D_o from proj. bound states */
  
  /*If IP3=1 FNRNG is read here*/
   iopt=xfr_option_menu_get_value(main_window,"opt_k5_p2");
   if (iopt){
     text[IP3]=g_strdup_printf("%i",1);
     text[P2]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k5_p2)));
   }
  break;
  
  case 7: /*Finite range transfers*/    
    /*P1 & P2 not used*/
    text[P1]=text[P2]=g_strdup("0");
    
    /*IP1: Post/prior*/
    iopt=xfr_option_menu_get_value(main_window,"opt_k7_ip1");
    /*  i=gtk_toggle_button_get_value(GTK_TOGGLE(check_k7_ip1));*/
    
    switch(iopt){
    case 0:
      if (GTK_TOGGLE_BUTTON(check_k7_ip1)->active)
	iaux=-2; /*Post + Theta quadrature*/
      else
	iaux=0; /*Post*/
 
      break;

    case 1:
      iaux=1;break;/*Prior*/

    case 2:
      iaux=-4;break;/*VCORE interaction*/
    }

    text[IP1]=g_strdup_printf("%i",iaux);

    /*IP2: Remnant*/
    iopt=xfr_option_menu_get_value(main_window,"opt_k7_ip2");
   
    iaux=-1;
    switch(iopt){
    case 0: /*No remnant*/
      iaux=0;break;
    case 1:/*Full real*/
      iaux=1;break;
    case 2:/*Full complex*/
      iaux=-1;break;
    case 3: /*Non-orthogonality remnant*/
      iaux=2;break;
    default:
      g_warning("Error in on_button_coupling,case 7");
      break;
    }
    text[IP2]=g_strdup_printf("%i",iaux);

    /*IP3=KPCORE*/
    text[IP3]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k7_ip3)));
 
    break;
    
  case 8:    
    /*IP1: Post/prior*/
    text[IP1]=g_strdup_printf("%i",xfr_option_menu_get_value(main_window,"opt_k8_ip1"));

    /*IP2: Read or not cfp table*/
    if (GTK_TOGGLE_BUTTON(check_k8_ip2)->active) text[IP2]=g_strdup("1");
    else text[IP2]=g_strdup("0");
    break;
    
  case 9: /*General spin transfer*/
    /* P1 & P2 scaling factors */
    text[P1]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k9_p1)));
    text[P2]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_k9_p2)));

    /*IP1 (local/non-local)*/
    text[IP1]=g_strdup_printf("%i",xfr_option_menu_get_value(main_window,"opt_k9_ip1"));
    
    /*IP2 (Read real/imaginary/complex)*/
    text[IP2]=g_strdup(xfr_option_menu_get_char_value(GTK_WIDGET(button),"opt_k9_ip2"));
    
    /*IP3 ??*/
    iopt=xfr_option_menu_get_value(main_window,"opt_k9_ip3");
    switch (iopt){
    case 0:
      iaux=0;break;
    case 1:
      iaux=1;break;
    case 2:
     iaux=2;break;
    case 3:
      iaux=3;break;
    case 4:
      iaux=-1;break;
    case 5:
      iaux=-2;break;
    case 6:
      iaux=-3;break;
    }
    text[IP3]=g_strdup_printf("%i",iaux);
}
    
  
  /*Row selected on step clist*/
  if (!selection){ 
    row=1;
  }
  else {
    row=GPOINTER_TO_INT(selection->data);
  }

  switch(option){
  case DELETE:
    if (!selection) return;
    if (ikind<9 && ikind>2){
      cfp_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(coup_clist),"cfp_clist"));
      g_return_if_fail(cfp_clist!=NULL);
      g_return_if_fail(GTK_IS_CLIST(cfp_clist));
      gtk_clist_clear (cfp_clist);
    }
    if (ikind==1 || ikind==2){
      inel_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(coup_clist),"inel_clist"));
      g_return_if_fail(inel_clist!=NULL);
      g_return_if_fail(GTK_IS_CLIST(inel_clist));
      gtk_clist_clear (inel_clist);
    }
    
    xfr_print("\nDeleting row %i from coup_clist\n",row);
    gtk_clist_remove(coup_clist,row);
    gtk_clist_select_row(coup_clist,row==0?0:row-1,0);
    
    /* Delete row from qscale_clist */
    xfr_print("\nDeleting row %i from qscale_clist\n",row);
    if (qscale_clist)gtk_clist_remove(GTK_CLIST(qscale_clist),row);  
   
    break; 
    
  case REPLACE:
    xfr_print("Replacing selection on coup_clist for row <%i> \n",row);
    if (!selection){
      glade_util_show_message_box("No coupling selected!");
      return;
      }
    g_return_if_fail(coup_clist!=NULL);
    g_return_if_fail(GTK_IS_CLIST(coup_clist));
    /*g_print("%i =? %i",GTK_CLIST(coup_clist)->rows,	\
      GTK_CLIST(qscale_clist)->rows);*/ 
    for(iaux=0;iaux<COLUMNS_COUP;iaux++){
      /*g_print("\n %i -> text[i]=%s",iaux,text[iaux]);*/
      gtk_clist_set_text(coup_clist,row,iaux,text[iaux]);  
    }

    
    if ((ikind!=3 && ikind!=4)|| ip3<10){
      xfr_print("\n Resetting qscale_clist values\n");
      qscale_clist_set_default(row);
      xfr_print("\n Updating qscale_table...\n");
      qscale_clist_to_table(GTK_CLIST(coup_clist),row);
    }
    break;
    
  case ADD:
      gtk_clist_append(coup_clist,text);
     
      /*For kinds 3-7 (& 8 if IP2>0) create a clist to hold "CFP" namelist*/
      if (ikind<9 && ikind>2){
	cfp_clist=GTK_CLIST(gtk_clist_new(COLUMNS_CFP));
	xfr_print("\n CFP clist attached to row %i",(coup_clist)->rows-1);
	/*Attach the "step_clist" to this row of coup_clist*/
	g_return_if_fail(cfp_clist!=NULL);
	g_return_if_fail(GTK_IS_CLIST(cfp_clist));
	gtk_clist_set_row_data(GTK_CLIST(coup_clist),\
			       (coup_clist)->rows-1,cfp_clist);
      }
      /*For kinds 1,2 attach inel_clist to hold INEL namelits*/
      if (ikind==1 || ikind==2){
	inel_clist=GTK_CLIST(gtk_clist_new(COLUMNS_INEL));
	/*Attach the "step_clist" to this row of coup_clist*/
	xfr_print("\nAdding row to inel_clist\n");
	g_return_if_fail(inel_clist!=NULL);
	g_return_if_fail(GTK_IS_CLIST(inel_clist));
	gtk_clist_set_row_data(GTK_CLIST(coup_clist),\
			       (coup_clist->rows)-1,inel_clist);
      }

      /* add a blank row to qscale clist */
      xfr_print("\n  Appending row to qscale_clist\n");
      qscale_clist_add_default();
      qscale_clist_to_table(GTK_CLIST(coup_clist),(coup_clist)->rows-1);
      gtk_clist_select_row(coup_clist,(coup_clist->rows)-1,0);

      break;

    case INSERT:
      gtk_clist_insert(coup_clist,row,text);

      /* Insert row in qscale_clist with default values*/
      xfr_print("\n Inserting row %i in QSCALE clist",row);
      qscale_clist_insert_default(row);

      /*For kinds 3-7 (& 8 if IP2>0) create a clist to hold "CFP" namelist*/
      if (ikind>2 && ikind<9){
	cfp_clist=GTK_CLIST(gtk_clist_new(COLUMNS_CFP));
	/*Attach the "step_clist" to this row of coup_clist*/
	xfr_print("\n CFP clist inserted at row %i",row);
	g_return_if_fail(cfp_clist!=NULL);
	g_return_if_fail(GTK_IS_CLIST(cfp_clist));
	gtk_clist_set_row_data(GTK_CLIST(coup_clist),\
			       row,cfp_clist);
      }
      /*For kinds 1,2 attach inel_clist to hold INEL namelits*/
      if (ikind==1 || ikind==2){
	inel_clist=GTK_CLIST(gtk_clist_new(COLUMNS_INEL));
	xfr_print("\n INEL clist inserted at row %i",row);
	g_return_if_fail(inel_clist!=NULL);
	g_return_if_fail(GTK_IS_CLIST(inel_clist));
	/*Attach the "step_clist" to this row of coup_clist*/
	gtk_clist_set_row_data(GTK_CLIST(coup_clist),\
			       row,inel_clist);
      }
      break;       
    }
}




/* Add a line to qscale_list with default values */
void
qscale_clist_add_default(){
  if (qscale_clist){
    gint iq;
    gchar *blank_row[2*(QSCALEDIM+1)];
    for(iq=0;iq< QSCALEDIM+1;iq++){
      blank_row[2*iq]=g_strdup("1.000");
      blank_row[2*iq+1]=g_strdup("0.000");
      
    }
    xfr_print("\nAdding row  from qscale_clist\n");
    gtk_clist_append(GTK_CLIST(qscale_clist),blank_row);
  } 
}


/* Add a line to qscale_list with default values */
void
qscale_clist_insert_default(gint irow){
  if (qscale_clist){
    gint iq;
    gchar *uno=g_strdup("1.000");
    gchar *zero=g_strdup("0.000");
    gchar *blank_row[2*(QSCALEDIM+1)];
    for(iq=0;iq< QSCALEDIM+1;iq++){
      blank_row[2*iq]=g_strdup("1.000");
      blank_row[2*iq+1]=g_strdup("0.000");
    }
    xfr_print("\nInsert row  from qscale_clist\n");
    gtk_clist_insert(GTK_CLIST(qscale_clist),irow,blank_row);
  } 
}


/* Set default values on row "irow" of qscale_clist */
void
qscale_clist_set_default(gint irow){
  if (qscale_clist){
    gint iq,irow;
    gchar *uno=g_strdup("1.000");
    gchar *zero=g_strdup("0.000");
    for(iq=0;iq< QSCALEDIM+1;iq++){
      gtk_clist_set_text(GTK_CLIST(qscale_clist),irow,2*iq,uno);
      gtk_clist_set_text(GTK_CLIST(qscale_clist),irow,2*iq+1,zero);
    }
  } 
}



void
on_inel_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  GtkWidget *spin_ib=lookup_widget(GTK_WIDGET(clist),"inel_ib");
  GtkWidget *spin_ia=lookup_widget(GTK_WIDGET(clist),"inel_ia");
  GtkWidget *spin_k=lookup_widget(GTK_WIDGET(clist),"inel_k");
  GtkWidget *opt_no=lookup_widget(GTK_WIDGET(clist),"opt_no");
  GtkWidget *spin_no=lookup_widget(GTK_WIDGET(clist),"spin_no");
  GtkWidget *spin_kp=lookup_widget(GTK_WIDGET(clist),"inel_kp");
  GtkWidget *entry_a=lookup_widget(GTK_WIDGET(clist),"inel_a");
   
  enum{IB,IA,K,NO,KP,A};/*Columns in clist */

  gchar *text,*kptext;
  gint no,kp;
  
  gtk_clist_get_text(clist,row,IB,&text);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_ib),atoi(text));

  gtk_clist_get_text(clist,row,IA,&text);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_ia),atoi(text));

  gtk_clist_get_text(clist,row,K,&text);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_k),atoi(text));
  
  gtk_clist_get_text(clist,row,NO,&text);
  gtk_clist_get_text(clist,row,KP,&kptext);
 
  no=atoi(text);
  kp=atoi(kptext);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_kp),0);
  gtk_widget_set_sensitive(GTK_WIDGET(spin_kp),FALSE);

  if (no<0)
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_no),0);

  if (no==0)
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_no),1);

  if (no>0){
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_no),abs(no));

      if (kp==0){ 
	gtk_option_menu_set_history(GTK_OPTION_MENU(opt_no),2);
      }
      else{
	gtk_option_menu_set_history(GTK_OPTION_MENU(opt_no),3);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_kp),kp);
	gtk_widget_set_sensitive(GTK_WIDGET(spin_kp),TRUE);
      }
  }

  gtk_clist_get_text(clist,row,A,&text);
  gtk_entry_set_text(GTK_ENTRY(entry_a),text);
}


void
on_cfp_clist_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{  
  GtkWidget *opt_in=lookup_widget(GTK_WIDGET(clist),"cfp_in");
  GtkWidget *spin_ib=lookup_widget(GTK_WIDGET(clist),"cfp_ib");
  GtkWidget *spin_ia=lookup_widget(GTK_WIDGET(clist),"cfp_ia");
  GtkWidget *spin_kn=lookup_widget(GTK_WIDGET(clist),"cfp_kn");
  GtkWidget *entry_a=lookup_widget(GTK_WIDGET(clist),"cfp_a");
 
  
  enum{IN,IB,IA,KN,A}; /*Columns order in cfp_clist*/
  gchar *text[COLUMNS_INEL];/*Array to hold the text on the cells*/
  gint col;

  /*Get text for columns of selected row*/
  for (col=0;col<COLUMNS_INEL;col++){
    gtk_clist_get_text(clist,row,col,&text[col]);
  }

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_ib),atoi(text[IB]));
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_ia),atoi(text[IA]));
  gtk_entry_set_text(GTK_ENTRY(entry_a),text[A]);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_kn),abs(atoi(text[KN])));
  /*  xfr_toggle_set_value(check_keep,text[KEEP]);*/

  if (atoi(text[IN])==1)
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_in),0);
  else
    gtk_option_menu_set_history(GTK_OPTION_MENU(opt_in),1);

  /* Emit changed signal for IB and IA spinbuttons to update the information in  the text descriptions */
  gtk_signal_emit_by_name(GTK_OBJECT(spin_ib),"changed");

}


void
on_button_cfp_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  gint kind,icol,row,columns;
  gint couprow;
  const gchar *text[10]; 
  gint option=GPOINTER_TO_INT(user_data); /*Identifies the button pressed*/

  enum{ICTO,ICTFROM,KIND,IP1,IP2,IP3,P1,P2,RMAX,JMAX};

  /* Clist for inel/cpf */
  GtkCList *clist;

  /*inel and cfp clists*/
  GtkWidget *nb=lookup_widget(GTK_WIDGET(button),"nb_cfp");

  /*Current page*/
  gint page=gtk_notebook_get_current_page(GTK_NOTEBOOK(nb));

  /*Row selected in cfp/inel clist*/
  GList *selection;

  /*We also need the COUPLING clist because it holds the cfp/inel information*/
  GtkCList *coup_clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"coup_clist"));
  
  
 

  /*Row selected in coupling clist*/
  GList *listcoup=coup_clist->selection;

/*  if(!selection){ */
   if (!listcoup){
    g_warning("CFP button: No coupling selected!");
    return;
  }
  else{
     couprow=GPOINTER_TO_INT(listcoup->data);
     g_print("CFP button: line %i selected in coupling clist",couprow);
    kind= atoi(xfr_clist_get_text(coup_clist,couprow,KIND));
  }

  /*cfp/inel information */
  GtkCList *data_clist=GTK_CLIST(gtk_clist_get_row_data(coup_clist,couprow));

  
  if (page==0){
    GtkWidget *spin_ib,*spin_ia,*spin_k,*opt_no,*spin_no,*spin_kp,*entry_a;

    gchar *buffer; 
    gint no;
    enum{IB,IA,K,NO,KP,A};  /* Columns in inel_clist */ 

    /* We are in inel_clist*/
    clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"inel_clist"));
    spin_ib=lookup_widget(GTK_WIDGET(button),"inel_ib");
    spin_ia=lookup_widget(GTK_WIDGET(button),"inel_ia");
    spin_k=lookup_widget(GTK_WIDGET(button),"inel_k");
    opt_no=lookup_widget(GTK_WIDGET(button),"opt_no");
    spin_no=lookup_widget(GTK_WIDGET(button),"spin_no");
    spin_kp=lookup_widget(GTK_WIDGET(button),"inel_kp");
    entry_a=lookup_widget(GTK_WIDGET(button),"inel_a");   
       
    /*We have inel_clist active*/
    /* g_print("Notebook is on page 0\n");*/
    text[IB]=g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_ib))));
    text[IA]=g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_ia))));
    text[K]=g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_k)))); 
    buffer=g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_no))));
    text[A]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_a)));
		       
    no=xfr_option_menu_get_value(main_window,"opt_no");
    /*g_print("Antes de switch. opt_no=%i\n",no);*/

    if (kind!=1 && kind!=2){
      g_warning("INEL namelist does not apply to coupling KIND=%i",kind);
      return;
    }

    switch(no){
    case 0:/*Read complex ff. KP ignored so I set it to zero*/     
      text[NO]=g_strdup_printf("-%s",buffer);/*Makes NO negative*/
      text[KP]=g_strdup("0");
      break;
    case 1:/*Read real ff. KP ignored.*/
      text[NO]=g_strdup(buffer);
      text[KP]=g_strdup("0");
      break;
    case 2:
      text[NO]=g_strdup(buffer);
      text[KP]=g_strdup("0");
      break;
    case 3:
      text[NO]=g_strdup(buffer);
      text[KP]=gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_kp)));
      break;
    }    
  }
  
  else if (page==1){
    /*cfp_clist is active*/
    /*    GtkWidget *cfp_in=lookup_widget(GTK_WIDGET(button),"cfp_in");*/
    GtkWidget *spin_ib=lookup_widget(GTK_WIDGET(button),"cfp_ib");
    GtkWidget *spin_ia=lookup_widget(GTK_WIDGET(button),"cfp_ia");
    /*GtkWidget *check_keep=lookup_widget(GTK_WIDGET(button),"cfp_keep");*/
    GtkWidget *spin_kn=lookup_widget(GTK_WIDGET(button),"cfp_kn");
    GtkWidget *entry_a=lookup_widget(GTK_WIDGET(button),"cfp_a");

    enum{IN,IB,IA,KN,A}; /*Columns order in cfp_clist*/

    clist=GTK_CLIST(lookup_widget(GTK_WIDGET(button),"cfp_clist"));

    /*g_print("Notebook is on page 1\n");*/
    text[IB]=gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_ib)));
    text[IA]=gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_ia)));
    /*text[KEEP]=xfr_toggle_get_state(check_keep);*/
    text[KN]=gtk_entry_get_text(GTK_ENTRY(GTK_SPIN_BUTTON(spin_kn))); 
    text[A]=gtk_entry_get_text(GTK_ENTRY(entry_a));		       
    
    if (xfr_option_menu_get_value(main_window,"cfp_in")==0){ 
      text[IN]=g_strdup("1");
    }
    else{ 
      text[IN]=g_strdup("2");
    }

    if (kind==1 || kind==2){
      g_warning("CFP namelist does not apply to coupling KIND=%i",kind);
      return;
  }
    
  }
  else{
    g_warning("\nThese buttons are only for CFP and INEL! (preceeding tabs)\n");
  }
  
  /*Checks*/
  g_return_if_fail(clist!=NULL);
  g_return_if_fail(GTK_IS_CLIST(clist));
  g_return_if_fail(coup_clist!=NULL);

  /*Number of columns in inel/cfp clist*/
  columns=clist->columns;

  /*g_print("\n antes de switch en button_cfp ");*/
  selection=GTK_CLIST(clist)->selection;

  
 


  /*Row selected on step clist*/
  if (!selection){ 
    g_warning("selection empty in cfp/inel_clist\n");
    row=1;
  }
  else {
    row=GPOINTER_TO_INT(selection->data);
  } 
 
  switch(option)
    {
    case INSERT:
      gtk_clist_insert(clist,row,(gchar **) text);
      break;
      
    case REPLACE:
      if (!selection) 
	{
	  glade_util_show_message_box("No state selected!");
	  return;
	}
      for(icol=0;icol<columns;icol++)
	{
	  gtk_clist_set_text(clist,row,icol,text[icol]);
	}
      break;
      
    case DELETE:
      if (!selection) return;
      gtk_clist_remove(clist,row);
      gtk_clist_select_row(clist,row-1,0);
      break;
      
    case ADD:
      gtk_clist_append(clist,(gchar **) text);
      break;
    }

   if (page==0){
     gtk_clist_set_text(GTK_CLIST(coup_clist),couprow,3, \
			g_strdup_printf("%i",clist->rows));
   }
  
  /*Dump the content of clist to the list attached to the gpointer data
   *of the selected row in pot_clist,that is,the clist holding the potentials
   */
  xfr_clist_to_clist(GTK_CLIST(clist),GTK_CLIST(data_clist));

}


/*************************************************************************/
/*                               AUXILIARY FUNCTIONS                     */
/*************************************************************************/






/* Value of spinbutton ib or ia has changed */ 

void
on_step_state_changed_value            (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GtkWidget *step_ib=lookup_widget(GTK_WIDGET(editable),"spin_ib");
  GtkWidget *step_ia=lookup_widget(GTK_WIDGET(editable),"spin_ia");
  GtkWidget *lab_ib=lookup_widget(GTK_WIDGET(editable),"entry_ib_desc");
  GtkWidget *lab_ia=lookup_widget(GTK_WIDGET(editable),"entry_ia_desc");
  

  gchar *buffer;
  gint pot_row=0,part_number=0,kp;
  gint type,who;
  gchar *text;


  GtkWidget *pot_clist=lookup_widget(GTK_WIDGET(editable),"pot_clist");
  GtkWidget *part_clist=lookup_widget(GTK_WIDGET(editable),"part_clist");
  /*GtkWidget *states_nb=lookup_widget(GTK_WIDGET(editable),"states_nb");*/
 
  enum{IB,IBDESC,IA,IADESC,K,STR};
  enum {
    JP,COPYP,BANDP,EP,KKP,TP,CPOT,
    JT,COPYT,BANDT,ET,KTT,TT,FEXCH,IGNORE
  };
  

  /*Row selected on pot_clist*/
  GList *sel_pot=GTK_CLIST(pot_clist)->selection;
  pot_row=GPOINTER_TO_INT(sel_pot->data);  

  /*Value of KP is on column 0*/
  gtk_clist_get_text(GTK_CLIST(pot_clist),pot_row,0,&text);
  kp=atoi(text);

  /*TYPE on column 1 */
  gtk_clist_get_text(GTK_CLIST(pot_clist),pot_row,1,&text);
  type=atoi(text);

  switch(type){
  case 12: case 14:
    who=1; break;
  case 13: case 15:
    who=2; break;
  case 16: case 17:
    who=3; break;
  default:
    who=3;
    break;
  }

  /* g_print("En ib/ia changed, buscando state con cpot=%i\n",kp);*/

  part_number=get_partition_number_from_kp(GTK_CLIST(part_clist),kp);

  /*** state IB ***/
  text=g_strdup(gtk_entry_get_text(GTK_ENTRY(step_ib)));
  if (atoi(text)==0) return;

  /*Display a short information for this state beside the entry*/
  if (part_number>0)
    buffer=g_strdup(state_desc(GTK_WIDGET(main_window),\
			       part_number,atoi(text),who));
  else
    buffer=g_strdup("no partiton found!");

  /*gtk_label_set_text(GTK_LABEL(lab_ib),buffer);  */
  gtk_entry_set_text(GTK_ENTRY(lab_ib),buffer);  


  /*** state IA ***/
  text=g_strdup(gtk_entry_get_text(GTK_ENTRY(step_ia)));
  if (atoi(text)==0) return;

  /*Display a short information for this state beside the entry*/
  if (part_number>0)
    buffer=g_strdup(state_desc(GTK_WIDGET(editable),\
			       part_number,atoi(text),who));
  else
    buffer=g_strdup("no partiton found!");

  /* gtk_label_set_text(GTK_LABEL(lab_ia),buffer);  */
  gtk_entry_set_text(GTK_ENTRY(lab_ia),buffer);  
}



void
on_bins_ok_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  /*We do not destroy the window; we just hide it */
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}



void
on_button_jbord_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  /* We use the same window_jbord widget each time, so first
     of all we create it if it hasn't already been created. */
  GtkWidget *main_window=lookup_widget(GTK_WIDGET(button),"main_window");
  GtkWidget *window_jbord=lookup_widget(main_window,"window_jbord");

  if (window_jbord == NULL){
    window_jbord = create_window_jbord ();
    gtk_object_set_data(GTK_OBJECT(main_window),\
			"window_jbord",window_jbord);
  }
  /* We save a pointer to the main window inside the window_jbord's
     data list, so we can get it easily in the callbacks. */
  gtk_object_set_data (GTK_OBJECT (window_jbord), "main_window", main_window);

  gtk_widget_show (window_jbord);
  gdk_window_raise (window_jbord->window);
}


void
on_jbord_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  /*We do not destroy the window; we just hide it */
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_button_nlab_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  /* We use the same window_jbord widget each time, so first
     of all we create it if it hasn't already been created. */
  GtkWidget *main_window=lookup_widget(GTK_WIDGET(button),"main_window");
  GtkWidget *window_eintervals=lookup_widget(main_window,"window_eintervals");

  if (window_eintervals == NULL){
    window_eintervals = create_window_eintervals ();
    gtk_object_set_data(GTK_OBJECT(main_window),\
			"window_eintervals",window_eintervals);
  }
  /* We save a pointer to the main window inside the window_eintervals's
     data list, so we can get it easily in the callbacks. */
  gtk_object_set_data (GTK_OBJECT (window_eintervals), "main_window", main_window);

  gtk_widget_show (window_eintervals);
  gdk_window_raise (window_eintervals->window);
}



void
on_button_open_ccwf_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
 /* We use the same window_ccwf widget each time, so first
     of all we create it if it hasn't already been created. */
  GtkWidget *main_window=lookup_widget(GTK_WIDGET(button),"main_window");
  GtkWidget *window_ccwf=lookup_widget(main_window,"window_ccwf");

  if (window_ccwf == NULL){
    window_ccwf = create_window_ccwf ();
    gtk_object_set_data(GTK_OBJECT(main_window),"window_ccwf",window_ccwf);
  }
  /* We save a pointer to the main window inside the window_ccwf's
     data list, so we can get it easily in the callbacks. */
  gtk_object_set_data (GTK_OBJECT (window_ccwf), "main_window", main_window);

  gtk_widget_show (window_ccwf);
  gdk_window_raise (window_ccwf->window);
}


void
on_ccwf_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  /*We do not destroy the window; we just hide it */
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

void
on_button_constants_close_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
  /*We do not destroy the window; we just hide it */
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button))); 
}


void
on_check_use_ccwf_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *button=lookup_widget(GTK_WIDGET(togglebutton),"button_open_ccwf");
  guint active=togglebutton->active;
  
  gtk_widget_set_sensitive(button,active? TRUE:FALSE);
}


void
add_extra_signals(GtkWidget *main_window){

}

void
on_combo_type_changed_selection(GtkEditable *entry,gpointer data){
  GtkWidget *combo_shape=lookup_widget(GTK_WIDGET(entry),"combo_shape");
  GtkWidget *combo_type=lookup_widget(GTK_WIDGET(entry),"combo_type");
  GList *glist=NULL;
  gint pot_type=0;

  g_assert(GTK_IS_COMBO(combo_type));

  /*The number of the potential in combo_type is given by the first two 
   * characters of the item */
  pot_type=atoi(g_strndup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo_type)->entry)),2));
  
  switch (pot_type){
  case -1:
    /* Potentials without type */
    glist=g_list_append(glist,"30.-J or L-dependent formfactor");
    glist=g_list_append(glist,"40.-Parity-dependent potential");
    glist=g_list_append(glist,"41.-L-dependent potential");
    glist=g_list_append(glist,"42.-J-dependent potential");
    break;

  case 0:
    glist=g_list_append(glist,"0 .Central monopole Coulomb potential");
   break;

  case 1:/*Central potential, volume*/
    glist=g_list_append(glist,"0 .-Woods-Saxon");
    glist=g_list_append(glist,"1 .-WS squared");
    glist=g_list_append(glist,"2 .-Gaussian");
    glist=g_list_append(glist,"3 .-Yukawa");
    glist=g_list_append(glist,"4 .-Expontential");
    glist=g_list_append(glist,"5 .-Reid soft core, central part");
    glist=g_list_append(glist,"6 .-N/A");
    glist=g_list_append(glist,"7 .-Read Real");
    glist=g_list_append(glist,"8 .-Read Imaginary");
    glist=g_list_append(glist,"9 .-Read Complex");
    break;

  case 2:/*Cental potential, derivative*/
    glist=g_list_append(glist,"0 .-Woods-Saxon");
    glist=g_list_append(glist,"1 .-WS squared");
    glist=g_list_append(glist,"2 .-Gaussian");
    glist=g_list_append(glist,"3 .-Yukawa");
    glist=g_list_append(glist,"4 .-Expontential");
    glist=g_list_append(glist,"5 .-Reid soft core, spin-orbit part");
    glist=g_list_append(glist,"7 .-Read Real");
    glist=g_list_append(glist,"8 .-Read Imaginary");
    glist=g_list_append(glist,"9 .-Read Complex");
    break;
    
  case 3: case 4:/*Spin-orbit for projectile(3) or target(4)*/
    glist=g_list_append(glist,"0 .-Woods-Saxon");
    glist=g_list_append(glist,"1 .-WS squared");
    glist=g_list_append(glist,"2 .-Gaussian");
    glist=g_list_append(glist,"3 .-Yukawa");
    glist=g_list_append(glist,"4 .-Expontential");
    glist=g_list_append(glist,"5 .-Reid soft core, spin-orbit part");
    glist=g_list_append(glist,"7 .-Read Real");
    glist=g_list_append(glist,"8 .-Read Imaginary");
    glist=g_list_append(glist,"9 .-Read Complex");
    break;
    
  case 5:case 6: case 7:
    glist=g_list_append(glist,"0 .-Woods-Saxon");
    glist=g_list_append(glist,"1 .-WS squared");
    glist=g_list_append(glist,"2 .-Gaussian");
    glist=g_list_append(glist,"3 .-Yukawa");
    glist=g_list_append(glist,"4 .-Expontential");
    glist=g_list_append(glist,"5 .-Reid soft core, tensor part");
    glist=g_list_append(glist,"6 .-Vol Gaussian");
    glist=g_list_append(glist,"7 .-Read Real");
    glist=g_list_append(glist,"8 .-Read Imaginary");
    glist=g_list_append(glist,"9 .-Read Complex");
    break;
    
  case 8:/*Shape is taken from subroutine SSCC in FRESCO*/
    break;

  case 9:
    break;

  case 10: case 11: case 12: case 13:
    glist=g_list_append(glist,"7 .-Read Real");
    glist=g_list_append(glist,"8 .-Read Imaginary");
    glist=g_list_append(glist,"9 .-Read Complex");
    /*Shape 10 depends on the type of potential of the previous card*/
    glist=g_list_append(glist,"10 .-Usual deformed charged sphere (coulomb)/derivative of the undeformed potential(nuclear)");
    
    glist=g_list_append(glist,"11 .-Deform radii of previous potential and projecting by Gaussian quadrature onto the required multipoles ");
    glist=g_list_append(glist,"12 .-Idem as 11, but monopole k=0 recalculated by quadrature + 1st order correction to the radius");
    glist=g_list_append(glist,"13 .-Idem, but no first order correction made");
    break;

  case 14:
    break;
    
  case 15:
    break;

   
  default: 
    break;
  }          
  if (!glist) glist=g_list_append(glist,"  No shapes available");
  gtk_combo_set_popdown_strings(GTK_COMBO(combo_shape),glist);    
}


void
on_combo_kind_changed_selection(GtkEditable *entry,gpointer data){
/*The selection of the combo kind has changed, so the right page should
    be shown on the notebook with kind specific widgets*/
  gint kind;
  gchar *buffer;
  GtkWidget *combo_kind=lookup_widget(GTK_WIDGET(entry),"combo_kind");
  GtkWidget *nb_coup=lookup_widget(GTK_WIDGET(entry),"nb_coup");

  /*The number of the potential in combo_type is given by the first 
   * character of the item */
  buffer=g_strdup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo_kind)->entry)));
  kind=atoi(g_strndup(buffer,1));

  /*  g_print("On combo_kind selection received.Option %i\n",kind);*/
  switch(kind){
  case 1:case 2: 
    if (GTK_WIDGET_VISIBLE(nb_coup)==TRUE) 
      gtk_widget_hide(nb_coup);
    break;
  case 3:case 4:
    gtk_notebook_set_page(GTK_NOTEBOOK(nb_coup),0);
    if (GTK_WIDGET_VISIBLE(nb_coup)==FALSE) 
      gtk_widget_show(nb_coup);
    break;
  case 5:case 6:
    gtk_notebook_set_page(GTK_NOTEBOOK(nb_coup),1);
    if (GTK_WIDGET_VISIBLE(nb_coup)==FALSE) 
      gtk_widget_show(nb_coup);
    break;  
  case 7:
    gtk_notebook_set_page(GTK_NOTEBOOK(nb_coup),2);
    if (GTK_WIDGET_VISIBLE(nb_coup)==FALSE) 
      gtk_widget_show(nb_coup);
    break;  
  case 8:
    gtk_notebook_set_page(GTK_NOTEBOOK(nb_coup),3);
    if (GTK_WIDGET_VISIBLE(nb_coup)==FALSE) 
      gtk_widget_show(nb_coup);
    break;  
  case 9:
    gtk_notebook_set_page(GTK_NOTEBOOK(nb_coup),4);
    if (GTK_WIDGET_VISIBLE(nb_coup)==FALSE) 
      gtk_widget_show(nb_coup);
    break;  
  }
  /* new in version 2.0 */
  g_free(buffer);
}













void
on_check_lampA_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  /*If check_lampA is active, check_lampf should also be active*/
  GtkWidget *lampf=lookup_widget(main_window,"check_lampf");
  if (GTK_TOGGLE_BUTTON(button)->active){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lampf),TRUE);
    gtk_widget_set_sensitive(lampf,FALSE);
  }
  else gtk_widget_set_sensitive(lampf,TRUE);
}


void
on_check_ldistrib_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  /*If check_ldistrib is active, check_bpm should also be active*/
  GtkWidget *check_bpm=lookup_widget(main_window,"check_bpm");
  if (GTK_TOGGLE_BUTTON(button)->active){
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check_bpm),TRUE);
    gtk_widget_set_sensitive(check_bpm,FALSE);
  }
  else gtk_widget_set_sensitive(check_bpm,TRUE);
    
}


void
on_check_mtmin_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  /*If check_button is active mtmin should be insensitive*/
  GtkWidget *entry_mtmin=lookup_widget(GTK_WIDGET(button),"entry_mtmin");
  
  if (GTK_TOGGLE_BUTTON(button)->active)
    gtk_widget_set_sensitive(entry_mtmin,FALSE);
  else
    gtk_widget_set_sensitive(entry_mtmin,TRUE);
        
}


void
on_check_k8_ip2_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  /*When the button is active the cfp table should be displayed*/
  GtkWidget *nb_cfp=lookup_widget(GTK_WIDGET(button),"nb_cfp");
  GtkWidget *frame_cfp=lookup_widget(GTK_WIDGET(button),"frame_cfp");
  if (GTK_TOGGLE_BUTTON(button)->active){
    gtk_notebook_set_page(GTK_NOTEBOOK(nb_cfp),1);
    gtk_widget_set_sensitive(frame_cfp,TRUE);
  }
  else{
    gtk_widget_set_sensitive(frame_cfp,FALSE);
  }
}



void
on_opt_lampl_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *combo=lookup_widget(GTK_WIDGET(button),"combo_lampl");
  gint iopt=xfr_combo_get_value(GTK_WIDGET(button),"opt_lampl");
  g_print("opt_lampl clicked!!\n");
  gtk_widget_set_sensitive(combo,iopt==0?FALSE:TRUE);
}



void
on_window_nlab_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}

void
on_over_be_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{
  /* GtkWidget *window_bins=lookup_widget(main_window,"window_bins");*/
  GtkWidget *over_isc_bound=lookup_widget(main_window,"over_isc");
  GtkWidget *over_isc_cont=lookup_widget(main_window,"over_isc_cont");
  /*GtkWidget *button_bins=lookup_widget(main_window,"button_bins");*/

  if (atof(gtk_entry_get_text(GTK_ENTRY(editable)))>0){
    gtk_widget_set_sensitive(GTK_WIDGET(over_isc_bound),TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(over_isc_cont),FALSE);
  }
  else{
    gtk_widget_set_sensitive(GTK_WIDGET(over_isc_bound),FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(over_isc_cont),TRUE);
  }

}















void
on_io_files_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}



void
on_button_open_Rmatrix_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
 /* We use the same window_Rmatrix widget each time, so first
     of all we create it if it hasn't already been created. */
  GtkWidget *main_window=lookup_widget(GTK_WIDGET(button),"main_window");
  GtkWidget *window_Rmatrix=lookup_widget(main_window,"window_Rmatrix");

  if (window_Rmatrix == NULL){
    g_print("window_Rmatrix=NULL\n");
    window_Rmatrix = create_window_Rmatrix ();
    gtk_object_set_data(GTK_OBJECT(main_window),\
			"window_Rmatrix",window_Rmatrix);
  }

  /* We save a pointer to the main window inside the window_Rmatrix's
     data list, so we can get it easily in the callbacks. */
  gtk_object_set_data (GTK_OBJECT (window_Rmatrix),\
		       "main_window", main_window);
  
  gtk_widget_show (window_Rmatrix);
  gdk_window_raise (window_Rmatrix->window);
  
}


void
on_Rmat_ok_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  /*We do not destroy the window; we just hide it */
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}









gboolean
on_window_constants_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
  return TRUE;
}


gboolean
on_window_ccwf_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
  return TRUE;
}


gboolean
on_window_jbord_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
  return TRUE;
}


gboolean
on_window_nlab_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide (widget);
  return TRUE;
}


gboolean
on_window_files_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide (widget);
  return TRUE;
}


gboolean
on_window_Rmatrix_delete_event         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (widget)));
  return TRUE;
}


/* Test-> Eliminar! */
gboolean
on_main_notebook_key_press_event       (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
 return FALSE;
}

/* Eliminar !!!*/
gboolean
on_main_notebook_key_release_event     (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{
   return FALSE;
}




gboolean
on_label_integration_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  g_print("Button press");
  return FALSE;
}




void
on_check_stdout_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *statusbar=lookup_widget(main_window,"statusbar");
  gchar *buffer;
  if (GPRINT==TRUE){
    GPRINT=FALSE;
    buffer=g_strdup("switching to silence mode");
  }
  else{
    GPRINT=TRUE;
    buffer=g_strdup("switching to verbose mode");
  }
  gtk_statusbar_push(GTK_STATUSBAR(statusbar),1,buffer);
  g_free(buffer);
}




void
on_files_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_cfp_ibia_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  gint icto,ictfrom,in,coup,ib,ia,kind,icore,icomp;
  GtkWidget *coup_clist=lookup_widget(main_window,"coup_clist");
  GtkWidget *ib_desc=lookup_widget(main_window,"cfp_ib_desc");
  GtkWidget *ia_desc=lookup_widget(main_window,"cfp_ia_desc");
  GtkWidget *cfp_ia=lookup_widget(main_window,"cfp_ia");
  GtkWidget *cfp_ib=lookup_widget(main_window,"cfp_ib");
  GtkWidget *part_clist=lookup_widget(main_window,"part_clist");
  
  enum{ICT0,ICTFROM,KIND,IP1,IP2,IP3};
  enum{NAMEP,MASSP,ZP,NAMET,MASST,ZT,QVAL};

  gchar *buffer,*mass1,*mass2;

  /*Get the row which is selected in the coupling clist*/
  GList *selection=GTK_CLIST(coup_clist)->selection;
  if (!selection) 
      coup=1;
  else
      coup=GPOINTER_TO_INT(selection->data);

  /* kind of coupling */
  gtk_clist_get_text(GTK_CLIST(coup_clist),coup,KIND,&buffer); 
  kind=abs(atoi(buffer));
 
  /* Projectile/target from IN option menu */
  in=xfr_option_menu_get_value(main_window,"cfp_in")+1;
  /* g_print("in=%i\n",in); */
  
  /* ICT0 and ICTFROM are in columns 0 and 1 */
  gtk_clist_get_text(GTK_CLIST(coup_clist),coup,0,&buffer); 
  icto=abs(atoi(buffer));
  
  gtk_clist_get_text(GTK_CLIST(coup_clist),coup,1,&buffer); 
  ictfrom=abs(atoi(buffer));

  /*g_print("icto=%i ictfrom=%i\n",icto,ictfrom);*/
  /*g_print("numb of partitions=%i\n",GTK_CLIST(part_clist)->rows);*/

  if (GTK_CLIST(part_clist)->rows < MIN(icto,ictfrom)){
    g_warning("There are some partitions left!\n");
    return;
  }

    g_return_if_fail(ictfrom>0);
    g_return_if_fail(icto>0);
    if (in==1){
      gtk_clist_get_text(GTK_CLIST(part_clist),ictfrom-1,MASSP,&mass1);
      gtk_clist_get_text(GTK_CLIST(part_clist),icto-1,MASSP,&mass2);
    }
    else{
      gtk_clist_get_text(GTK_CLIST(part_clist),ictfrom-1,MASST,&mass1);
      gtk_clist_get_text(GTK_CLIST(part_clist),icto-1,MASST,&mass2);
      
    }
    

    if (atof(mass1)>atof(mass2)){
      icore=icto;
      icomp=ictfrom;
    }
    else{
      icore=ictfrom;
      icomp=icto;
    }
  
    /* g_print("in=%i mass1=%s mass2=%s icomp=%i\n",in,mass1,mass2,icomp);*/

    /*** state IB ***/
    buffer=g_strdup(gtk_entry_get_text(GTK_ENTRY(cfp_ib)));
  ib=atoi(buffer);
  if (ib==0) return;
  
  buffer=state_desc(main_window,icomp,ib,in);
  gtk_entry_set_text(GTK_ENTRY(ib_desc),buffer);

   /*** state IA ***/
  buffer=g_strdup(gtk_entry_get_text(GTK_ENTRY(cfp_ia)));
  if ((ia=atoi(buffer))==0) return;
  /* g_print("ib=%i ia=%i\n",ib,ia); */

  buffer=state_desc(main_window,icore,ia,in);
  gtk_entry_set_text(GTK_ENTRY(ia_desc),buffer);

  


}


/******************************************************************/
/** POPUP MENUS **/


gboolean MousePressed( GtkWidget * widget, GdkEvent *event, gpointer menu )
{
   /* Check to see if the event was a mouse button press */
   if ( event->type == GDK_BUTTON_PRESS )
   {
      /* Cast the event into a GdkEventButton structure */
      GdkEventButton *buttonevent = (GdkEventButton *) event;

      /* Check the button member to see which button was pressed. */
      if ( buttonevent->button == 3 )
      {
         /* If the right button was pressed, pop up the menu */
         gtk_widget_show_all (GTK_WIDGET (menu));
         gtk_menu_popup( GTK_MENU(menu), NULL, NULL, NULL
                                                   , NULL
                                                   , buttonevent->button, 0 );

         /* return TRUE because we dealt with the event */
         return TRUE;
      }
   }

   /* Return FALSE here because we didn't do anything with the event */
   return FALSE;
}

/** Menu Utility Function **/
GtkWidget *BuildMenuItem (gchar * menutext,
                           gchar acceleratorkey,
                           GtkSignalFunc signalhandler,
                           GtkWidget * menu, GtkAccelGroup * accelgroup)
{
   GtkWidget *menuitem;

   /* First, build the menu item */
   if (menutext != NULL)
      menuitem = gtk_menu_item_new_with_label (menutext);
   else
      menuitem = gtk_menu_item_new ();

   /* Next,  attach the signal handler */
   if (signalhandler != NULL)
      gtk_signal_connect (GTK_OBJECT (menuitem),
                           "activate", signalhandler, NULL);

   /* Now we can add the item to the menu itself */
   if (menu != NULL)
      gtk_menu_append (GTK_MENU (menu), menuitem);

/* Finally, build the accelerator */
   if (accelgroup != NULL && (guint) acceleratorkey != 0)
      gtk_accel_group_add (accelgroup,
       (guint) acceleratorkey,
       GDK_CONTROL_MASK,
       GTK_ACCEL_VISIBLE,
       GTK_OBJECT (menuitem), "activate");

   /* All done, return the menu item */
   return menuitem;
}

gboolean
on_window_ener_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_entry_elab_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GtkWidget *elab1=lookup_widget(GTK_WIDGET(editable),"elab1");

  /* gulong handler=g_signal_handler_find(G_OBJECT(elab1), */
  /* 				       G_SIGNAL_MATCH_FUNC,0,0,NULL, */
  /* 				       on_elab1_changed,NULL); */


  g_signal_handlers_block_by_func (GTK_EDITABLE(elab1), on_elab1_changed, NULL);
  /*  g_signal_handler_block ((gpointer) elab1,handler);*/
  gtk_entry_set_text(GTK_ENTRY(elab1), 
 		     gtk_entry_get_text(GTK_ENTRY(editable)));
  g_signal_handlers_unblock_by_func (GTK_EDITABLE(elab1),on_elab1_changed, NULL);
  /*g_signal_handler_unblock ((gpointer) elab1,handler);*/

}


void
on_elab1_changed                       (GtkEditable     *editable,
                                        gpointer         user_data)
{
  g_print("entry elab1_changed called\n");
  GtkWidget *elab=lookup_widget(GTK_WIDGET(editable),"elab");
  g_signal_handlers_block_by_func (GTK_EDITABLE(elab), on_entry_elab_changed, NULL);
  gtk_entry_set_text(GTK_ENTRY(elab), 
 		     gtk_entry_get_text(GTK_ENTRY(editable))); 
  g_signal_handlers_unblock_by_func (GTK_EDITABLE(elab), on_entry_elab_changed, NULL);
 
}


void
on_elab2_changed                       (GtkEditable     *editable,
                                        gpointer         user_data)
{

}


void
on_elab3_changed                       (GtkEditable     *editable,
                                        gpointer         user_data)
{

}



gboolean
on_qscale_focus_out_event              (GtkWidget       *widget,
                                        GdkEventFocus   *event,
                                        gpointer         user_data)
{
  gint iq,coup_row;
  GtkWidget *coup_clist=lookup_widget(GTK_WIDGET(widget),"coup_clist");
  /*Get the data of the selected row in coup_clist*/
  GList *selection=GTK_CLIST(coup_clist)->selection;

  gchar *rowtext[2*(QSCALEDIM+1)];

  /*Row selected on coup clist*/
  if (!selection){
    g_warning("No row selected in couplings!\n");
    return 0;
  }
  else {
    coup_row=GPOINTER_TO_INT(selection->data);
  }


  for(iq=0;iq<=QSCALEDIM;iq++){
    gint col=2*iq;
    gchar *bufr=g_strdup_printf("q%i_nr",iq);
    gchar *bufi=g_strdup_printf("q%i_ni",iq);
    GtkWidget *entry1=lookup_widget(GTK_WIDGET(coup_clist),bufr); /* table cell qx_nr */
    GtkWidget *entry2=lookup_widget(GTK_WIDGET(coup_clist),bufi); /* table cell qx_ni */
    


    if (entry1 && entry2){
      rowtext[2*iq]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry1)));
      rowtext[2*iq+1]=g_strdup(gtk_entry_get_text(GTK_ENTRY(entry2)));
    }
    else{
      g_print("Internal error. Could not retrieve entry in qscale_changed\n");
      return 0;
    }
  }
  if (qscale_clist==NULL) g_warning("qscale_clist is NULL!\n");
  g_assert(GTK_IS_CLIST(qscale_clist));
  if ((GTK_CLIST(qscale_clist)->rows)-1 < coup_row){
    g_warning("\n In qscale clist number of rows is %i, but expected at least %i\n",\
	      GTK_CLIST(qscale_clist)->rows,coup_row+1);
    return 0;
  }
  gtk_clist_insert(GTK_CLIST(qscale_clist),coup_row,rowtext);
  return FALSE;
}




gboolean
on_label_inel_pressed                  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  g_print("\nLabel INEL pressed \n");
  return FALSE;
}



/* Dummy functions that are not used in GTK2 */
GtkAccelGroup* 
gtk_menu_ensure_uline_accel_group (GtkMenu *menu)
{
};


void        
gtk_accel_group_add             (GtkAccelGroup *accel_group,
				 guint accel_key,
				 GdkModifierType accel_mods,
				 GtkAccelFlags accel_flags,
				 GtkObject *object,
				 const gchar *accel_signal)
{
};




/* void */
/* on_revert_activate                     (GtkMenuItem     *menuitem, */
/*                                         gpointer         user_data) */
/* { */

/* } */


void
on_part_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

}


void
on_states_clist_select_row             (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

}


/* void */
/* on_partitions_select_row               (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


/* void */
/* on_states_select_row                   (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


/* -------------------------    PARTITIONS BUTTONS ------------------------------*/

void
on_partition_add_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

/* void on_button_partition_clicked(GtkButton *button, gpointer user_data) */
  on_button_partition_clicked(button, GINT_TO_POINTER(ADD));
}


void
on_partition_replace_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
 on_button_partition_clicked(button, GINT_TO_POINTER(REPLACE));
}


void
on_partition_insert_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
 on_button_partition_clicked(button, GINT_TO_POINTER(INSERT));
}


void
on_partition_delete_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_partition_clicked(button, GINT_TO_POINTER(DELETE));
}
/* ---------------------------------------------------------------------- */



/* -------------------------    STATES BUTTONS ------------------------------*/
void
on_states_replace_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_states_clicked(button, GINT_TO_POINTER(REPLACE));
}


void
on_states_insert_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_states_clicked(button, GINT_TO_POINTER(INSERT));
}


void
on_states_add_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_states_clicked(button, GINT_TO_POINTER(ADD));
}


void
on_states_delete_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_states_clicked(button, GINT_TO_POINTER(DELETE));
}
/* ---------------------------------------------------------------------- */



/* -------------------------   POTENTIALS BUTTONS ------------------------------*/
void
on_potential_add_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_potential_clicked(button, GINT_TO_POINTER(ADD));
}


void
on_potential_insert_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_potential_clicked(button, GINT_TO_POINTER(INSERT));
}


void
on_potential_replace_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_potential_clicked(button, GINT_TO_POINTER(REPLACE));
}


void
on_potential_delete_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_potential_clicked(button, GINT_TO_POINTER(DELETE));
}

/* ---------------------------------------------------------------------- */



/* -------------------------   STEP BUTTONS ------------------------------*/
void
on_step_add_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_step_clicked(button,GINT_TO_POINTER(ADD));
}


void
on_step_insert_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_step_clicked(button,GINT_TO_POINTER(INSERT));
}


void
on_step_replace_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_step_clicked(button,GINT_TO_POINTER(REPLACE));
}


void
on_step_delete_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_step_clicked(button,GINT_TO_POINTER(DELETE));
}
/* ---------------------------------------------------------------------- */



/* -------------------------   OVERLAP BUTTONS ------------------------------*/

void
on_overlap_add_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_overlap_clicked (button,GINT_TO_POINTER(ADD));
}


void
on_overlap_insert_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_overlap_clicked(button,GINT_TO_POINTER(INSERT));
}


void
on_overlap_replace_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_overlap_clicked(button,GINT_TO_POINTER(REPLACE));
}


void
on_overlap_delete_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_overlap_clicked(button,GINT_TO_POINTER(DELETE));
}
/* ---------------------------------------------------------------------- */



/* -------------------------   COUPLING BUTTONS ------------------------------*/

void
on_coupling_add_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_coupling_clicked(button,GINT_TO_POINTER(ADD));
}


void
on_coupling_insert_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_coupling_clicked  (button,GINT_TO_POINTER(INSERT));
}


void
on_coupling_replace_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_coupling_clicked  (button,GINT_TO_POINTER(REPLACE));
}


void
on_coupling_delete_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
   on_button_coupling_clicked  (button,GINT_TO_POINTER(DELETE));
}
/* ---------------------------------------------------------------------- */



/* -------------------------   CFP BUTTONS ------------------------------*/

void
on_cfp_add_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_cfp_clicked  (button,GINT_TO_POINTER(ADD));
}


void
on_cfp_insert_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_cfp_clicked  (button,GINT_TO_POINTER(INSERT));
}


void
on_cfp_replace_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_cfp_clicked  (button,GINT_TO_POINTER(REPLACE));
}


void
on_cfp_delete_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  on_button_cfp_clicked  (button,GINT_TO_POINTER(DELETE));
}
/* ---------------------------------------------------------------------- */





/* void */
/* on_pot_clist_select_row                (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


/* void */
/* on_step_clist_select_row               (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


/* void */
/* on_overlap_clist_select_row            (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


/* void */
/* on_coup_clist_select_row               (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


/* void */
/* on_inel_clist_select_row               (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


/* void */
/* on_cfp_clist_select_row                (GtkCList        *clist, */
/*                                         gint             row, */
/*                                         gint             column, */
/*                                         GdkEvent        *event, */
/*                                         gpointer         user_data) */
/* { */

/* } */


void
on_toggle_2ntrans_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *frame_bins=lookup_widget(GTK_WIDGET(togglebutton),"frame_bins");
  GtkWidget *hbox_zr=lookup_widget(GTK_WIDGET(togglebutton),"hbox_zr");
  GtkWidget *frame_2ntransfer=lookup_widget(GTK_WIDGET(togglebutton),"frame_2ntransfer");
  
  if (GTK_TOGGLE_BUTTON(togglebutton)->active){
    gtk_widget_show(frame_bins);
    gtk_widget_show(hbox_zr);
    gtk_widget_show(frame_2ntransfer);
  }
  else{
    gtk_widget_hide(frame_bins);
    gtk_widget_hide(hbox_zr);
    gtk_widget_hide(frame_2ntransfer);

  }  

}



/* Combo "KIND of overlap" changed: 
   We need to activate the LMAX entry for deformed potentials (KIND>0 
   or deactivate it otherwise */
void
on_kind_overlap_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
 
 
}


void
on_toggle_elab_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget *hbox_eintervals=lookup_widget(GTK_WIDGET(togglebutton),
					   "hbox_eintervals");
  xfr_print("toggle button ELAB toggled");
  if (GTK_TOGGLE_BUTTON(togglebutton)->active){
    gtk_widget_show(hbox_eintervals);
  }
  else{
    gtk_widget_hide(hbox_eintervals);
  }  
}









gboolean
on_misc_bins_press_event               (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
  GtkWidget *table_misc_bins=lookup_widget(GTK_WIDGET(widget),
					   "table_misc_bins");
  g_print("misc_bins pressed\n");
  if (GTK_WIDGET_VISIBLE(table_misc_bins)==TRUE){
    gtk_widget_hide(table_misc_bins);
  }else{
    gtk_widget_show(table_misc_bins);
  }
  
  return FALSE;
}

