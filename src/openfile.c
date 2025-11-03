#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <gtk/gtk.h>

#include "clist_compat.h"
#include "utils.h"
#include "xfresco.h"
#include "interface.h"
#include "openfile.h"
#include "support.h"

#include "callbacks.h"

extern void on_states_select_row(GtkCList        *clist,
				    gint             row,
				    gint             column,
				    GdkEvent        *event,
				    gpointer         user_data);


extern GtkWidget *main_window;
extern gint nwarnings;

extern GtkWidget *qscale_clist ;

/* Complicated dialog boxes can be slow to create, so we create them once and
   keep pointers to them in static variables so we can reuse them. */
GtkWidget *open_filesel = NULL;
GtkWidget *save_filesel = NULL;
/* GtkWidget *window_constants = NULL; */

/* This is the filename of the file currently being edited. */
extern gchar *current_filename ;

/* This flag is set to TRUE if the file has been changed. We don't actually
   use it here, but the program could be extended to prompt to save changes. */
gboolean file_changed = FALSE;

/* A key used to store pointers to the main window. */
const gchar *MainWindowKey = "MainWindowKey";

/********************************************/

/*
enum{OK,END_OF_FILE,EMPTYLIST,ENDOFLIST};
*/

gint 
real_open_file(GtkWidget *main_window,const gchar *filename)
{
  /*Some required widgets*/
  GtkWidget *statusbar=lookup_widget(main_window,"statusbar");
  GtkToggleButton *toggle; /*a generic toggle_button, for general use*/
  GtkWidget *widget; /* a generic widget*/

  /*  gint npages;*/

  /*For overlaps*/
  GtkWidget *overlap_clist;
 
  /*Potentials*/
  GtkWidget *step_clist,*pot_clist;
  /*GtkWidget *states_nb;*/

  /*Dialog windows*/
  /* GtkWidget *window_bins=NULL;*/
  /*GtkWidget *window_ccwf=NULL;*/
  /* GtkWidget *window_constants=NULL; */
  GtkWidget *window_jbord=NULL;
  GtkWidget *window_Rmatrix=NULL;
  GtkWidget *window_eintervals=NULL;

  gint part_number=0,pot_number=0,overlap_numb=0;
  gint inex,i,iwaves,iread;
  guint handler_id;

  /* gchar *buffer;*/
  GList *potlist=NULL;
  FILE *f;

  guint contid;

  
  /*For partitions*/
  GtkWidget *part_clist=lookup_widget(main_window,"part_clist");
  GtkWidget *states_clist=lookup_widget(main_window,"states_clist");

  


  /***********************   NAMELIST STUFF    ********************/

  /*Synonymous
  Vardesc equiv_p1[3],equiv_p2[1],equiv_p3[1];
  Vardesc equiv_p4[1],equiv_p5[1],equiv_p6[1],equiv_p7[1];
  */
  /*
#define p1_dv (equiv_p1)
#define p_dv (equiv_p1)
  */
  /*Variable description for namelist "fresco"*/
  Vardesc hcm_dv=define("hcm","0.1",XFR_NUMBER);
  Vardesc rmatch_dv=define("rmatch","",XFR_NUMBER);
  Vardesc rintp_dv=define("rintp","0.5",XFR_NUMBER);
  Vardesc hnl_dv=define("hnl","0",XFR_NUMBER);
  Vardesc rnl_dv=define("rnl","0",XFR_NUMBER);
  Vardesc centre_dv=define("centre","0",XFR_NUMBER);
  Vardesc hnn_dv=define("hnn","0",XFR_NUMBER);
  Vardesc rnn_dv=define("rnn","0",XFR_NUMBER);
  Vardesc rmin_dv=define("rmin","0",XFR_NUMBER);
  Vardesc rsp_dv=define("rsp","0",XFR_NUMBER);
  Vardesc rasym_dv=define("rasym","0.0",XFR_NUMBER);/*  ??? */
  Vardesc accrcy_dv=define("accrcy","1000",XFR_NUMBER);/*  ??? */
  Vardesc Switch_dv=define("switch","0.0",XFR_NUMBER);
  Vardesc ajswtch_dv=define("ajswtch","0",XFR_NUMBER);
  Vardesc jtmin_dv=define("jtmin","0",XFR_NUMBER);
  Vardesc jtmax_dv=define("jtmax","0",XFR_NUMBER);
  Vardesc absend_dv=define("absend","0",XFR_NUMBER);
  Vardesc dry_dv=define("dry","F",XFR_STRING);
  Vardesc rela_dv=define("rela","F",XFR_STRING);
  Vardesc nearfa_dv=define("nearfa","0",XFR_NUMBER);
  Vardesc jump_dv=define_array("jump","0",1,6,1,XFR_NUMBER);
  Vardesc jbord_dv=define_array("jbord","0",1,6,0,XFR_NUMBER);
  Vardesc kqmax_dv=define("kqmax","0",XFR_NUMBER);
  Vardesc pp_dv=define("pp","0",XFR_NUMBER);
  Vardesc thmin_dv=define("thmin","0",XFR_NUMBER);
  Vardesc thmax_dv=define("thmax","180",XFR_NUMBER);
  Vardesc thinc_dv=define("thinc","1",XFR_NUMBER);
  Vardesc koords_dv=define("koords","0",XFR_NUMBER);
  Vardesc cutl_dv=define("cutl","0",XFR_NUMBER);
  Vardesc cutr_dv=define("cutr","0",XFR_NUMBER);
  Vardesc cutc_dv=define("cutc","0",XFR_NUMBER);
  Vardesc ips_dv=define("ips","0",XFR_NUMBER);
  Vardesc smallchan_dv=define("smallchan","0",XFR_NUMBER);
  Vardesc smallcoup_dv=define("smallcoup","0",XFR_NUMBER);
  Vardesc sinjmax_dv=define("sinjmax","0",XFR_NUMBER);
 
  Vardesc plane_dv=define("plane","0",XFR_NUMBER);
  Vardesc it0_dv=define("it0","0",XFR_NUMBER);
  Vardesc iter_dv=define("iter","0",XFR_NUMBER);
  Vardesc iblock_dv=define("iblock","0",XFR_NUMBER);
  Vardesc complexbins_dv=define("complexbins","F",XFR_STRING);

  /* */
  

  /*PVM*/
 
  Vardesc numnode_dv=define("numnode","0",XFR_NUMBER);


  /* R-matrix */
  Vardesc nrbases_dv=define("nrbases","0",XFR_NUMBER);
  Vardesc nrbmin_dv=define("nrbmin","0",XFR_NUMBER);
  Vardesc pralpha_dv=define("pralpha","F",XFR_STRING);
  Vardesc pcon_dv=define("pcon","0",XFR_NUMBER);
  Vardesc meigs_dv=define("meigs","0",XFR_NUMBER);
  Vardesc rmatr_dv=define("rmatr","0",XFR_NUMBER);
  Vardesc buttle_dv=define("buttle","0",XFR_NUMBER);
  Vardesc ebeta_dv=define("ebeta","0",XFR_NUMBER);
  
  Vardesc pade_dv=define("pade","0",XFR_NUMBER);
  Vardesc iso_dv=define("iso","",XFR_STRING);
  Vardesc nnu_dv=define("nnu","18",XFR_NUMBER);
  Vardesc maxl_dv=define("maxl","0",XFR_NUMBER);
  Vardesc minl_dv=define("minl","0",XFR_NUMBER);
  Vardesc mtmin_dv=define("mtmin","0",XFR_NUMBER);
  Vardesc epc_dv=define("epc","0",XFR_NUMBER);
  Vardesc erange_dv=define("erange","",XFR_NUMBER);
  Vardesc dk_dv=define("dk","0",XFR_NUMBER);
  Vardesc chans_dv=define("chans","0",XFR_NUMBER);
  Vardesc listcc_dv=define("listcc","0",XFR_NUMBER);
  Vardesc treneg_dv=define("treneg","0",XFR_NUMBER);
  Vardesc cdetr_dv=define("cdetr","0",XFR_NUMBER);
  Vardesc smats_dv=define("smats","0",XFR_NUMBER);
  Vardesc xstabl_dv=define("xstabl","0",XFR_NUMBER);
  Vardesc nlpl_dv=define("nlpl","0",XFR_NUMBER);
  Vardesc waves_dv=define("waves","0",XFR_NUMBER);
  Vardesc lampl_dv=define("lampl","0",XFR_NUMBER);
  Vardesc veff_dv=define("veff","0",XFR_NUMBER);
  Vardesc kfus_dv=define("kfus","0",XFR_NUMBER);
  Vardesc nfus_dv=define("nfus","0",XFR_NUMBER);
  Vardesc wdisk_dv=define("wdisk","0",XFR_NUMBER);
  Vardesc bpm_dv=define("bpm","0",XFR_NUMBER);
  Vardesc melfil_dv=define("melfil","0",XFR_NUMBER);
  Vardesc cdcc_dv=define("cdcc","0",XFR_NUMBER);
  Vardesc pel_dv=define("pel","1",XFR_NUMBER);
  Vardesc exl_dv=define("exl","1",XFR_NUMBER);
  Vardesc lab_dv=define("lab","1",XFR_NUMBER);
  Vardesc lin_dv=define("lin","1",XFR_NUMBER);
  Vardesc lex_dv=define("lex","1",XFR_NUMBER);
  /*Vardesc elab_dv=define("elab","0",XFR_NUMBER);*/
  Vardesc elab_dv=define_array("elab","0",1,4,0,XFR_NUMBER);
  Vardesc nlab_dv=define_array("nlab","0",1,3,0,XFR_NUMBER);
  Vardesc fatal_dv=define("fatal","T",XFR_STRING);
  Vardesc nosol_dv=define("nosol","F",XFR_STRING);
  Vardesc nangle_dv=define("nangle","",XFR_NUMBER);
  Vardesc pset_dv=define("pset","0",XFR_NUMBER);
  Vardesc jset_dv=define("jset","0",XFR_NUMBER);
  Vardesc inh_dv=define("inh","",XFR_NUMBER);
  Vardesc psiren_dv=define("psiren","F",XFR_STRING);
  Vardesc tmp_dv=define("tmp","",XFR_DELSTR);
  Vardesc masfil_dv=define("masfil","",XFR_DELSTR);
  Vardesc unitmass_dv=define("unitmass","1.000",XFR_NUMBER);
  Vardesc finec_dv=define("finec","137.03599d0",XFR_NUMBER);

  Vardesc expand_dv=define_array("expand","0",1,6,1,XFR_NUMBER);
  
  /*Variable description for Namelist partition*/
  Vardesc namep_dv=define("namep","",XFR_DELSTR);
  Vardesc massp_dv=define("massp","",XFR_NUMBER);
  Vardesc zp_dv=define("zp","",XFR_NUMBER);
  Vardesc nex_dv=define("nex","",XFR_NUMBER);
  Vardesc namet_dv=define("namet","",XFR_DELSTR);
  Vardesc masst_dv=define("masst","",XFR_NUMBER);
  Vardesc zt_dv=define("zt","",XFR_NUMBER);
  Vardesc qval_dv=define("qval","0.0000",XFR_NUMBER);
  Vardesc pwf1_dv=define("pwf","T",XFR_STRING);/*I call it  pwf1 instead of pwf to avoid confusion with pwf in states*/
  Vardesc readstates_dv=define("readstates","",XFR_NUMBER);

  

  /*Variable description for Namelist states */
  Vardesc jp_dv=define("jp","0.0",XFR_NUMBER);
  Vardesc copyp_dv=define("copyp","",XFR_NUMBER);
  Vardesc bandp_dv=define("bandp","",XFR_NUMBER);
  Vardesc ep_dv=define("ep","0.0000",XFR_NUMBER);
  Vardesc kkp_dv=define("kkp","",XFR_NUMBER);
  Vardesc tp_dv=define("tp","",XFR_NUMBER);
  Vardesc cpot_dv=define("cpot","",XFR_NUMBER);
  Vardesc jt_dv=define("jt","0.0",XFR_NUMBER);
  Vardesc copyt_dv=define("copyt","",XFR_NUMBER);
  Vardesc bandt_dv=define("bandt","",XFR_NUMBER);
  Vardesc et_dv=define("et","0.0000",XFR_NUMBER);
  Vardesc kkt_dv=define("kkt","",XFR_NUMBER);
  Vardesc tt_dv=define("tt","",XFR_NUMBER);
  Vardesc fexch_dv=define("fexch","F",XFR_STRING);
  Vardesc ignore_dv=define("ignore","F",XFR_STRING);
  Vardesc ptyp_dv=define("ptyp","",XFR_NUMBER);
  Vardesc ptyt_dv=define("ptyt","",XFR_NUMBER);
  Vardesc infam_dv=define("infam","0",XFR_NUMBER);
  Vardesc outfam_dv=define("outfam","0",XFR_NUMBER);

  /* Deprecated variables in namelist STATES */
  Vardesc pwf_dv=define("pwf","F",XFR_STRING); 
  Vardesc extra_dv=define("extra","F",XFR_STRING);
  Vardesc spifl_dv=define("spifl","F",XFR_STRING);


  /*Variables for namelist: pot */  
  Vardesc kp_dv=define("kp","-1",XFR_NUMBER);
  Vardesc type_dv=define("type","-1",XFR_NUMBER);
  Vardesc itt_dv=define("itt","F",XFR_STRING);
  Vardesc shape_dv=define("shape","0",XFR_NUMBER);
  Vardesc mnep_dv=define_array("mnep","0",1,7,0,XFR_NUMBER);
  Vardesc mnet_dv=define_array("mnet","0",1,7,0,XFR_NUMBER);
  Vardesc ap_dv=define("ap","0",XFR_NUMBER);
  Vardesc at_dv=define("at","0",XFR_NUMBER);
  Vardesc rc_dv=define("rc","0",XFR_NUMBER);
  Vardesc ac_dv=define("ac","0",XFR_NUMBER);
  Vardesc p_dv=define_array("p","0",1,7,0,XFR_NUMBER);
  Vardesc def_dv=define_array("def","0",1,7,0,XFR_NUMBER);
  Vardesc p0_dv=define("p0","0",XFR_NUMBER);
  Vardesc p1_dv=define("p1","0",XFR_NUMBER);
  Vardesc p2_dv=define("p2","0",XFR_NUMBER);
  Vardesc p3_dv=define("p3","0",XFR_NUMBER);
  Vardesc p4_dv=define("p4","0",XFR_NUMBER);
  Vardesc p5_dv=define("p5","0",XFR_NUMBER);
  Vardesc p6_dv=define("p6","0",XFR_NUMBER);
  Vardesc p7_dv=define("p7","0",XFR_NUMBER);
  Vardesc v_dv=define("v","0",XFR_NUMBER);
  Vardesc r0_dv=define("r0","0",XFR_NUMBER);
  Vardesc rv_dv=define("rv","0",XFR_NUMBER);
  Vardesc vr0_dv=define("vr0","0",XFR_NUMBER);
  Vardesc a_dv=define("a","0",XFR_NUMBER);
  Vardesc av_dv=define("av","0",XFR_NUMBER);
  Vardesc w_dv=define("w","0",XFR_NUMBER);
  Vardesc wr0_dv=define("wr0","0",XFR_NUMBER);
  Vardesc rw_dv=define("rw","0",XFR_NUMBER);
  Vardesc aw_dv=define("aw","0",XFR_NUMBER);
  Vardesc wa_dv=define("wa","0",XFR_NUMBER);
  Vardesc r0w_dv=define("r0w","0",XFR_NUMBER);
  Vardesc vso_dv=define("vso","0",XFR_NUMBER);
  Vardesc rso_dv=define("rso","0",XFR_NUMBER);
  Vardesc rso0_dv=define("rso0","0",XFR_NUMBER);
  Vardesc aso_dv=define("aso","0",XFR_NUMBER);
  Vardesc vsoi_dv=define("vsoi","0",XFR_NUMBER);
  Vardesc rsoi_dv=define("rsoi","0",XFR_NUMBER);
  Vardesc asoi_dv=define("asoi","0",XFR_NUMBER);
  Vardesc wd_dv=define("wd","0",XFR_NUMBER);
  Vardesc wdr_dv=define("wdr","0",XFR_NUMBER);
  Vardesc wda_dv=define("wda","0",XFR_NUMBER);
  Vardesc wdr0_dv=define("wdr0","0",XFR_NUMBER);
  Vardesc awd_dv=define("awd","0",XFR_NUMBER);
  Vardesc defp_dv=define_array("defp","0",1,7,0,XFR_NUMBER);
  Vardesc deft_dv=define_array("deft","0",1,7,0,XFR_NUMBER);
  Vardesc vd_dv=define("vd","0",XFR_NUMBER);
  Vardesc vdr_dv=define("vdr","0",XFR_NUMBER);
  Vardesc vda_dv=define("vda","0",XFR_NUMBER);
  Vardesc nosub_dv=define("nosub","F",XFR_STRING);

   /*Variables for namelist: step */
  Vardesc step_ib_dv=define("ib","",XFR_NUMBER);
  Vardesc step_ia_dv=define("ia","",XFR_NUMBER);
  Vardesc step_k_dv=define("k","",XFR_NUMBER);
  Vardesc step_str_dv=define("str","",XFR_NUMBER);

  /*Variables for namelist: overlap */
  Vardesc kn1_dv=define("kn1","0",XFR_NUMBER);
  Vardesc kn2_dv=define("kn2","0",XFR_NUMBER);
  Vardesc ic1_dv=define("ic1","0",XFR_NUMBER);
  Vardesc ic2_dv=define("ic2","-1",XFR_NUMBER);
  Vardesc in_dv=define("in","0",XFR_NUMBER);
  Vardesc kind_dv=define("kind","0",XFR_NUMBER);
  Vardesc ch1_dv=define("ch1","0",XFR_STRING);
  Vardesc nn_dv=define("nn","0",XFR_NUMBER);
  Vardesc l_dv=define("l","0",XFR_NUMBER);
  Vardesc lmax_dv=define("lmax","0",XFR_NUMBER);
  Vardesc sn_dv=define("sn","0",XFR_NUMBER);
  Vardesc ia_dv=define("ia","0",XFR_NUMBER);
  Vardesc j_dv=define("j","0",XFR_NUMBER);
  Vardesc ib_dv=define("ib","0",XFR_NUMBER);
  Vardesc kbpot_dv=define("kbpot","0",XFR_NUMBER);
  Vardesc krpot_dv=define("krpot","0",XFR_NUMBER);
  Vardesc be_dv=define("be","0",XFR_NUMBER);
  Vardesc isc_dv=define("isc","0",XFR_NUMBER);
  Vardesc ipc_dv=define("ipc","0",XFR_NUMBER);
  Vardesc nfl_dv=define("nfl","0",XFR_NUMBER);
  Vardesc nam_dv=define("nam","0",XFR_NUMBER);
  Vardesc ampl_dv=define("ampl","0",XFR_NUMBER);
  Vardesc keep_dv=define("keep","F",XFR_STRING);
  Vardesc dm_dv=define("dm","0",XFR_NUMBER);
  Vardesc nk_dv=define("nk","0",XFR_NUMBER);
  Vardesc er_dv=define("er","0",XFR_NUMBER);
  Vardesc e_dv=define("e","0",XFR_NUMBER);

  /*NAMELIST COUPLING->See read_couplings function*/
 
   /*Structure with variables for namelist FRESCO*/
  Vardesc *fresco_vl[]={&hcm_dv,&rmatch_dv,&rintp_dv,
			&hnl_dv,&rnl_dv,&centre_dv,
			&hnn_dv,&rnn_dv,&rmin_dv,&rsp_dv,
			&rasym_dv,&accrcy_dv,&Switch_dv,&ajswtch_dv,
			&jtmin_dv,&jtmax_dv,
			&absend_dv,&dry_dv,&rela_dv,&nearfa_dv,
			&jump_dv,&jbord_dv,
			&kqmax_dv,&pp_dv,&thmin_dv,&thmax_dv,&thinc_dv,
			&koords_dv,&cutl_dv,&cutr_dv,
			&cutc_dv,&ips_dv,
			&smallchan_dv,&smallcoup_dv,&numnode_dv,
                        &sinjmax_dv,
                        &plane_dv,
			&it0_dv,&iter_dv,&iblock_dv,
			&nrbases_dv,&nrbmin_dv,&pralpha_dv,
			&pcon_dv,&meigs_dv,&rmatr_dv,&buttle_dv,
			&ebeta_dv,
			&pade_dv,
			&iso_dv,&nnu_dv,&maxl_dv,
			&minl_dv,&mtmin_dv,&epc_dv,
			&erange_dv,&dk_dv,&chans_dv,
			&listcc_dv,&treneg_dv,
			&cdetr_dv,&smats_dv,&xstabl_dv,&nlpl_dv,&waves_dv,
			&lampl_dv,&veff_dv,&kfus_dv,&nfus_dv,
			&wdisk_dv,&bpm_dv,&melfil_dv,&cdcc_dv,
			&pel_dv,&exl_dv,&lab_dv,
			&lin_dv,&lex_dv,&elab_dv,
			&nlab_dv,&fatal_dv,&nosol_dv,&nangle_dv,&pset_dv,
			&jset_dv,&inh_dv,&psiren_dv,&tmp_dv,
			&tmp_dv,&masfil_dv,&unitmass_dv,&finec_dv,
                        &plane_dv,&complexbins_dv};
   
 
  /*Structure with variables for namelist: PARTITION */
  Vardesc *partition_vl[]={&namep_dv,
			   &massp_dv,
			   &zp_dv,
			   &namet_dv,
			   &masst_dv,
			   &zt_dv,
			   &qval_dv,
			   &pwf1_dv,
			   &nex_dv,
                           &readstates_dv};
  
  /*Idem for namelist: STATES */
  Vardesc *states_vl[]={&jp_dv,
			&copyp_dv,
			&bandp_dv,
			&ep_dv,
			&kkp_dv,
			&tp_dv,
			&cpot_dv,
			&jt_dv,
			&copyt_dv,
			&bandt_dv,
			&et_dv,
			&kkt_dv,
			&tt_dv,
			&fexch_dv,
			&ignore_dv,
			&infam_dv,
			&outfam_dv,	
			&ptyp_dv,
			&ptyt_dv,
			&extra_dv,
			&pwf_dv,
			&spifl_dv};
			
  
   /*Idem for namelist: POT*/
   Vardesc *pot_vl[]={&kp_dv,&type_dv,&itt_dv,&shape_dv,&mnep_dv,
		      &mnet_dv,
		      &ap_dv,
		      &at_dv,&rc_dv,&ac_dv,&p_dv,&p0_dv,&p1_dv,&p2_dv,
		      &p3_dv,&p4_dv,&p5_dv,&p6_dv,
		      &p7_dv,&v_dv,&r0_dv,&rv_dv,&vr0_dv,&a_dv,&av_dv,
		      &w_dv,&wr0_dv,&rw_dv,&aw_dv,
		      &wa_dv,&r0w_dv,&vso_dv,&rso_dv,&rso0_dv,&aso_dv,
		      &vsoi_dv,&rsoi_dv,
		      &asoi_dv,&wd_dv,&wdr_dv,&wda_dv,&wdr0_dv,&awd_dv,
		      &defp_dv,&deft_dv,
		      &vd_dv,&vdr_dv,&vda_dv,&nosub_dv};
   
   /*Array of variables for namelist STEP*/
   Vardesc *step_vl[]={&step_ib_dv,
		       &step_ia_dv,
		       &step_k_dv,
		       &step_str_dv};

   /*Array of variables for namelist OVERLAP*/
   Vardesc *overlap_vl[]={&kn1_dv, &kn2_dv,&ic1_dv, &ic2_dv, &in_dv,
			  &kind_dv, &ch1_dv, &nn_dv,&l_dv,&lmax_dv,
			  &sn_dv,&ia_dv,&j_dv,&ib_dv,&kbpot_dv,&krpot_dv,
			  &be_dv,&isc_dv,&ipc_dv,&nfl_dv,&nam_dv,&ampl_dv,
			  &keep_dv,&dm_dv,&nk_dv,&er_dv};
   
 
   
  /*Namelist declarations: 77, 9 19, 48, 26 */
   Namelist fresco={"FRESCO",fresco_vl,sizeof(fresco_vl)/sizeof(&hcm_dv)};
   Namelist partition={"PARTITION",partition_vl,\
		       sizeof(partition_vl)/sizeof(&namep_dv)};
   Namelist states={"STATES",states_vl,sizeof(states_vl)/sizeof(&jp_dv)};
   Namelist pot={"POT",pot_vl,sizeof(pot_vl)/sizeof(&kp_dv)};
   Namelist step={"STEP",step_vl,sizeof(step_vl)/sizeof(&step_ib_dv)};
   Namelist overlap={"OVERLAP",overlap_vl,\
		     sizeof(overlap_vl)/sizeof(&kn1_dv)};
 
   GString *ghead=g_string_new("");
   gint c;

  /********************************************************************/
  /********** END OF DECLARATIONS: START READING NAMELISTS ************/
  /*          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~            */

   /* Set number of warnings to 0 */
   nwarnings=0;

   
  if ((f=fopen(filename,"r"))==NULL){
    glade_util_show_message_box ("Error opening file");
    g_warning("Unable to open file '%s'!",filename);
    return(-1);
  }
  else{
    gchar *buffer=g_strdup(filename);
    g_strreverse(buffer);
    buffer=g_strndup(buffer,6);
    g_strreverse(buffer);
  
    if(strcmp(buffer,".empty")==0)
      current_filename=g_strdup("newfile");
    else
      current_filename=g_strdup(filename);
    }

   xfr_print("Setting filename to %s\n",current_filename);

  contid=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),
				       "openfile");

