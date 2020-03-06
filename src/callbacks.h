/*  Note: You are free to use whatever license you want.
    Eventually you will be able to edit it within Glade. */

/*  xfresco
 *  Copyright (C) <1999> < A.M. Moro>
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

#include <gtk/gtk.h>

enum {ZERO,
      REPLACE,
      INSERT,
      ADD,
      DELETE
}BUTTONS;



enum {
  JP,COPYP,BANDP,EP,KKP,TP,CPOT,PWF,JT,COPYT,BANDT,ET,KKT,TT,EXCH,IGNORE,INFAM,OUTFAM
}STATES;

enum {
  KP,TYPE,ITT,SHAPE,DEF,MENP,MNET,AP,AT,RC,AC,P,P0,P1,P2,P3,P4,P5,P6,P7,
  V,R0,RV,VR0,A,AV,W,WR0,RW,AW,WA,R0W,VSL,RSL,RSO0,ASO,VSOI,RSOI,ASOI,WD,
  WDR,WDA,WDR0,AWD,DEFP,DEFT,VD,VDR,VDA
}POTENTIALS;


/*
 * MENUBAR
 */


void
on_file_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Open_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Save_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Save_as_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Exit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_confirm_exit(GtkWidget *button,gpointer data);

void
on_Edit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Show_input_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_run_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


gboolean
on_open_filesel_delete_ev              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_delete_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_insert_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_replace_partition_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_add_partition_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_partitions_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_states_select_row               (GtkCList        *clist,
				    gint             row,
				    gint             column,
				    GdkEvent        *event,
				    gpointer         user_data);
void
on_step_select_row               (GtkCList        *clist,
				  gint             row,
				  gint             column,
				  GdkEvent        *event,
				  gpointer         user_data);



void
on_button_states_clicked               (GtkButton       *button,
                                        gpointer         user_data);

gint
on_band_get_value(GtkWidget *band);


gint
on_infam_get_value(GtkWidget *opt);

gint
on_outfam_get_value(GtkWidget *opt);

/*
void
on_bandp_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_bandt_clicked                       (GtkButton       *button,
                                      gpointer         user_data);

*/

void
on_band_clicked                        (GtkButton       *button,
                                        gpointer         user_data);


void
on_button_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_potential_delete             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_coupling_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_replace_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_cfp_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_overlap_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_potential_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_coupling_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_potential_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_cfp_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data);


void
on_button_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_states_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_cplstates_clicked            (GtkButton       *button,
                                        gpointer         user_data);



void
on_potential_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_step_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_step_select_row                     (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_button_step_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_step_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_step_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_step_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_coup_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);


void
qscale_clist_to_table(GtkCList *clist,gint coup_row);

void
qscale_clist_add_default();

void
qscale_clist_insert_default(gint irow);

void
qscale_clist_set_default(gint irow);

void
xfr_qscale_table_freeze(GtkWidget *table);

void
xfr_qscale_table_thaw(GtkWidget *table);



void
on_inel_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_cfp_clist_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);