/*   buffer=g_strdup_printf("Opening file='%s'",current_filename); */
/*   gtk_statusbar_push( GTK_STATUSBAR(statusbar), contid, buffer); */

  gtk_statusbar_push( GTK_STATUSBAR(statusbar), contid, \
		      g_strdup_printf("Opening file='%s'",current_filename));
  gtk_window_set_title(GTK_WINDOW(main_window),
		       g_strdup_printf("Xfresco: %s",current_filename));


  /* HEADING */
  while ((c=fgetc(f))!='\n'){
    g_string_append_printf(ghead,"%c",c);
    if (c==EOF) {
      g_warning("Unexpected end of file!");
      return(-1);
    }
  }
  /* window_constants=lookup_widget(main_window,"window_constants"); */
  xfr_print("Heading:%s\n",ghead->str);
  /* gtk_entry_set_text(GTK_ENTRY(lookup_widget(window_constants,"entry_heading")),
     ghead->str);*/

  gtk_editable_set_text(GTK_EDITABLE(lookup_widget(main_window,"entry_heading")),
		      ghead->str);


  /******************************************************************/
  /*                          FRESCO NAMELIST                       */
  /******************************************************************/
  if (readnl(&fresco,f)==LIST_NOT_FOUND) return(LIST_NOT_FOUND);

  xfr_set_text(main_window,"entry_hcm",&hcm_dv);
  /* For rmatch, displays absolute value. Minus sign means only that 
   * CCWF will be read */
  gtk_editable_set_text(GTK_EDITABLE(lookup_widget(main_window,"entry_rmatch")),
		     abs_valof(&rmatch_dv));
  xfr_set_text(main_window,"entry_rintp",&rintp_dv);
  xfr_set_text(main_window,"entry_hnl",&hnl_dv);
  xfr_set_text(main_window,"entry_rnl",&rnl_dv); 
  xfr_set_text(main_window,"entry_centre",&centre_dv);
  xfr_set_text(main_window,"entry_hnn",&hnn_dv);
  xfr_set_text(main_window,"entry_rnn",&rnn_dv);
  xfr_set_text(main_window,"entry_rmin",&rmin_dv);  
  xfr_set_text(main_window,"entry_rsp",&rsp_dv); 
  xfr_set_text(main_window,"entry_jtmin",&jtmin_dv);  
  xfr_set_text(main_window,"entry_jtmax",&jtmax_dv);
  xfr_set_text(main_window,"spin_pel",&pel_dv);
  xfr_set_text(main_window,"spin_exl",&exl_dv);
  xfr_set_text(main_window,"spin_lab",&lab_dv);
  xfr_set_text(main_window,"spin_lex",&lex_dv);
  /* xfr_set_text(main_window,"entry_elab",&elab_dv); */


  /* if (window_eintervals == NULL){ */
/*     window_eintervals = create_window_eintervals (); */
/*     g_object_set_data(G_OBJECT(main_window),\ */
/* 			"window_eintervals",window_eintervals); */
/*   } */

  /* We save a pointer to the main window inside the window_eintervals's
     data list, so we can get it easily in the callbacks. */
 /*  g_object_set_data (GTK_OBJECT (window_eintervals), "main_window", main_window); */

  /* ELAB */

/*   if (window_eintervals!=NULL){ */
/*     xfr_set_text_n(window_eintervals,"elab1",&elab_dv,0); */
/*     xfr_set_text_n(window_eintervals,"elab2",&elab_dv,1); */
/*     xfr_set_text_n(window_eintervals,"elab2b",&elab_dv,1); */
/*     xfr_set_text_n(window_eintervals,"elab3",&elab_dv,2); */
/*     xfr_set_text_n(window_eintervals,"elab3b",&elab_dv,2); */
/*     xfr_set_text_n(window_eintervals,"elab4",&elab_dv,3); */
/*     xfr_set_text_n(window_eintervals,"nlab1",&nlab_dv,0); */
/*     xfr_set_text_n(window_eintervals,"nlab2",&nlab_dv,1); */
/*     xfr_set_text_n(window_eintervals,"nlab3",&nlab_dv,2); */
    
/*   }else{ */
/*     g_print("**ERROR*: window_eintervals=NULL\n"); */
/*   } */
  


/*   for (i=1;i<4;i++){ */
/*      gchar *elab_name=g_strdup_printf("elab%i",i); */
/*      gchar *nlab_name=g_strdup_printf("nlab%i",i); */
/*      xfr_set_text_n(main_window,elab_name,&elab_dv,i-1);  */
/*      xfr_set_text_n(main_window,nlab_name,&nlab_dv,i-1);  */
/*     } */
     
/*   xfr_set_text_n(main_window,"elab",&elab_dv,0);*/

  widget=lookup_widget(main_window,"elab");