void
on_overlap_clist_select_row            (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_check_vary_be_clicked               (GtkButton       *button,
                                        gpointer         user_data);


void
on_step_state_changed_value            (GtkEditable     *editable,
                                        gpointer         user_data);



void
on_button_bins_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bins_ok_clicked                     (GtkButton       *button,
                                        gpointer         user_data);


void
on_ccwf_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_jbord_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_jbord_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_ccwf_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_constants_close_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_use_ccwf_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_open_ccwf_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_states_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_open_filesel_ok_button_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_open_filesel_cancel_button_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_save_filesel_ok_button_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_save_filesel_cancel_button_clicked  (GtkButton       *button,
                                        gpointer         user_data);
void
add_extra_signals(GtkWidget *mainwindow);


void
on_Run_options_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Run_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_General_params_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_check_lampA_clicked                 (GtkButton       *button,
                                        gpointer         user_data);



void
on_check_ldistrib_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_mtmin_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_k8_ip2_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_New_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_About_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Version_activate                    (GtkMenuItem     *menuitem,                                     
					gpointer         user_data);



void
on_import_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_print_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_combo_type_changed_selection        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_combo_kind_changed_selection        (GtkEditable     *editable,
                                        gpointer         user_data);
void
on_about_button_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_opt_lampl_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_opt_lampl_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_band_clicked                        (GtkButton       *button,
                                        gpointer         user_data);


void
on_main_window_destroy                 (GtkObject       *object,
                                        gpointer         user_data);

void
on_button_nlab_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_window_nlab_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_New_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_import_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Run_options_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Run_activate                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_General_params_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_About_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Version_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_button_open_ccwf_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_use_ccwf_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_jbord_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_nlab_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_ldistrib_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_opt_lampl_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_mtmin_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_band_clicked                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_partition_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_partitions_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_button_states_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_states_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_combo_type_changed_selection        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button_potential_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_potential_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_step_state_changed_value            (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button_step_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_step_select_row                     (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_button_bins_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_check_vary_be_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_overlap_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_overlap_clist_select_row            (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_combo_kind_changed_selection        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_check_k8_ip2_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_coupling_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_coup_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_inel_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_cfp_clist_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_button_cfp_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_open_filesel_delete_ev              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_open_filesel_ok_button_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_open_filesel_cancel_button_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_bins_ok_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_constants_close_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_ccwf_ok_button_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_jbord_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_save_filesel_ok_button_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_save_filesel_cancel_button_clicked  (GtkButton       *button,
                                        gpointer         user_data);


void
on_over_be_changed                     (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_io_files_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);



/* Callback regularly called */
gint
timeout_callback(gpointer data);

void
on_revert_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_button_open_Rmatrix_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_Rmat_ok_clicked                     (GtkButton       *button,
                                        gpointer         user_data);



void
on_window_Rmatrix_destroy              (GtkObject       *object,
                                        gpointer         user_data);

void
on_window_bins_destroy                 (GtkObject       *object,
                                        gpointer         user_data);

gboolean
on_window_bins_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_window_constants_delete_event       (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_window_ccwf_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_window_jbord_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_window_nlab_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_window_files_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_window_Rmatrix_delete_event         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_main_notebook_key_press_event       (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_main_notebook_key_release_event     (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_label_integration_button_press_event(GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_check_stdout_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);




void
on_files_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);



void
on_cfp_ibia_changed                    (GtkEditable     *editable,
                                        gpointer         user_data);
GtkWidget 
*BuildMenuItem (gchar * menutext,
                           gchar acceleratorkey,
                           GtkSignalFunc signalhandler,
			  GtkWidget * menu, GtkAccelGroup * accelgroup);


gboolean 
MousePressed( GtkWidget * widget, GdkEvent *event, gpointer menu );

void
popup_delete(GtkWidget *widget,gpointer data);

void
popup_moveup(GtkWidget *widget,gpointer data);

void
popup_movedown(GtkWidget *widget,gpointer data);


void
create_popup();

gboolean
on_window_ener_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_entry_elab_changed                  (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_elab1_changed                       (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_elab2_changed                       (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_elab3_changed                       (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_kill_current_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_qscale_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);


gboolean
on_qscale_focus_out_event              (GtkWidget       *widget,
                                        GdkEventFocus   *event,
                                        gpointer         user_data);

gboolean
on_label_inel_pressed                  (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);



void
on_revert_activate2                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);



/* Missing functions in GTK2 */
GtkAccelGroup* 
gtk_menu_ensure_uline_accel_group (GtkMenu *menu);


void        
gtk_accel_group_add             (GtkAccelGroup *accel_group,
				 guint accel_key,
				 GdkModifierType accel_mods,
				 GtkAccelFlags accel_flags,
				 GtkObject *object,
				 const gchar *accel_signal);

void
on_revert_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_part_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_states_clist_select_row             (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_partitions_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_states_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_states_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_states_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_partition_add_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_partition_replace_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_partition_insert_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_partition_delete_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_states_replace_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_states_insert_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_states_add_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_states_delete_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_potential_add_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_potential_insert_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_potential_replace_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_potential_delete_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_step_add_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_step_insert_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_step_replace_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_step_delete_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_overlap_add_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_overlap_insert_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_overlap_replace_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_overlap_delete_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_coupling_add_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_coupling_insert_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_coupling_replace_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_coupling_delete_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_cfp_add_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_cfp_insert_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_cfp_replace_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_cfp_delete_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_pot_clist_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_step_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_overlap_clist_select_row            (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_coup_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_inel_clist_select_row               (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_cfp_clist_select_row                (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_toggle_2ntrans_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);


void
on_kind_overlap_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_toggle_elab_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);






gboolean
on_misc_bins_press_event               (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);