/*   handler_id=g_signal_handler_find(G_OBJECT(widget), */
/* 				   G_SIGNAL_MATCH_FUNC,0,0,NULL, */
/* 				   on_entry_elab_changed,NULL); */
/*   g_signal_handler_block (G_OBJECT(widget),handler_id); */

  g_signal_handlers_block_by_func (GTK_EDITABLE(widget), on_entry_elab_changed, NULL);
  xfr_set_text_n(main_window,"elab",&elab_dv,0);
  g_signal_handlers_unblock_by_func (GTK_EDITABLE(widget), on_entry_elab_changed, NULL);
  
  widget=lookup_widget(main_window,"elab1");
  g_signal_handlers_block_by_func (GTK_EDITABLE(widget), on_elab1_changed, NULL);
  xfr_set_text_n(main_window,"elab1",&elab_dv,0);
  g_signal_handlers_unblock_by_func (widget, on_elab1_changed, NULL);

  /* g_signal_handler_unblock (G_OBJECT(widget),handler_id);*/

   xfr_set_text_n(main_window,"nlab1",&nlab_dv,0);
   xfr_set_text_n(main_window,"elab2",&elab_dv,1);
   xfr_set_text_n(main_window,"nlab2",&nlab_dv,1);
   xfr_set_text_n(main_window,"elab3",&elab_dv,2);
   xfr_set_text_n(main_window,"nlab3",&nlab_dv,2);
   xfr_set_text_n(main_window,"elab4",&elab_dv,3);


  /*If rmatch<0 or rasym!=0 =>Coupled Coulomb wave functions*/
  if (ivalof(&rmatch_dv)<0 || atoi(abs_valof(&rasym_dv))>0){
    GtkWidget *window_ccwf=lookup_widget(main_window,"window_ccwf");
    GtkWidget *use_ccwf=lookup_widget(main_window,"check_use_ccwf");
    GtkWidget *check_rasym=lookup_widget(window_ccwf,"check_rasym");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(use_ccwf),TRUE);
    xfr_set_text(window_ccwf,"entry_rasym",&rasym_dv);

    if (atof(valof(&rasym_dv))<0){
      xfr_toggle_set_value(check_rasym,"TRUE");
    }
    else{ 
      xfr_toggle_set_value(check_rasym,"FALSE");
    }; 
    
    xfr_set_text(window_ccwf,"entry_accrcy",&accrcy_dv);  
    xfr_set_text(window_ccwf,"entry_switch",&Switch_dv);
    xfr_set_text(window_ccwf,"entry_ajswtch",&ajswtch_dv);
  }
  /* gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(use_ccwf),FALSE);*/

  /*LIN*/
  /* GtkOptionMenu removed in GTK-4 - use GtkDropDown instead */
  /* if(ivalof(&lin_dv)==1){
    GtkOptionMenu *opt_lin=GTK_OPTION_MENU(lookup_widget(main_window,"opt_lin"));
    gtk_option_menu_set_history(opt_lin,0);
  }
  else if (ivalof(&lin_dv)==2){
    GtkOptionMenu *opt_lin=GTK_OPTION_MENU(lookup_widget(main_window,"opt_lin"));
    gtk_option_menu_set_history(opt_lin,1);
  } */


  /*Absend*/
  gtk_toggle_button_set_active\
    (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_absend")),\
     atof(valof(&absend_dv))>0?1:0);

  gtk_editable_set_text(GTK_EDITABLE(lookup_widget(main_window,"entry_absend")),\
		     abs_valof(&absend_dv));

  /* Dry run */
  xfr_toggle_set_value(lookup_widget(main_window,"check_dry"),valof(&dry_dv));

  /*Near side /far side control*/
  /* GtkOptionMenu removed in GTK-4 - use GtkDropDown instead */
  /* if (ivalof(&nearfa_dv)<0){
    gtk_option_menu_set_history\
      (GTK_OPTION_MENU(lookup_widget(main_window,"opt_nearfa1")),1);
  }
  else{
    gtk_option_menu_set_history\
      (GTK_OPTION_MENU(lookup_widget(main_window,"opt_nearfa1")),0);
  }

  switch(ivalof(&nearfa_dv)){
  case 0: case 1:
    gtk_option_menu_set_history\
      (GTK_OPTION_MENU(lookup_widget(main_window,"opt_nearfa2")),0);break;
  case 2: case -2:
     gtk_option_menu_set_history\
      (GTK_OPTION_MENU(lookup_widget(main_window,"opt_nearfa2")),1);break;
  case 3: case -3:
     gtk_option_menu_set_history\
       (GTK_OPTION_MENU(lookup_widget(main_window,"opt_nearfa2")),2);break;
  } */


  /*
  switch (absivalof(&nearfa_dv)){
  case 0:case 1:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_usualcs")),TRUE);
    break;
  case 2:case -2:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_fs")),TRUE);
    break;
  case 3:case -3:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_fsns")),TRUE);
    break;
  default:
    g_print("Incorrect nearfa number:%s",valof(&nearfa_dv));break;
  }
  
  */

  xfr_set_text(main_window,"spin_kqmax",&kqmax_dv);
  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history\
    (GTK_OPTION_MENU(lookup_widget(main_window,"opt_pp")),atoi(valof(&pp_dv))); */
  xfr_set_text(main_window,"spin_thmin",&thmin_dv);
  xfr_set_text(main_window,"spin_thmax",&thmax_dv);
  xfr_set_text(main_window,"spin_thinc",&thinc_dv);

  /*opt_inh*/
  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history\
    (GTK_OPTION_MENU(lookup_widget(main_window,"opt_inh")),atoi(valof(&inh_dv))); */
 


  /*Coordinate system (koords) OLD
  switch (atoi(koords)){
  case 0:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_M")),TRUE);
    break;
  case 1:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_MT")),TRUE);
    break;
  case 2:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_MTR")),TRUE);
    break;
  case 3:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_MTRHJ")),TRUE);
    break;
  default:
    g_print("Invalid value for variable KOORDS:%s",koords);
    break;
  }
  */
  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history\
    (GTK_OPTION_MENU(lookup_widget(main_window,"opt_koords")),ivalof(&koords_dv)); */

  /* gtk_option_menu_set_history\
    (GTK_OPTION_MENU(lookup_widget(main_window,"opt_plane")),ivalof(&plane_dv)); */


  xfr_set_text(main_window,"entry_smallchan",&smallchan_dv);  
  xfr_set_text(main_window,"entry_smallcoup",&smallcoup_dv);  
  xfr_set_text(main_window,"entry_numnode",&numnode_dv);  


  xfr_set_text(main_window,"entry_cutl",&cutl_dv);  
  xfr_set_text(main_window,"entry_cutr",&cutr_dv);
  xfr_set_text(main_window,"entry_cutc",&cutc_dv);

  toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_num_errors"));

  i=ivalof(&ips_dv);
  widget=lookup_widget(main_window,"entry_ips");
  if (i>0){
    gtk_toggle_button_set_active(toggle,TRUE);
    gtk_editable_set_text(GTK_EDITABLE(widget),valof(&ips_dv));
  }
  else{  
    gtk_toggle_button_set_active(toggle,FALSE);
    gtk_editable_set_text(GTK_EDITABLE(widget),abs_valof(&ips_dv));
  }

  /*Iterations*/
  gtk_spin_button_set_value\
    (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_it0")),ivalof(&it0_dv));
  gtk_spin_button_set_value\
    (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_iter")),ivalof(&iter_dv));
  xfr_set_text(main_window,"spin_it0",&it0_dv);
  /* does not work */
  /*  xfr_set_text(main_window,"spin_iblock",&iblock_dv); */
  /* g_print("iblock=%s",valof(&iblock_dv)); */

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_iblock")),
			    ivalof(&iblock_dv));


  xfr_toggle_set_value(lookup_widget(main_window,"check_fatal"),valof(&fatal_dv));
  xfr_toggle_set_value(lookup_widget(main_window,"check_nosol"),valof(&nosol_dv));
  xfr_set_text(main_window,"entry_jset",&jset_dv); 

  toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_pset"));
  switch (ivalof(&pset_dv)){
  case 1:case -1:
    gtk_toggle_button_set_active(toggle,TRUE);
    toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"toggle_pset"));
    if (ivalof(&pset_dv)==1)
      gtk_toggle_button_set_active(toggle,FALSE);
    else
      gtk_toggle_button_set_active(toggle,TRUE);
    break;
  case 0:
    gtk_toggle_button_set_active(toggle,FALSE);
    break;    
  }

  /*Pade*/
  /*
  switch (atoi(pade)){
  case 0:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_pade0")),TRUE);break;
  case 1:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_pade1")),TRUE);break;
  case 2:
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_pade2")),TRUE);break;
  default:
    g_print("Invalid value for variable PADE:%s",pade);
  }
  */

  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history(GTK_OPTION_MENU(lookup_widget(main_window,"opt_pade")),
			     ivalof(&pade_dv)); */

  /*Check button "check_psiren"*/
  xfr_toggle_set_value(lookup_widget(main_window,"check_psiren"),valof(&psiren_dv));

  /*ISOcentrifugal approximations*/
  i=0; /* default */
  if (g_strcasecmp(valof(&iso_dv),"A")==0 ||
      g_strcasecmp(valof(&iso_dv),"J")==0) i=1;
  if (g_strcasecmp(valof(&iso_dv),"B")==0 ||
      g_strcasecmp(valof(&iso_dv),"L")==0) i=2;
  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history(GTK_OPTION_MENU(lookup_widget(main_window,"opt_iso")),i); */

  /*Gaussian integration*/
  xfr_set_text(main_window,"spin_nnu",&nnu_dv);
  xfr_set_text(main_window,"entry_epc",&epc_dv);

  /*L range for NL kernels*/
  xfr_set_text(main_window,"entry_maxl",&maxl_dv);  
  xfr_set_text(main_window,"entry_minl",&minl_dv);
  if (ivalof(&mtmin_dv)<0){
    GtkWidget *check_mtmin=lookup_widget(main_window,"check_mtmin");
    gchar *buffer=g_strdup_printf("%i",abs(ivalof(&mtmin_dv)));
    xfr_toggle_set_value(check_mtmin,"TRUE");
    /* buffer=g_strdup_printf("%i",abs(ivalof(&mtmin_dv)));*/
    assign(&mtmin_dv,buffer);    
  }
  xfr_set_text(main_window,"entry_mtmin",&mtmin_dv);

  
  /*We use the same window_bins widget each time, so first
    of all we create it if it hasn't already been created. */
  /* if (window_bins == NULL)
     window_bins = create_window_bins (); */

  /* erange y dk*/
  /* xfr_set_text(window_bins,"entry_erange",&erange_dv);
     xfr_set_text(window_bins,"entry_dk",&dk_dv);*/
  
  xfr_set_text(main_window,"entry_erange",&erange_dv);
  xfr_set_text(main_window,"entry_dk",&dk_dv);



  /*xfr_set_text(window_bins,"over_er",&er_dv);*/
  /*xfr_set_text(window_bins,"over_nk",&nk_dv);*/
  /*xfr_set_text(window_bins,"over_isc",&isc_dv);*/
  
  /*TRACE options*/
  xfr_set_text(main_window,"spin_treneg",&treneg_dv);  

  /*CHANS*/
  gtk_spin_button_set_value\
    (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_chans")),ivalof(&chans_dv));
   
  /*WDISK*/
  if (abs(ivalof(&wdisk_dv))==1 || abs(ivalof(&wdisk_dv))==2)
    {
      gtk_toggle_button_set_active\
	(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_wdisk")),TRUE);
      switch (abs(ivalof(&wdisk_dv))){
      case 1:
	gtk_toggle_button_set_active\
	  (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_wfelastic")),TRUE);
	break;
	
      case 2:
	gtk_toggle_button_set_active\
	  (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_wfall")),TRUE);
	break;
      }
      if (ivalof(&wdisk_dv)>0){
	gtk_toggle_button_set_active\
	  (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_file17formatted")),TRUE);
      }
      else{
	gtk_toggle_button_set_active\
	  (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_file17formatted")),TRUE);
      }
    }
  else{
    /*Desactivar los controles conrrespondientes...*/
  }
  
  /*LISTCC*/
  gtk_spin_button_set_value\
    (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_listcc")),ivalof(&listcc_dv));
  
  /*WAVES*/
  switch (ivalof(&waves_dv))
    {
    case 1:iwaves=1;break;
    case 2:iwaves=2;break;
    case 3:iwaves=3;break;
    case -1:case -3:iwaves=4;break;
    default:iwaves=0;break;
    }
  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history\
    (GTK_OPTION_MENU(lookup_widget(main_window,"opt_waves")),iwaves); */
  
  /*TRENEG*/
  gtk_spin_button_set_value\
    (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_treneg")),ivalof(&treneg_dv));
  
  /*CDETR*/
  gtk_spin_button_set_value\
    (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_cdetr")),ivalof(&cdetr_dv));
  
  /*SMATS*/
  xfr_combo_set_value(main_window,"combo_smats",ivalof(&smats_dv));
   
  /*NLPL-Contour plot for NL kernels*/
  gtk_spin_button_set_value\
    (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_nlpl")),ivalof(&nlpl_dv));
  
  /*LAMP*/
  /* Needs partitions, so see below*/

  /*
  gtk_widget_set_sensitive(lookup_widget(main_window,"combo_lampl"),
			   ivalof(&lampl_dv)==0?FALSE:TRUE);
			   */

  /*VEFF*/
  if (ivalof(&veff_dv)!=0){
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_veff")),TRUE);
    if (ivalof(&veff_dv)<0){
      gtk_toggle_button_set_active\
	(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_veff1")),TRUE);
    }
    if (abs(ivalof(&veff_dv))==2){
      toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_veff2"));
      gtk_toggle_button_set_active(toggle,TRUE);
    }
  }
  else{
    toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_veff"));
    gtk_toggle_button_set_active(toggle,FALSE);
  }
  
  /* XSTABL */
  if (ivalof(&xstabl_dv)!=0){
    /* gtk_toggle_button_set_active\
       (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_xstabl")),TRUE);*/
    gtk_spin_button_set_value\
      (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_xstabl")),ivalof(&xstabl_dv));
  }
  
  /* KFUS, NFUS */
  if (ivalof(&kfus_dv)>0){
    gtk_spin_button_set_value\
      (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_kfus")),ivalof(&kfus_dv));
    gtk_spin_button_set_value\
      (GTK_SPIN_BUTTON(lookup_widget(main_window,"spin_nfus")),ivalof(&nfus_dv));
    
  }
  
  /* BPM */
  if (ivalof(&bpm_dv)>0){
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_bpm")),TRUE);
  }
  if (ivalof(&bpm_dv)>1){
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_ldistrib")),TRUE);
  }
  
  /* MELFIL */
  switch(i=ivalof(&melfil_dv)){
  case 0: case 1:case 2:
    /* GtkOptionMenu removed in GTK-4 */
    /* gtk_option_menu_set_history\
      (GTK_OPTION_MENU(lookup_widget(main_window,"opt_melfil")),i); */
    break;
  default:
    g_print("\nWARNING**: Unexpected value for MELFIL=%i\n",i);
    nwarnings+=1;
    break;
  }

  /* CDCC */
  if (ivalof(&cdcc_dv)!=0)
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_cdcc")),TRUE);
  else
    gtk_toggle_button_set_active\
      (GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_cdcc")),FALSE);

  /*General constants*/
  xfr_set_text(main_window,"entry_tmp",&tmp_dv);  
  xfr_set_text(main_window,"entry_masfil",&masfil_dv);  
  xfr_set_text(main_window,"entry_unitmass",&unitmass_dv);
  xfr_set_text(main_window,"entry_finec",&finec_dv);

  /*JBORD*/
  window_jbord=lookup_widget(main_window,"window_jbord");
  xfr_set_text(window_jbord,"entry_jtmin",&jtmin_dv);  
  xfr_set_text(window_jbord,"entry_jtmax",&jtmax_dv);   
 
  /* Umcommented 14/11/2003 */
  for (i=2;i<6;i++){
    gchar *jbord_name=g_strdup_printf("jbord%i",i);
    gchar *jump_name=g_strdup_printf("jump%i",i);
    GtkWidget *jbord=lookup_widget(window_jbord,jbord_name);
    GtkWidget *jump=lookup_widget(window_jbord,jump_name);
    gtk_editable_set_text(GTK_EDITABLE(jbord),valof_n(&jbord_dv,i-2));
    gtk_editable_set_text(GTK_EDITABLE(jump),valof_n(&jump_dv,i-2));
  }



  
  /* R-matrix */
  window_Rmatrix=lookup_widget(main_window,"window_Rmatrix");
  xfr_set_text(window_Rmatrix,"spin_nrbases",&nrbases_dv);
  xfr_set_text(window_Rmatrix,"spin_nrbmin",&nrbases_dv);
  widget=lookup_widget(window_Rmatrix,"opt_pcon");
  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history(GTK_OPTION_MENU(widget),\ */
  /* ivalof(&pcon_dv)); */
  widget=lookup_widget(window_Rmatrix,"opt_buttle");
  /* GtkOptionMenu removed in GTK-4 */
  /* gtk_option_menu_set_history(GTK_OPTION_MENU(widget),\ */
  /* ivalof(&buttle_dv)); */
  widget=lookup_widget(window_Rmatrix,"check_pralpha");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),
			       strcmp(valof(&pralpha_dv),"T")==0? TRUE:FALSE);
    
  xfr_set_text(window_Rmatrix,"spin_meigs",&meigs_dv);
  xfr_set_text(window_Rmatrix,"entry_rmatr",&rmatr_dv);
  xfr_set_text(window_Rmatrix,"entry_ebeta",&ebeta_dv);
  


  xfr_print("\nNamelist fresco read ok..\n");
  
  
  /************************************************************************
  **                          READ PARTITIONS                           **
  ************************************************************************/
 
  if (part_clist==NULL){
    g_print("openfile: part_clist=NULL!\n");
  }
  gtk_clist_clear(GTK_CLIST(part_clist));
  gtk_clist_clear(GTK_CLIST(states_clist));

 
  for(i=readnl(&partition,f);namep_dv.read==TRUE;i=readnl(&partition,f)){
    GtkWidget *data_clist;
    gint nfile;
    FILE *g;

    if (namep_dv.read==FALSE) g_print("EMPTY PARTITION!!!!!!! \n");
    if (i==LIST_NOT_FOUND) return(LIST_NOT_FOUND);

    inex=ivalof(&nex_dv);

    if (inex<0)
      assign(&nex_dv,"F");
    else
      assign(&nex_dv,"T");

    part_number++;
    put_partition(part_clist,&partition);
    data_clist=gtk_clist_new(COLUMNS_STATES);


    nfile=ivalof(&readstates_dv);
    if (nfile>0){
      gchar *dirname=g_path_get_dirname(current_filename);
      gchar *filestat=g_strdup_printf("fort.%i",nfile);
      /*      gchar *fullfilestat=g_strconcat(dirname,"/",filestat);*/
      gchar *fullfilestat=g_strdup_printf("%s/%s",dirname,filestat);
    
      g_print("\n Reading states from file %s",fullfilestat);
      if ((g=fopen(fullfilestat,"r"))==NULL){
	g_warning("Unable to read states from file '%s'!",filestat);
	return(-1);
      }
      else{
	/* PROVISIONAL */
	inex=10;
	fscanf(g,"%i",&inex);
	xfr_print("\nReading %i state(s) for partition %i from unit %i\n",inex,part_number,nfile);
      }
    }
    /*Read "states namelist"*/
    xfr_print("\nReading %i state(s) for partition %i:\n",inex,part_number);
    for (i=0;i<abs(inex);i++){
      if (nfile==0){
	if (readnl(&states,f)==LIST_NOT_FOUND) break;
      }
      else{
	if (readnl(&states,g)==LIST_NOT_FOUND) break;
      }
      put_states(data_clist,&states);
      /*Reset states namelist*/
      resetnl(&states);
    }

    g_return_val_if_fail(data_clist!=NULL,-1);
    g_return_val_if_fail(GTK_IS_CLIST(data_clist),-1);
 

    /*Attach data_clist to the row of part_clist containing their partition*/
    gtk_clist_set_row_data(GTK_CLIST(part_clist),part_number-1,data_clist);

    /*Reset partition namelist*/
    resetnl(&partition);
  }

  /*Select first row */
  if (xfr_clist_get_rows(part_clist)>0){
    gtk_clist_select_row(GTK_CLIST(part_clist),0,0);
  }
  
  /* Refresh combo_lampl, combo_ic1, combo_ic2 ...containing the list 
   * of partitions with a brief
   * description for each partition */
    
    
    /* if (get_partitions_list(GTK_CLIST(part_clist))==NULL)
     * g_print("\nWARNING!!: get_partitions_list returned NULL!!\n"); */
  if (part_clist  && get_partitions_list(GTK_CLIST(part_clist)))
    /* GtkCombo removed in GTK-4 */

    /* gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,
						     "combo_lampl")),
				  get_partitions_list(GTK_CLIST(part_clist))); */
  

  /*LAMPL*/
  widget=lookup_widget(main_window,"opt_lampl");
  if (ivalof(&lampl_dv)==0){
    /* GtkOptionMenu removed in GTK-4 */
    /* gtk_option_menu_set_history(GTK_OPTION_MENU(widget),0); */
  }
  else{
    /* xfr_combo_set_value(main_window,"combo_lampl",abs(ivalof(&lampl_dv))-1); */
    if (ivalof(&lampl_dv)>0){
      /* GtkOptionMenu removed in GTK-4 */
      /* gtk_option_menu_set_history(GTK_OPTION_MENU(widget),2); */
    }
    else{
      /* gtk_option_menu_set_history(GTK_OPTION_MENU(widget),1); */
    }
  }

  if (part_clist && get_partitions_list(GTK_CLIST(part_clist))){
    /* GtkCombo removed in GTK-4 */

    /* gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,\
						       "combo_ic1")),
				  get_partitions_list(GTK_CLIST(part_clist))); */
    /* GtkCombo removed in GTK-4 */

    /* gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,\
						       "combo_ic2")),
				  get_partitions_list(GTK_CLIST(part_clist))); */
    /* GtkCombo removed in GTK-4 */

    /* gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,
						       "combo_ictfrom")),
				  get_partitions_list(GTK_CLIST(part_clist))); */
    /* GtkCombo removed in GTK-4 */

    /* gtk_combo_set_popdown_strings(GTK_COMBO(lookup_widget(main_window,\
						       "combo_icto")),
				  get_partitions_list(GTK_CLIST(part_clist))); */
    
  }
  xfr_print("Partitions read ok...\n");


  /******************************************************************/      
  /*                        READ POTENTIALS                          */
  /******************************************************************/
  pot_clist=lookup_widget(main_window,"pot_clist");
  /* gtk_clist_freeze removed in GTK-4 */

  /* gtk_clist_freeze(GTK_CLIST(pot_clist)); */
  gtk_clist_clear(GTK_CLIST(pot_clist));

  while((iread=readnl(&pot,f))!=EMPTY_LIST){    
    enum{KP,TYPE,SHAPE,ITT,P1,P2,P3,P4,P5,P6,P7,ADD};
    gchar *row[COLUMNS_POT];
    gint i;
    gint kp=ivalof(&kp_dv);
    if (iread==LIST_NOT_FOUND) return(LIST_NOT_FOUND);

    /* Reset row*/
    for(i=0;i<COLUMNS_POT;i++) row[i]=g_strdup("");

    if (type_dv.read==FALSE){
      /* Type not specified->combined namelist */
      xfr_print("->combined namelist");       
      
      /*Coulomb*/
      row[KP]=g_strdup(abs_valof(&kp_dv));
      row[TYPE]=g_strdup("0");
      row[SHAPE]=g_strdup(valof(&shape_dv));
      row[ITT]=g_strdup(valof(&itt_dv));
      row[P1]=g_strdup(valof(&ap_dv));
      row[P2]=g_strdup(valof(&at_dv));
      row[P3]=g_strdup(valof(&rc_dv));
      row[P4]=g_strdup(valof(&ac_dv));
      gtk_clist_append(GTK_CLIST(pot_clist),row);
      pot_number++;
      
      /*Coulomb deformation for projectile*/
      if (mnep_dv.read==TRUE){
	gint j;
	gint jmax=(mnep_dv.garray)->len;
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("10"); /* ¿ 10 o 12 ? */
	row[SHAPE]=g_strdup(valof(&shape_dv));
	row[ITT]=g_strdup(valof(&itt_dv));
	
	for (j=0;j<jmax;j++) row[P1+j]=g_strdup(valof_n(&mnep_dv,j));
	
	gtk_clist_append(GTK_CLIST(pot_clist),row);
	pot_number++;
	xfr_print("Coulomb deformation for projectile read");
      }
      
      /* Coulomb deformation for target */
      if (mnet_dv.read==TRUE){
	gint j;
	gint jmax=(mnep_dv.garray)->len;
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("11"); /* Type=11 */
	row[SHAPE]=g_strdup(valof(&shape_dv));
	row[ITT]=g_strdup(valof(&itt_dv));
	for (j=0;j<jmax;j++) row[P1+j]=g_strdup(valof_n(&mnet_dv,j));
	gtk_clist_append(GTK_CLIST(pot_clist),row);
	pot_number++; 
	xfr_print("Coulomb deformation for target read");
      }
      
      /*Nuclear:volume*/
      if(ivalof(&v_dv)!=0 || ivalof(&w_dv)!=0){
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("1");
	row[SHAPE]=g_strdup(valof(&shape_dv));
	row[ITT]=g_strdup(valof(&itt_dv));
	row[P1]=g_strdup(valof(&v_dv));
	row[P2]=g_strdup(valof(&rv_dv));
	row[P3]=g_strdup(valof(&av_dv));
	row[P4]=g_strdup(valof(&w_dv));
	row[P5]=g_strdup(valof(&rw_dv));
	row[P6]=g_strdup(valof(&aw_dv));
	gtk_clist_append(GTK_CLIST(pot_clist),row);
	
      }
      
      /*Nuclear:surface*/
      if(ivalof(&wd_dv)!=0 || ivalof(&vd_dv)!=0){ 
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("2");
	row[SHAPE]=g_strdup(valof(&shape_dv));
	row[ITT]=g_strdup(valof(&itt_dv));
	row[P1]=g_strdup(valof(&wd_dv));
	row[P2]=g_strdup(valof(&wdr_dv));
	row[P3]=g_strdup(valof(&wda_dv));
	row[P4]=g_strdup(valof(&vd_dv));
	row[P5]=g_strdup(valof(&vdr_dv));
	row[P6]=g_strdup(valof(&vda_dv));
	gtk_clist_append(GTK_CLIST(pot_clist),row);
	
	pot_number++;
      }
      /*Nuclear:spin-orbit*/
      if(ivalof(&vso_dv)!=0 ||ivalof(&rso_dv)!=0){ 
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("3"); /*REPASAR ESTE: 3 O 4 ?*/
	row[SHAPE]=g_strdup(valof(&shape_dv));
	row[ITT]=g_strdup(valof(&itt_dv));
	row[P1]=g_strdup(valof(&vso_dv));
	row[P2]=g_strdup(valof(&rso_dv));
	row[P3]=g_strdup(valof(&aso_dv));
	row[P4]=g_strdup(valof(&vsoi_dv));
	row[P5]=g_strdup(valof(&rsoi_dv));
	row[P6]=g_strdup(valof(&asoi_dv));
	gtk_clist_append(GTK_CLIST(pot_clist),row);    
	pot_number++;      
      }
      
      /*Nuclear deformation for projectile*/
      if (defp_dv.read==TRUE){
	gint j;
	gint jmax=(defp_dv.garray)->len;
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("10"); /* Type=10 */
	row[SHAPE]=g_strdup(valof(&shape_dv));
	row[ITT]=g_strdup(valof(&itt_dv));
	for (j=0;j<jmax;j++) row[P1+j]=g_strdup(valof_n(&defp_dv,j));
	gtk_clist_append(GTK_CLIST(pot_clist),row);
	pot_number++;      
      }
      
      /*Nuclear deformation for target*/
      if (deft_dv.read==TRUE){
	gint j;
	gint jmax=(deft_dv.garray)->len;
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("11"); /* Type=11 */
	row[SHAPE]=g_strdup(valof(&shape_dv));
	row[ITT]=g_strdup(valof(&itt_dv));
	for (j=0;j<jmax;j++) row[P1+j]=g_strdup(valof_n(&deft_dv,j));
	gtk_clist_append(GTK_CLIST(pot_clist),row);
	pot_number++;      
      }
    }
    else{

      switch(ivalof(&type_dv)){
	/*Probablemente este caso lo puedo quitar y englobarlo en default*/
      case 0:  /*Coulomb potential*/
	xfr_print("->Coulomb potential");
	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup("0");
	row[SHAPE]=g_strdup("");
	row[ITT]=g_strdup(valof(&itt_dv));
	if (p_dv.read==FALSE && def_dv.read==FALSE){
	  row[P1]=g_strdup(valof(&ap_dv));
	  row[P2]=g_strdup(valof(&at_dv));
	  row[P3]=g_strdup(valof(&rc_dv));
	  row[P4]=g_strdup(valof(&ac_dv));
	}
	else{
	  /* dimensioned variables p(1:7)*/
	  gint j,jmin,jmax,dim;
	  jmin=(p_dv.start);
	  jmax=(p_dv.end);
	  dim=jmax-jmin+1;
	  /*xfr_print("jmin,jmax:%i %i %i ",jmin,jmax);
	    xfr_print("Voy a poner %i valores\n",dim);*/
	  for (j=0;j<dim;j++){
	    row[P1+j]=g_strdup(valof_n(&p_dv,j));
	  }
	}
	break;
	
      default:
	xfr_print("->single namelist");
	if (ivalof(&type_dv)<0)
	  row[ADD]=g_strdup("T");
	else
	  row[ADD]=g_strdup("F");

	row[KP]=g_strdup(abs_valof(&kp_dv));
	row[TYPE]=g_strdup(abs_valof(&type_dv));
	if ((abs(ivalof(&type_dv))==12 || abs(ivalof(&type_dv))==13) \
	    && ivalof(&shape_dv)==0){
	  row[SHAPE]=g_strdup("10");
	}
	else{
	  row[SHAPE]=g_strdup(valof(&shape_dv));
	}
	row[ITT]=g_strdup(valof(&itt_dv));
				  
	if (p_dv.read==FALSE && def_dv.read==FALSE){
	  row[P1]=g_strdup(valof(&p1_dv));
	  row[P2]=g_strdup(valof(&p2_dv));
	  row[P3]=g_strdup(valof(&p3_dv));
	  row[P4]=g_strdup(valof(&p4_dv));
	  row[P5]=g_strdup(valof(&p5_dv));
	  row[P6]=g_strdup(valof(&p6_dv));
	  row[P7]=g_strdup(valof(&p7_dv));	  
	}
	else{
	  Vardesc var_dv;
	  gint j,jmin,jmax,dim;
	  /* read p(:) / def(:) */
	  if (p_dv.read==TRUE) var_dv=p_dv;
	  else var_dv=def_dv;
	  
	  jmin=(var_dv.start);
	  jmax=(var_dv.end);
	  dim=jmax-jmin+1;
	  /*xfr_print("jmin,jmax:%i %i %i ",jmin,jmax);
	    xfr_print("Voy a poner %i valores\n",dim);*/
	  for (j=0;j<dim;j++){
	    row[P1+j]=g_strdup(valof_n(&var_dv,j));
	  }
	}
	break;
      }
      gtk_clist_append(GTK_CLIST(pot_clist),row);
      pot_number++;
    }
	
   
    
    /*For type 12-17 read in step namelist*/
    if (ivalof(&type_dv)>=12 && ivalof(&type_dv)<=17){
      gint irow=pot_number-1;
      gint who=ivalof(&type_dv)==12?1:2;

      /*Create a clist to hold "step" namelist (couplings)*/
      step_clist=gtk_clist_new(COLUMNS_STEP);
      
      /*Attach the "step_clist" to this row of pot_clist*/
      gtk_clist_set_row_data(GTK_CLIST(pot_clist),irow,step_clist);
      
      /*Read step namelists with coupling details between states*/
      while((iread=readnl(&step,f))!=EMPTY_LIST ){
	gint ib=ivalof(&step_ib_dv);
	gint ia=ivalof(&step_ia_dv);
	gchar *row_step[COLUMNS_STEP];
	enum{IB,IBDESC,IA,IADESC,K,STR};

	if (iread==LIST_NOT_FOUND) {
	  g_print("WARNING!!: Namelist step not found");
	  return(-1);
	}

	if (iread==END_OF_FILE) {
	  g_print("WARNING!!:End of file trying to read step namelist\n");
	  return(-1);
	}
	
	i=get_partition_number_from_kp(GTK_CLIST(part_clist),abs(kp));
	row_step[IB]=g_strdup(abs_valof(&step_ib_dv));
	row_step[IBDESC]=state_desc(GTK_WIDGET(main_window),i,abs(ib),who);
	row_step[IA]=g_strdup(valof(&step_ia_dv));
	row_step[IADESC]=state_desc(GTK_WIDGET(main_window),i,abs(ia),who);
	row_step[K]=g_strdup(valof(&step_k_dv));
	row_step[STR]=g_strdup(valof(&step_str_dv));
	gtk_clist_append(GTK_CLIST(step_clist),row_step);
	 
	resetnl(&step);
	if (ib<0) {
	  /* g_print("Negative value of ib in step ->last namelist\n");*/
	  break;
	}
   	
      }
      
      /*Attach data_clist to the present row of coup_clist
      gtk_clist_set_row_data(GTK_CLIST(pot_clist),pot_number-1,step_clist);
      g_print("Acabo de poner namelist step en pot %i\n",irow);*/
    }

    /* Reset potential namelist*/
    resetnl(&pot);
   if (kp<0) break;  
 }
  /* gtk_clist_thaw removed in GTK-4 */

  /* gtk_clist_thaw(GTK_CLIST(pot_clist)); */
  g_list_free(potlist);
  xfr_print("Read %i potentials\n",pot_number);

  /*******************************************************/
  /**                 READ OVERLAPS                     **/
  /*******************************************************/
  overlap_clist=lookup_widget(main_window,"overlap_clist");
  /* gtk_clist_freeze removed in GTK-4 */

  /* gtk_clist_freeze(GTK_CLIST(overlap_clist)); */
  gtk_clist_clear(GTK_CLIST(overlap_clist));
  /*kn2 is not used for 1-particle overlaps*/
  
  while(readnl(&overlap,f)!=EMPTY_LIST){
    gchar *overlap_row[]={valof(&kn1_dv),
			  valof(&kn2_dv),
			  valof(&ic1_dv),
			  valof(&ic2_dv),
			  valof(&in_dv),
			  valof(&kind_dv),
			  valof(&ch1_dv),
			  valof(&nn_dv),
			  valof(&l_dv),
			  valof(&lmax_dv),
			  valof(&sn_dv),
			  valof(&ia_dv),
			  valof(&j_dv),
			  valof(&ib_dv),
			  valof(&kbpot_dv),
			  valof(&krpot_dv),
			  valof(&be_dv),
			  valof(&isc_dv),
			  valof(&ipc_dv),
			  valof(&nfl_dv),
			  valof(&nam_dv),
			  valof(&ampl_dv),
			  valof(&dm_dv),
			  valof(&nk_dv),
			  valof(&er_dv),
			  valof(&e_dv)};

    if (iread==LIST_NOT_FOUND) break;
    gtk_clist_append(GTK_CLIST(overlap_clist),overlap_row);
    overlap_numb++;
    /*Reset to default if keep=false*/
    if (g_strcasecmp(valof(&keep_dv),"T")!=0){
      resetnl(&overlap);
    }
  }
  /* gtk_clist_thaw removed in GTK-4 */
  /* gtk_clist_thaw(GTK_CLIST(overlap_clist)); */

  if (xfr_clist_get_rows(overlap_clist)>0){
    gtk_clist_select_row(GTK_CLIST(overlap_clist),0,0);
/*    g_print("selection first row in overlap clist\n"); */
  }
  
  /*We save a pointer to the main window inside the window_bins's
    data list, so we can get it easily in the callbacks. */

  /* no longer necessary */
  /* g_object_set_data (GTK_OBJECT (window_bins), MainWindowKey, main_window); */    
  
  /*And to the inverse...*/
  /* g_object_set_data(G_OBJECT(main_window),"window_bins",window_bins);*/



  /********************************************************/
  /**                 READ COUPLINGS                     **/
  /********************************************************/

  read_couplings(f);
  fclose(f);

  if (nwarnings>0)
    g_print("\n\n Total number of WARNINGS=%i\n",nwarnings);

  /*free memory allocated for namelists
   freenl(&fresco);
   freenl(&partition);freenl(&states);
   freenl(&pot);freenl(&step);
   freenl(&overlap);*/


  /*  buffer=g_strdup_printf("OK");
  gtk_statusbar_push( GTK_STATUSBAR(statusbar), 1, buffer);*/
  gtk_statusbar_pop( GTK_STATUSBAR(statusbar),contid);

  return(0);
}









/***************************************************************/
gint
read_couplings(FILE *f){
  gint iread,i,iq;
  gint count_coup=0;  
  GtkCList *coup_clist=GTK_CLIST(lookup_widget(main_window,"coup_clist"));
  GtkWidget *check_icto=lookup_widget(main_window,"check_icto");
  
  GtkCList *inel_clist=GTK_CLIST(lookup_widget(main_window,"inel_clist"));
  GtkCList *cfp_clist=GTK_CLIST(lookup_widget(main_window,"cfp_clist"));
  GtkCList *data_clist;/*clist structure to attach the cfp namelist's*/
  
  /*Variables for namelist: COUPLING */
  Vardesc icto_dv=define("icto","0",XFR_NUMBER);
  Vardesc icfrom_dv=define("icfrom","0",XFR_NUMBER);
  Vardesc kind_dv=define("kind","0",XFR_NUMBER);
  Vardesc ip1_dv=define("ip1","0",XFR_NUMBER);
  Vardesc ip2_dv=define("ip2","0",XFR_NUMBER);
  Vardesc ip3_dv=define("ip3","0",XFR_NUMBER);
  Vardesc p1_dv=define("p1","0",XFR_NUMBER);
  Vardesc p2_dv=define("p2","0",XFR_NUMBER);
  Vardesc jmax_dv=define("jmax","0",XFR_NUMBER);
  Vardesc rmax_dv=define("rmax","0",XFR_NUMBER);
  Vardesc kfrag_dv=define("kfrag","0",XFR_NUMBER);
  Vardesc kcore_dv=define("kcore","0",XFR_NUMBER);
  
  /*Variables for namelist: INEL */
  Vardesc inel_ib_dv=define("ib","0",XFR_NUMBER);
  Vardesc inel_ia_dv=define("ia","0",XFR_NUMBER);
  Vardesc inel_k_dv=define("k","0",XFR_NUMBER);
  Vardesc inel_no_dv=define("no","0",XFR_NUMBER);
  Vardesc inel_kp_dv=define("kp","0",XFR_NUMBER);
  Vardesc inel_a_dv=define("a","0",XFR_NUMBER);

  /*Variables for namelist: CFP */
  Vardesc cfp_in_dv=define("in","0",XFR_NUMBER);
  Vardesc cfp_ib_dv=define("ib","0",XFR_NUMBER);
  Vardesc cfp_ia_dv=define("ia","0",XFR_NUMBER);
  Vardesc cfp_kn_dv=define("kn","0",XFR_NUMBER);
  Vardesc cfp_a_dv=define("a","0",XFR_NUMBER);
  Vardesc cfp_keep_dv=define("keep","F",XFR_STRING);


  /*Variables for namelist: SCALE */
  Vardesc qscale_dv=define_carray("qscale","0.0",0,QSCALEDIM,0,XFR_COMPLEX);

  /*Array of variables for namelist coupling*/
  Vardesc *coupling_vl[]={&icto_dv,
			  &icfrom_dv,
			  &kind_dv,
			  &ip1_dv,   
			  &ip2_dv,     
			  &ip3_dv,
			  &p1_dv,    
			  &p2_dv,      
			  &rmax_dv,  
			  &jmax_dv,
			  &kfrag_dv, 
			  &kcore_dv};

     
   /*Idem for namelist inel*/
   Vardesc *inel_vl[]={&inel_ib_dv,&inel_ia_dv,&inel_k_dv,
		       &inel_no_dv,&inel_kp_dv,&inel_a_dv};


   /*Idem for namelist cfp*/
   Vardesc *cfp_vl[]={&cfp_in_dv, &cfp_ib_dv, &cfp_ia_dv,
		      &cfp_kn_dv, &cfp_a_dv , &cfp_keep_dv};

   /*Idem for namelist scale*/
   Vardesc *scale_vl[]={&qscale_dv};

   Namelist coupling={"COUPLING",coupling_vl,12};
   Namelist inel={"INEL",inel_vl,6};
   Namelist cfp={"CFP",cfp_vl,6};
   Namelist scale={"SCALE",scale_vl,1};

   enum{ICT0,ICFROM,KIND,IP1,IP2,IP3,P1,P2,RMAX,JMAX};

   /***************** End of declarations **************************/
 
   xfr_print("Clearing qscale namelist\n");
   if (qscale_clist) gtk_clist_clear(GTK_CLIST(qscale_clist));
   xfr_print("Clearing coup namelist\n");
   if (coup_clist) gtk_clist_clear(GTK_CLIST(coup_clist));
   xfr_print("Clearing CFP namelist\n");
   if (cfp_clist) gtk_clist_clear(GTK_CLIST(cfp_clist));
   xfr_print("Clearing INEL namelist\n");
   if (inel_clist)gtk_clist_clear(GTK_CLIST(inel_clist));
  
  

  

   while((iread=readnl(&coupling,f))!=EMPTY_LIST){
     gint icto=ivalof(&icto_dv);
     if (iread==LIST_NOT_FOUND) return(LIST_NOT_FOUND);
     if (ivalof(&kfrag_dv)>0) assign(&p1_dv,valof(&kfrag_dv));
     if (ivalof(&kcore_dv)>0) assign(&p2_dv,valof(&kcore_dv));


     if (iread!=EMPTY_LIST && ivalof(&icto_dv)!=0){
       gchar *coup_row[]={valof(&icto_dv),
			  valof(&icfrom_dv),
			  valof(&kind_dv),
			  valof(&ip1_dv),
			  valof(&ip2_dv),
			  valof(&ip3_dv),
			  valof(&p1_dv),
			  valof(&p2_dv),
			  valof(&rmax_dv),
			  valof(&jmax_dv)};
       count_coup++;
       gtk_clist_append(coup_clist,coup_row);
     }
     else{
       return(EMPTY_LIST);
     }
     
     /* Read CFP and INEL */
     switch(ivalof(&kind_dv)){     
     default:
       break;
     case 1:case 2: /* Inelastic */
       /*Read IP1's namelists INEL*/
       xfr_print("kind=1 coupling read->Reading inel namelist...\n");
      
       data_clist=GTK_CLIST(gtk_clist_new(COLUMNS_INEL));
       
       for(i=1;1>0;i++){       	 
	 if (readnl(&inel,f)==LIST_NOT_FOUND) break;
	 if (ivalof(&inel_ib_dv)!=0){
	   gint ib=ivalof(&inel_ib_dv);
	   gchar *inel_row[]={abs_valof(&inel_ib_dv),
			      valof(&inel_ia_dv),
			      valof(&inel_k_dv),
			      valof(&inel_no_dv),
			      valof(&inel_kp_dv),
			      valof(&inel_a_dv)};
	   
	   gtk_clist_append(data_clist,inel_row);
	    resetnl(&inel);
	   if (ib<0) break; 
	   if( ivalof(&ip1_dv)>0 && i==ivalof(&ip1_dv)) break;
	 }
	 else{
	   /* ib=0 => do not parse this namelist and stop reading inel */ 
	   break;
	 }
       }
       /*Attach data_clist to the present row of coup_clist*/
       xfr_print("\n Attaching data_clist(inel) to row=%i\n",count_coup-1);
       gtk_clist_set_row_data(coup_clist,count_coup-1,data_clist);
       break;
       
     case 3:case 4:case 5: case 6:case 7:case 8:
     /*Read namelists CFP*/
     xfr_print("\nReading CFP namelists for kind=%i coupling..",\
	     ivalof(&kind_dv));
     data_clist=GTK_CLIST(gtk_clist_new(COLUMNS_CFP));
     
     if (ivalof(&kind_dv)==8 && ivalof(&ip2_dv)<=0) break;
     
     while((iread=readnl(&cfp,f))!=EMPTY_LIST || iread==LIST_NOT_FOUND){
       gint in=ivalof(&cfp_in_dv);
       gchar *cfp_row[]={abs_valof(&cfp_in_dv),
			 valof(&cfp_ib_dv),
			 valof(&cfp_ia_dv),
			 valof(&cfp_kn_dv),
			 valof(&cfp_a_dv)};
       
       if (in==0){
	 xfr_print("\nIN=0 found->Stop reading cfp\n");
	 break;}

       gtk_clist_append(data_clist,cfp_row);
       
       

       if (in<0) break; /*Last cfp just read*/
       if (g_strcasecmp(valof(&cfp_keep_dv),"T")!=0 &&
	   g_strcasecmp(valof(&cfp_keep_dv),"TRUE")!=0){
	 resetnl(&cfp);
       }
     }
     /*Attach data_clist to the present row of coup_clist*/
     
     if (data_clist==NULL) g_warning("In read_couplings, cfp_clist NULL!!\n");
     xfr_print("\n Attaching cfp clist to row=%i\n",count_coup-1);
     gtk_clist_set_row_data(coup_clist,count_coup-1,data_clist);
     
     break;
     
     } /* switch */


     if (qscale_clist==NULL){
       xfr_print("\n Creating CLIST for qscale with %i columns", 2*(QSCALEDIM+1));
       qscale_clist=gtk_clist_new(2*(QSCALEDIM+1));
     }

    
     if ((ivalof(&kind_dv)==3||ivalof(&kind_dv)==4)&&(ivalof(&ip3_dv)>9)){
       gchar *row[2*(QSCALEDIM+1)];
       
      if (readnl(&scale,f)==LIST_NOT_FOUND) return(LIST_NOT_FOUND); 
       
       /* TEST */
    /*     g_print("\n qscale[0]=%s",valof_n(&qscale_dv,0)); */
/*         g_print("\n qscale[1]=%s",valof_n(&qscale_dv,1)); */
/*         g_print("\n qscale[2]=%s",valof_n(&qscale_dv,2)); */
/*         g_print("\n qscale[3]=%s",valof_n(&qscale_dv,3)); */
/*         g_print("\n qscale[4]=%s",valof_n(&qscale_dv,4)); */
       

       for(iq=0;iq<=QSCALEDIM;iq++){
	
 	 /*row[2*iq]=g_strdup("0.0"); */
  	 /*row[2*iq+1]=g_strdup("0.0"); */

	 row[2*iq]=g_strstrip(g_strdup(valof_n(&qscale_dv,iq)));
	 row[2*iq+1]=g_strstrip(g_strdup(valof_nimag(&qscale_dv,iq)));

	 /* g_print("\n %i => %s,%s ",iq, row[2*iq],row[2*iq+1]); */
       }
       gtk_clist_append(GTK_CLIST(qscale_clist),row);
       
     
     }
     else{
       /* g_print("\n ... and filling with default values"); */
       qscale_clist_add_default();
     }

     if (icto<0) xfr_toggle_set_value(check_icto,"T");
     if (icto==0) return(0);
     else resetnl(&coupling);
   }
   if (xfr_clist_get_rows(coup_clist)>0)
     gtk_clist_select_row(GTK_CLIST(coup_clist),0,0);

   freenl(&coupling);
   freenl(&cfp);
   freenl(&inel);
   freenl(&scale);

return(0);
}






/***************************************************************************/
/*                       AUXILIARY FUNCTIONS                               */
/***************************************************************************/



/*Refresh the content of "combo_lampl". This combo contains 
 *the list of partitions in clist_combo with a brief description
 *for each partition NOT USES NOW*/ 
void
refresh_combo_lampl(GtkWidget *combo,GtkCList *clist){
  GList *glist=NULL;
  /*Number of partitions*/
  gint rows=xfr_clist_get_rows((GtkWidget*)clist);
  gint i=0;
  while(i<rows){
    gchar *namep,*namet,*text;
    gtk_clist_get_text(clist,i,0,&namep); /*Proj. name is on column 0*/
    gtk_clist_get_text(clist,i,3,&namet); /*Targ. name is on column 3*/
    text=g_strconcat(namep,"+",namet,NULL);
    glist=g_list_append(glist,(gpointer)text);
    i++;
  }
  if (combo && glist){
    /* GtkCombo removed in GTK-4 */
    /* gtk_combo_set_popdown_strings(GTK_COMBO(combo),glist); */
  }
}
 

/*   g_list_append (GList *list,gpointer data); */

void 
put_partition(GtkWidget *clist,Namelist *partition)
{
  gint i;
  gchar *row[COLUMNS_PARTITIONS];
  Vardesc **varlist=partition->vars;
  /* gtk_clist_freeze removed in GTK-4 */

  /* gtk_clist_freeze(GTK_CLIST(clist)); */
  for (i=0;i<COLUMNS_PARTITIONS;i++)
    {
      row[i]=g_strdup(valof(*(varlist+i)));
    }

  
  gtk_clist_append( (GtkCList*)clist, row);
  /* gtk_clist_thaw removed in GTK-4 */

  /* gtk_clist_thaw(GTK_CLIST(clist)); */
}
 
void
put_states(GtkWidget *clist,Namelist *nl)
{
  gint i;
  gchar *row[COLUMNS_STATES+1];
  Vardesc **varlist=nl->vars;
  GtkWidget *frame_states=lookup_widget(GTK_WIDGET(main_window),"frame_states");
  enum {
    JP,COPYP,BANDP,EP,KKP,TP,CPOT,
    JT,COPYT,BANDT,ET,KTT,TT,FEXCH,IGNORE,INFAM,OUTFAM,PTYP,PTYT
  };
  Vardesc *bandp=*(varlist+BANDP);
  Vardesc *bandt=*(varlist+BANDT);
  Vardesc *ptyp=*(varlist+PTYP);
  Vardesc *ptyt=*(varlist+PTYT); 


  /*xfr_print("En put_states ptyp=%s",valof(ptyp));*/  
  /*xfr_print("En put_states jp=%s",valof(jp)); */
  
  if (ptyp->read==TRUE){ 
    assign(bandp,valof(ptyp));
    xfr_print("bandp=%s",valof(bandp));
  };
  if (ptyt->read==TRUE) assign(bandt,valof(ptyt));

  /* gtk_clist_freeze removed in GTK-4 */


  /* gtk_clist_freeze(GTK_CLIST(clist)); */
  for (i=0;i<=COLUMNS_STATES;i++)
    {

      row[i]=g_strdup(g_strdup(valof(*(varlist+i))));
    }
   
  gtk_clist_append(GTK_CLIST(clist), row);
  gtk_widget_set_sensitive (clist, TRUE);
  gtk_widget_set_sensitive (frame_states,TRUE);
  /* gtk_clist_thaw removed in GTK-4 */

  /* gtk_clist_thaw(GTK_CLIST(clist)); */
}





















































