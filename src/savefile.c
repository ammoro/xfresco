#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include "clist_compat.h"
#include "utils.h"
#include "savefile.h"
#include "interface.h"
#include "support.h"

/** Run'dan önce "Save" in otomatik yapılabilmesi için 19 Eylül de callbacks.c
    ve savefile.c için de yerdeğiştirmeler yaptım.
   unistd.h --> getpid() process ID yakalayıcı için gerekli.
**/
#include <unistd.h>

extern GtkWidget *save_filesel;
extern gchar *current_filename;
extern gboolean GPRINT; 

extern GtkWidget *qscale_clist ;

/* This is the process number currently running */
/** savefile.c den buraya aldım kill_current_run() için**/
gint idchild=-1;
/** savefile.c den buraya aldım run_file() için**/
extern gboolean FRESCO_IS_RUNNING;


/* This shows the file selection dialog to save a file. */
void
save_as (GtkWidget *main_window)
{
  if (save_filesel == NULL)
    save_filesel = create_save_filesel ();

  g_object_set_data (G_OBJECT (save_filesel), "main_window", main_window);

  /* If the current document has a filename we use that as the default. */
  if (current_filename)
    /* GtkFileSelection removed in GTK-4 - use GtkFileChooserDialog */

    /* gtk_file_selection_set_filename (GTK_FILE_SELECTION (save_filesel),
				     current_filename); */

  gtk_widget_show (save_filesel);
  /* gdk_window_raise removed in GTK-4 - use gtk_window_present() */

  /* gdk_window_raise (save_filesel->window); */
}

void
real_save_file(GtkWidget *main_window,const gchar *filename){
  gint irow,icol,i=0;
  guint contid;
  gchar *buffer;
  gchar *date;
  time_t timet;
  FILE *f;
  
 /*Variable description for namelist "fresco"*/
  /* arg. 1: variable name */
  /* arg. 2: default value */
  /* arg. 3: data type */

  Vardesc hcm_dv=define("hcm","0.0",XFR_NUMBER);
  Vardesc rmatch_dv=define("rmatch","",XFR_NUMBER);
  Vardesc rintp_dv=define("rintp","0.5",XFR_NUMBER);
  Vardesc hnl_dv=define("hnl","0",XFR_NUMBER);
  Vardesc rnl_dv=define("rnl","0",XFR_NUMBER);
  Vardesc rsp_dv=define("rsp","0",XFR_NUMBER);
  Vardesc centre_dv=define("centre","0",XFR_NUMBER);
  Vardesc hnn_dv=define("hnn","0",XFR_NUMBER);
  Vardesc rnn_dv=define("rnn","0",XFR_NUMBER);
  Vardesc rmin_dv=define("rmin","0",XFR_NUMBER);
  Vardesc rasym_dv=define("rasym","0.01",XFR_NUMBER);/*  ??? */
  Vardesc accrcy_dv=define("accrcy","1000",XFR_NUMBER);/*  ??? */
  Vardesc Switch_dv=define("switch","0.0",XFR_NUMBER);
  Vardesc ajswtch_dv=define("ajswtch","0",XFR_NUMBER);
  Vardesc jtmin_dv=define("jtmin","0",XFR_NUMBER);
  Vardesc jtmax_dv=define("jtmax","0",XFR_NUMBER);
  Vardesc absend_dv=define("absend","0",XFR_NUMBER);
  Vardesc dry_dv=define("dry","F",XFR_NUMBER);
  Vardesc rela_dv=define("rela","",XFR_STRING);
  Vardesc nearfa_dv=define("nearfa","0",XFR_NUMBER);
  Vardesc jump_dv=define_array("jump","0",1,6,1,XFR_NUMBER);
  Vardesc jbord_dv=define_array("jbord","0.0",1,6,0,XFR_NUMBER);
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
  Vardesc numnode_dv=define("numnode","0",XFR_NUMBER);
  Vardesc plane_dv=define("plane","0",XFR_NUMBER);
  Vardesc it0_dv=define("it0","0",XFR_NUMBER);
  Vardesc iter_dv=define("iter","0",XFR_NUMBER);
  Vardesc iblock_dv=define("iblock","0",XFR_NUMBER);
   /* R-matrix */
  Vardesc nrbases_dv=define("nrbases","0",XFR_NUMBER);
  Vardesc nrbmin_dv=define("nrbmin","0",XFR_NUMBER);
  Vardesc pralpha_dv=define("pralpha","F",XFR_NUMBER);
  Vardesc pcon_dv=define("pcon","0",XFR_NUMBER);
  Vardesc meigs_dv=define("meigs","0",XFR_NUMBER);
  Vardesc rmatr_dv=define("rmatr","0.0",XFR_NUMBER);
  Vardesc buttle_dv=define("buttle","0.0",XFR_NUMBER);
  Vardesc ebeta_dv=define("ebeta","0.0",XFR_NUMBER);
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
  /* Vardesc elab_dv=define("elab","0",XFR_NUMBER);*/
  Vardesc elab_dv=define_array("elab","0",1,4,0,XFR_NUMBER);
  Vardesc nlab_dv=define_array("nlab","0",1,3,0,XFR_NUMBER);
  Vardesc fatal_dv=define("fatal","T",XFR_NUMBER);
  Vardesc nosol_dv=define("nosol","F",XFR_NUMBER);
  Vardesc nangle_dv=define("nangle","",XFR_NUMBER);
  Vardesc pset_dv=define("pset","0",XFR_NUMBER);
  Vardesc jset_dv=define("jset","0",XFR_NUMBER);
  Vardesc inh_dv=define("inh","",XFR_NUMBER);
  Vardesc psiren_dv=define("psiren","F",XFR_NUMBER);
  Vardesc tmp_dv=define("tmp","",XFR_STRING);
  Vardesc masfil_dv=define("masfil","",XFR_STRING);
  Vardesc unitmass_dv=define("unitmass","1.000",XFR_NUMBER);
  Vardesc finec_dv=define("finec","137.03599d0",XFR_NUMBER);

 /*Variable description for Namelist partition*/
  Vardesc namep_dv=define("namep","",XFR_STRING);
  Vardesc massp_dv=define("massp","",XFR_NUMBER);
  Vardesc zp_dv=define("zp","",XFR_NUMBER);
  Vardesc nex_dv=define("nex","",XFR_NUMBER);
  Vardesc namet_dv=define("namet","",XFR_STRING);
  Vardesc masst_dv=define("masst","",XFR_NUMBER);
  Vardesc zt_dv=define("zt","",XFR_NUMBER);
  Vardesc qval_dv=define("qval","",XFR_NUMBER);
  Vardesc pwf1_dv=define("pwf","T",XFR_NUMBER);/*we use pwf1 instead of pwf to avoid confusion with pwf in states*/
  Vardesc readstates_dv=define("readstates","",XFR_NUMBER);

  /*Variable description for Namelist states */
  Vardesc jp_dv=define("jp","",XFR_NUMBER);
  Vardesc copyp_dv=define("copyp","",XFR_NUMBER);
  Vardesc bandp_dv=define("bandp","",XFR_NUMBER);
  Vardesc ep_dv=define("ep","0.0000",XFR_NUMBER);
  Vardesc kkp_dv=define("kkp","",XFR_NUMBER);
  Vardesc tp_dv=define("tt","",XFR_NUMBER);
  Vardesc cpot_dv=define("cpot","",XFR_NUMBER);
  /* Vardesc pwf_dv=define("pwf","F",XFR_NUMBER); */
  Vardesc jt_dv=define("jt","",XFR_NUMBER);
  Vardesc copyt_dv=define("copyt","",XFR_NUMBER);
  Vardesc bandt_dv=define("bandt","",XFR_NUMBER);
  Vardesc et_dv=define("et","0.0000",XFR_NUMBER);
  Vardesc kkt_dv=define("kkt","",XFR_NUMBER);
  Vardesc tt_dv=define("tt","",XFR_NUMBER);
  Vardesc extra_dv=define("extra","F",XFR_NUMBER);
  Vardesc fexch_dv=define("fexch","F",XFR_NUMBER);
  Vardesc ignore_dv=define("ignore","F",XFR_NUMBER);
  Vardesc infam_dv=define("infam","0",XFR_NUMBER);
  Vardesc outfam_dv=define("outfam","0",XFR_NUMBER);
  /*  
      Vardesc ptyp_dv=define("ptyp","",XFR_NUMBER);
      Vardesc ptyt_dv=define("ptyt","",XFR_NUMBER);
      */

  /*Variables for namelist: pot */  
 Vardesc kp_dv=define("kp","-1",XFR_NUMBER);
  Vardesc type_dv=define("type","0",XFR_NUMBER);
  Vardesc itt_dv=define("itt","F",XFR_NUMBER);
  Vardesc shape_dv=define("shape","0",XFR_NUMBER);
  Vardesc mnep_dv=define_array("mnep","0",1,7,0,XFR_NUMBER);
  Vardesc mnet_dv=define_array("mnet","0",1,7,0,XFR_NUMBER);
  Vardesc ap_dv=define("ap","0",XFR_NUMBER);
  Vardesc at_dv=define("at","0",XFR_NUMBER);
  Vardesc rc_dv=define("rc","0",XFR_NUMBER);
  Vardesc ac_dv=define("ac","0",XFR_NUMBER);
  Vardesc p_dv=define_array("p","0.000",1,7,0,XFR_NUMBER);
  /*  Vardesc def_dv=define_array("def","0",1,7,0);*/
  Vardesc p0_dv=define("p0","0.000",XFR_NUMBER);
  Vardesc p1_dv=define("p1","0.000",XFR_NUMBER);
  Vardesc p2_dv=define("p2","0.000",XFR_NUMBER);
  Vardesc p3_dv=define("p3","0.000",XFR_NUMBER);
  Vardesc p4_dv=define("p4","0.000",XFR_NUMBER);
  Vardesc p5_dv=define("p5","0.000",XFR_NUMBER);
  Vardesc p6_dv=define("p6","0.000",XFR_NUMBER);
  Vardesc p7_dv=define("p7","0.000",XFR_NUMBER);
  Vardesc v_dv=define("v","0.000",XFR_NUMBER);
  Vardesc r0_dv=define("r0","0.000",XFR_NUMBER);
  Vardesc rv_dv=define("rv","0.000",XFR_NUMBER);
  Vardesc vr0_dv=define("vr0","0.000",XFR_NUMBER);
  Vardesc a_dv=define("a","0.000",XFR_NUMBER);
  Vardesc av_dv=define("av","0.000",XFR_NUMBER);
  Vardesc w_dv=define("w","0.000",XFR_NUMBER);
  Vardesc wr0_dv=define("wr0","0.000",XFR_NUMBER);
  Vardesc rw_dv=define("rw","0.000",XFR_NUMBER);
  Vardesc aw_dv=define("aw","0.000",XFR_NUMBER);
  Vardesc wa_dv=define("wa","0.000",XFR_NUMBER);
  Vardesc r0w_dv=define("r0w","0.000",XFR_NUMBER);
  Vardesc vso_dv=define("vso","0.000",XFR_NUMBER);
  Vardesc rso_dv=define("rso","0.000",XFR_NUMBER);
  Vardesc rso0_dv=define("rso0","0.000",XFR_NUMBER);
  Vardesc aso_dv=define("aso","0.000",XFR_NUMBER);
  Vardesc vsoi_dv=define("vsoi","0.000",XFR_NUMBER);
  Vardesc rsoi_dv=define("rsoi","0.000",XFR_NUMBER);
  Vardesc asoi_dv=define("asoi","0.000",XFR_NUMBER);
  Vardesc wd_dv=define("wd","0.000",XFR_NUMBER);
  Vardesc wdr_dv=define("wdr","0.000",XFR_NUMBER);
  Vardesc wda_dv=define("wda","0.000",XFR_NUMBER);
  Vardesc wdr0_dv=define("wdr0","0.000",XFR_NUMBER);
  Vardesc awd_dv=define("awd","0.000",XFR_NUMBER);
  Vardesc defp_dv=define_array("defp","0.000",1,7,0,XFR_NUMBER);
  Vardesc deft_dv=define_array("deft","0.000",1,7,0,XFR_NUMBER);
  Vardesc vd_dv=define("vd","0.000",XFR_NUMBER);
  Vardesc vdr_dv=define("vdr","0.000",XFR_NUMBER);
  Vardesc vda_dv=define("vda","0.000",XFR_NUMBER);


   /*Variables for namelist: step */
  Vardesc step_ib_dv=define("ib","",XFR_NUMBER);
  Vardesc step_ia_dv=define("ia","",XFR_NUMBER);
  Vardesc step_k_dv=define("k","",XFR_NUMBER);
  Vardesc step_str_dv=define("str","",XFR_NUMBER);

  /*Variables for namelist: overlap */
  Vardesc kn1_dv=define("kn1","0",XFR_NUMBER);
  Vardesc kn2_dv=define("kn2","0",XFR_NUMBER); /*not used for 1 particle f.f*/
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
  /*Vardesc keep_dv=define("keep","false",XFR_NUMBER);*/
  Vardesc dm_dv=define("dm","0",XFR_NUMBER);
  Vardesc nk_dv=define("nk","0",XFR_NUMBER);
  Vardesc er_dv=define("er","0",XFR_NUMBER);
  /*Vardesc e_dv=define("e","0",XFR_NUMBER);*/

  /*NAMELIST COUPLING->See write_couplings function*/
 
   /*Structure of variables for namelist FRESCO*/
  Vardesc *fresco_vl[]={&hcm_dv,&rmatch_dv,&rintp_dv,
			&hnl_dv,&rnl_dv,&centre_dv,
			&hnn_dv,&rnn_dv,&rmin_dv,&rsp_dv,
			&rasym_dv,&accrcy_dv,&Switch_dv,&ajswtch_dv,
			&jtmin_dv,&jtmax_dv,
			&absend_dv,&dry_dv,&rela_dv,&nearfa_dv,
			&jump_dv,&jbord_dv,
			&kqmax_dv,&pp_dv,&thmin_dv,
			&thmax_dv,&thinc_dv,
			&koords_dv,&cutl_dv,&cutr_dv,
			&cutc_dv,&ips_dv,
			&smallchan_dv,&smallcoup_dv,&numnode_dv,
                        &plane_dv,
			&it0_dv,
			&iter_dv,&iblock_dv,
			&nrbases_dv,&nrbmin_dv,&pralpha_dv,
			&pcon_dv,&meigs_dv,&rmatr_dv,&buttle_dv,
			&ebeta_dv,
			&pade_dv,
			&iso_dv,&nnu_dv,&maxl_dv,
			&minl_dv,&mtmin_dv,&epc_dv,
			&erange_dv,&dk_dv,&chans_dv,
			&listcc_dv,&treneg_dv,
			&cdetr_dv,&smats_dv,&xstabl_dv,
			&nlpl_dv,&waves_dv,
			&lampl_dv,&veff_dv,&kfus_dv,&nfus_dv,
			&wdisk_dv,&bpm_dv,&melfil_dv,&cdcc_dv,
			&pel_dv,&exl_dv,&lab_dv,
			&lin_dv,&lex_dv,&elab_dv,
			&nlab_dv,&fatal_dv,&nosol_dv,
			&nangle_dv,&pset_dv,
			&jset_dv,&inh_dv,&psiren_dv,&tmp_dv,
			&tmp_dv,&masfil_dv,&unitmass_dv,&finec_dv};

 
  /*Structure of variables for namelist: PARTITION */
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
			&ignore_dv,
			&fexch_dv,
			&infam_dv,
			&outfam_dv};
  
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
		      &vd_dv,&vdr_dv,&vda_dv};
		      
 /*These namelists are not used in fresco, but they help me to write the output*/
   /*Coulomb potential*/
   Vardesc *cpot_vl[]={&kp_dv,
		       &type_dv,
		       &shape_dv,
		       &itt_dv,
		       &ap_dv,
		       &at_dv,
		       &rc_dv,
		       &ac_dv};

   /*Other single namelistd*/
   Vardesc *spot_vl[]={&kp_dv,
		       &type_dv,
		       &shape_dv,
		       &itt_dv,
		       &p1_dv,&p2_dv,&p3_dv,&p4_dv,&p5_dv,&p6_dv,&p7_dv};
   
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
			  &dm_dv,&nk_dv,&er_dv};
   
 
   
  /*Namelist declarations 77, 8, 17, 8, 11, 4, 25 */

   Namelist fresco={"FRESCO",fresco_vl,sizeof(fresco_vl)/sizeof(&hcm_dv)};
   Namelist partition={"PARTITION",partition_vl,\
		       sizeof(partition_vl)/sizeof(&namep_dv)};
   Namelist states={"STATES",states_vl,sizeof(states_vl)/sizeof(&jp_dv)};
   /* Namelist pot={"POT",pot_vl,sizeof(pot_vl)/sizeof(&kp_dv)}; */
   Namelist cou_pot={"POT",cpot_vl,sizeof(cpot_vl)/sizeof(&kp_dv)};
   Namelist spot={"POT",spot_vl,sizeof(spot_vl)/sizeof(&kp_dv)};
   Namelist step={"STEP",step_vl,sizeof(step_vl)/sizeof(&step_ib_dv)};
   Namelist overlap={"OVERLAP",overlap_vl,\
		     sizeof(overlap_vl)/sizeof(&kn1_dv)};

   


   /*Relevant widgets to retrieve information */
   
   /*General use*/
   GtkToggleButton *toggle;
   GtkWidget *widget;
   /*   GtkWidget *statusbar=get_widget(main_window,"statusbar");*/
   GtkWidget *statusbar=lookup_widget(main_window,"statusbar");
   gint iopt;

   /*Fresco*/
   GtkWidget *window_ccwf=lookup_widget(main_window,"window_ccwf");
   GtkWidget *window_jbord=lookup_widget(main_window,"window_jbord");
   /*GtkWidget *window_bins=lookup_widget(main_window,"window_bins");*/
   /*GtkWidget *window_constants=lookup_widget(main_window,"window_constants");*/
   GtkWidget *window_Rmatrix=lookup_widget(main_window,"window_Rmatrix");

   /*GtkWidget *window_eintervals=get_widget(main_window,"window_eintervals");*/
 
   /*Partition & states*/
   enum{PNUCLEUS,PMASS,PZ,TNUCLEUS,TMASS,TZ,QVALUE,PWF1,XSEC,READSTATES};/*Partition clist*/
   GtkCList *part_clist=GTK_CLIST(lookup_widget(main_window,"part_clist"));
   GtkCList *states_clist;
   /* GtkWidget *states_nb=get_widget(main_window,"states_nb");*/
   
   /*Potentials*/
   GtkCList *pot_clist=GTK_CLIST(lookup_widget(main_window,"pot_clist"));
   
   /*Overlap*/
   GtkCList *overlap_clist=GTK_CLIST(lookup_widget(main_window,"overlap_clist"));
   

   /********************************************************************/
   /*          END OF DECLARATIONS: START WRITING NAMELISTS            */
   /*          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~            */


   xfr_print("\nSaving project in file '%s'\n\n",filename);
   buffer=g_strdup_printf("Saving file='%s'\n\n",filename);

   contid=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),
				       "savefile");

   buffer=g_strdup_printf("Opening file='%s'",current_filename);
   gtk_statusbar_push( GTK_STATUSBAR(statusbar), contid, buffer);


   if ((f=fopen(filename,"w"))==NULL){
    glade_util_show_message_box ("Error opening file");
    g_print("\nERROR opening input file '%s'!",filename);
    return;
   }
   
   gtk_window_set_title(GTK_WINDOW(main_window),filename);
   
   /* In version 2.0 onward, the header is in the main window */
   /* widget=lookup_widget(window_constants,"entry_heading"); */
   widget=lookup_widget(main_window,"entry_heading");
   buffer=g_strdup(gtk_editable_get_text(GTK_EDITABLE(widget)));
   fputs(buffer,f);
   fputs("\nNAMELIST\n",f);

   /* Now we have to extract the values from the controls and create the 
    * namelists to be written with writenl
    */
   
   /*****************************************************************/
   /*                     NAMELIST FRESCO                           */
   /*****************************************************************/

   /* HCM,RMATCH,RINTP,HNL,RNL,CENTRE,HNN,RNN,RMIN,RSP */
   buffer=xfr_get_text(main_window,"entry_hcm");
   assign(&hcm_dv,buffer);

   /* RMATCH:
    * if use_ccwf is pressed write -rmatch  
    */
   /*if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(get_widget(main_window,"check_use_ccwf"))))
     buffer=g_strdup_printf("-%s",xfr_get_text(main_window,"entry_rmatch"));
   else 
     buffer=xfr_get_text(main_window,"entry_rmatch");

   assign(&rmatch_dv,buffer);*/

   assign(&rmatch_dv,xfr_get_text(main_window,"entry_rmatch"));
   assign(&rintp_dv,xfr_get_text(main_window,"entry_rintp"));
   assign(&hnl_dv,xfr_get_text(main_window,"entry_hnl"));
   assign(&rnl_dv,xfr_get_text(main_window,"entry_rnl"));
   assign(&centre_dv,xfr_get_text(main_window,"entry_centre"));
   assign(&hnn_dv,xfr_get_text(main_window,"entry_hnn"));
   assign(&rnn_dv,xfr_get_text(main_window,"entry_rnn"));
   assign(&rmin_dv,xfr_get_text(main_window,"entry_rmin"));
   assign(&rsp_dv,xfr_get_text(main_window,"entry_rsp"));

   /*CCWF*/
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_ccwf,"check_rasym"))))
     buffer=g_strdup_printf("-%s",xfr_get_text(window_ccwf,"entry_rasym"));
   else 
     buffer=xfr_get_text(window_ccwf,"entry_rasym");

   assign(&rasym_dv,buffer);
   assign(&accrcy_dv,xfr_get_text(window_ccwf,"entry_accrcy"));
   assign(&Switch_dv,xfr_get_text(window_ccwf,"entry_switch"));
   assign(&ajswtch_dv,xfr_get_text(window_ccwf,"entry_ajswtch"));

   /*JTMIN,JTMAX,ABSEND,JUMP,JBORD*/
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_jtmin"))))
     buffer=g_strdup_printf("-%s",xfr_get_text(window_ccwf,"entry_jtmin"));
   else 
     buffer=xfr_get_text(main_window,"entry_jtmin");


   assign(&jtmin_dv,buffer);  
   assign(&jtmax_dv,xfr_get_text(main_window,"entry_jtmax"));

   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_absend"))))
     buffer=xfr_get_text(main_window,"entry_absend");
   else
     buffer=g_strdup_printf("-%s",\
			    xfr_get_text(main_window,"entry_absend"));
   

   
   assign(&absend_dv,buffer); 

   /*JBORD*/
   for (i=2;i<6;i++){
     gchar *jbord_name=g_strdup_printf("jbord%i",i);
     gchar *jump_name=g_strdup_printf("jump%i",i);
     assign_n(&jbord_dv,i-2,xfr_get_text(window_jbord,jbord_name));
     assign_n(&jump_dv,i-2,xfr_get_text(window_jbord,jump_name));
    }





   /* R-matrix */
   if (window_Rmatrix==NULL){
     g_print("ERROR**: window_Rmatrix=NULL\n");
   }
   else{  
     assign(&nrbases_dv,xfr_get_text(window_Rmatrix,"spin_nrbases"));
     assign(&nrbmin_dv,xfr_get_text(window_Rmatrix,"spin_nrbmin"));
     
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(window_Rmatrix,\
				      "check_pralpha"))))
       assign(&pralpha_dv,"T");
     else 
       assign(&pralpha_dv,"F");
     
     assign(&pcon_dv,xfr_option_menu_get_char_value(window_Rmatrix,\
						    "opt_pcon"));  
     assign(&buttle_dv,xfr_option_menu_get_char_value(window_Rmatrix,\
						    "opt_buttle"));  
     assign(&meigs_dv,xfr_get_text(window_Rmatrix,"spin_meigs"));
     assign(&rmatr_dv,xfr_get_text(window_Rmatrix,"entry_rmatr"));
     assign(&ebeta_dv,xfr_get_text(window_Rmatrix,"entry_ebeta"));
   }

   /*DRY*/
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_dry"))))
     assign(&dry_dv,"T");
   else 
     assign(&dry_dv,"F");


   iopt=xfr_option_menu_get_value(main_window,"opt_nearfa2");
   switch(iopt){
   case 0:
     /* g_print("Usual cross sections active\n"); */
     buffer=g_strdup("0");break;
   case 1:
     buffer=g_strdup("2");break;
   case 2:
     buffer=g_strdup("3");break;
   }
   
   if (iopt>0){
    
     gint iopt1=xfr_option_menu_get_value(main_window,"opt_nearfa1");
     g_print("nearfa1=%i\n",iopt);
     if (iopt1>0){buffer=g_strdup_printf("-%s",buffer);};
   }
   assign(&nearfa_dv,buffer);



   /*NEARFA*/
   /*   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(get_widget(main_window,"rb_usualcs")))){
     buffer=g_strdup("0");

   }
   else{ 
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(get_widget(main_window,"rb_fs"))))
       buffer=g_strdup("2");
     else
       buffer=g_strdup("3");

     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(get_widget(main_window,"rb_nearfa_all"))))
       buffer=g_strdup_printf("-%s",buffer);
    
   }
   assign(&nearfa_dv,buffer);*/

   /*Card 3: KQMAX,PP,THMIN,THMAX,THINC,KOORDS,CUTL,CUTR,CUTC*/
   assign(&kqmax_dv,xfr_get_text(main_window,"spin_kqmax"));
   assign(&pp_dv,xfr_option_menu_get_char_value(main_window,"opt_pp"));
   assign(&thmin_dv,xfr_get_text(main_window,"spin_thmin"));   
   assign(&thmax_dv,xfr_get_text(main_window,"spin_thmax"));
   assign(&thinc_dv,xfr_get_text(main_window,"spin_thinc"));
   assign(&koords_dv,xfr_option_menu_get_char_value(main_window,"opt_koords"));
   assign(&cutl_dv,xfr_get_text(main_window,"entry_cutl"));
   assign(&cutr_dv,xfr_get_text(main_window,"entry_cutr")); 
   assign(&cutc_dv,xfr_get_text(main_window,"entry_cutc")); 

   /*Card 4: IPS,IT0ITER,IBLOCK,PADE,ISO,NNU,MAXL,MINL,MTMIN,EPC,ERANGE,DK*/
   assign(&ips_dv,xfr_get_text(main_window,"entry_ips"));
   assign(&it0_dv,xfr_get_text(main_window,"spin_it0")); 
   assign(&iter_dv,xfr_get_text(main_window,"spin_iter")); 
   assign(&iblock_dv,xfr_get_text(main_window,"spin_iblock"));
   assign(&pade_dv,xfr_option_menu_get_char_value(main_window,"opt_pade"));
   assign(&smallchan_dv,xfr_get_text(main_window,"entry_smallchan"));
   assign(&smallcoup_dv,xfr_get_text(main_window,"entry_smallcoup"));
   assign(&numnode_dv,xfr_get_text(main_window,"entry_numnode"));

   switch (xfr_option_menu_get_value(main_window,"opt_iso")){
   case 0:
     assign(&iso_dv,"0");break;
   case 1:
     assign(&iso_dv,"J");break;
   case 2:
     assign(&iso_dv,"L");break;
   }
   assign(&nnu_dv,xfr_get_text(main_window,"spin_nnu")); 
   assign(&maxl_dv,xfr_get_text(main_window,"entry_maxl")); 
   assign(&minl_dv,xfr_get_text(main_window,"entry_minl"));
   buffer=g_strdup(xfr_get_text(main_window,"entry_mtmin"));
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_mtmin"))))
    assign(&mtmin_dv,g_strdup_printf("-%s",buffer));
   else
    assign(&mtmin_dv,buffer);
   
   /*
   if (window_bins!=NULL){
     assign(&erange_dv,xfr_get_text(window_bins,"entry_erange")); 
     assign(&dk_dv,xfr_get_text(window_bins,"entry_dk"));
     }*/

   assign(&erange_dv,xfr_get_text(main_window,"entry_erange")); 
   assign(&dk_dv,xfr_get_text(main_window,"entry_dk"));

   /*New variables: pset,jset,inh,psiren...*/
   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_pset")))){
     toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"toggle_pset"));
     if (gtk_toggle_button_get_active(toggle)) assign(&pset_dv,"-1");
     else assign(&pset_dv,"1");
   }
     
   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_psiren"))))
     assign(&psiren_dv,"T");
   else 
     assign(&psiren_dv,"F");

   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_nosol"))))
     assign(&nosol_dv,"T");
   else 
     assign(&nosol_dv,"F");

   assign(&jset_dv,xfr_get_text(main_window,"entry_jset"));
   assign(&inh_dv,xfr_option_menu_get_char_value(main_window,"opt_inh"));
   assign(&plane_dv,xfr_option_menu_get_char_value(main_window,"opt_plane"));
   assign(&fatal_dv,xfr_toggle_get_state(lookup_widget(main_window,\
						    "check_fatal")));

   /* these parameters are now in main window */
   /* if (window_constants!=NULL){ */
     assign(&masfil_dv,xfr_get_text(main_window,"entry_masfil"));
     assign(&tmp_dv,xfr_get_text(main_window,"entry_tmp"));
     assign(&unitmass_dv,xfr_get_text(main_window,"entry_unitmass"));
     assign(&finec_dv,xfr_get_text(main_window,"entry_finec"));
  /*}*/

   /*Trace controls*/
   assign(&chans_dv,xfr_get_text(main_window,"spin_chans"));
   assign(&listcc_dv,xfr_get_text(main_window,"spin_listcc"));
   assign(&treneg_dv,xfr_get_text(main_window,"spin_treneg"));
   assign(&cdetr_dv,xfr_get_text(main_window,"spin_cdetr"));
   widget=lookup_widget(main_window,"combo_smats");
   /* GtkCombo removed in GTK-4 */

   /* buffer=g_strndup(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(widget)->entry)),2); */

   buffer=""; /* TODO: Replace with GtkDropDown */
   assign(&smats_dv,buffer);
   assign(&xstabl_dv,xfr_get_text(main_window,"spin_xstabl"));
   assign(&nlpl_dv,xfr_get_text(main_window,"spin_nlpl"));

   switch(xfr_option_menu_get_value(main_window,"opt_waves")){
   case 0:
     i=0;break;/*NO trace*/
   case 1: 
     i=1;break;/* wf */
   case 2:
     i=2;break;/* source term */
   case 3:
     i=3;break;/* wf + source term*/
   case 4:
     i=-1;break;/* wf + ratio to asymptotic*/
   }
   assign(&waves_dv,g_strdup_printf("%i",i));
   
   /*lampl*/
   widget=lookup_widget(main_window,"entry_combo_lampl");
   buffer=g_strdup(gtk_editable_get_text(GTK_EDITABLE(widget)));
   buffer=g_strndup(buffer,2);

   iopt=xfr_option_menu_get_value(main_window,"opt_lampl");
   switch(iopt){
   case 0:
     assign(&lampl_dv,"0");break;
   case 1:
     assign(&lampl_dv,g_strdup_printf("-%s",buffer));break;
   case 2:
     assign(&lampl_dv,buffer);break;
   default:
     g_print("Internal ERROR detected in opt_lampl!!\n");break;
   }


       /*
   toggle=GTK_TOGGLE_BUTTON(get_widget(main_window,"check_lampf"));
   if (gtk_toggle_button_get_active(toggle)){
     widget=get_widget(main_window,"combo_lampl");
     buffer=gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(widget)->entry));
     buffer=g_strndup(buffer,2);

     lampA==GTK_TOGGLE_BUTTON(get_widget(main_window,"check_lampA"));
     if (lampA->active)
       assign(&lampl_dv,buffer);
     else
       assign(&lampl_dv,g_strdup_printf("-%s",buffer));
   }
   else{
     assign(&lampl_dv,"0");
   }
   */

   /*veff*/
   toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_veff"));
   if (gtk_toggle_button_get_active(toggle)){
     toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_veff2"));
     if (gtk_toggle_button_get_active(toggle)) buffer=g_strdup("2");/*exclude p.w. with Sl<0.1*/
     else buffer=g_strdup("1");

     toggle=GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_veff1"));
     if (gtk_toggle_button_get_active(toggle)) buffer=g_strdup_printf("-%s",buffer);
   }
   else{
     buffer=g_strdup("0");
   }

   assign(&veff_dv,buffer);

   assign(&kfus_dv,xfr_get_text(main_window,"spin_kfus"));
   assign(&nfus_dv,xfr_get_text(main_window,"spin_nfus"));

   /*wdisk*/
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_wdisk")))){
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_wfelastic"))))
       buffer=g_strdup("1");
     else
       buffer=g_strdup("2");

     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"rb_file17unformatted"))))
       buffer=g_strdup_printf("-%s",buffer);
     
     assign(&wdisk_dv,buffer);
   }
   else
     assign(&wdisk_dv,"0");

   /*bpm*/
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_bpm"))))
     assign(&bpm_dv,"1");
   
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_ldistrib"))))
     assign(&bpm_dv,"2");
   
   /*melfil*/
   i=xfr_option_menu_get_value(main_window,"opt_melfil");
   assign(&melfil_dv,g_strdup_printf("%i",i));

   /* CDCC */
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window,"check_cdcc"))))
     assign(&cdcc_dv,"1");
   else
     assign(&cdcc_dv,"0");
   
   /*Energy, incoming channel...*/
   assign(&pel_dv,xfr_get_text(main_window,"spin_pel"));
   assign(&exl_dv,xfr_get_text(main_window,"spin_exl"));
   assign(&lab_dv,xfr_get_text(main_window,"spin_lab"));
   i=xfr_option_menu_get_value(main_window,"opt_lin")+1;
   assign(&lin_dv,g_strdup_printf("%i",i));
   assign(&lex_dv,xfr_get_text(main_window,"spin_lex"));
   /*assign(&elab_dv,xfr_get_text(main_window,"entry_elab"));*/

   for (i=1;i<4;i++){
     gchar *elab_name=g_strdup_printf("elab%i",i);
     gchar *nlab_name=g_strdup_printf("nlab%i",i);
    /*  assign_n(&elab_dv,i-1,xfr_get_text(window_eintervals,elab_name)); */
/*      assign_n(&nlab_dv,i-1,xfr_get_text(window_eintervals,nlab_name)); */
     assign_n(&elab_dv,i-1,xfr_get_text(main_window,elab_name)); 
     assign_n(&nlab_dv,i-1,xfr_get_text(main_window,nlab_name)); 
    }
    assign_n(&elab_dv,3,xfr_get_text(main_window,"elab4"));

   /*g_print("Voy a escribir namelist fresco\n");*/
   writenl(&fresco,f,NONZERO);
   /*g_print("Namelist fresco written ok\n");*/
   fputs("\n",f);
   fflush(f);

   /*****************************************************************/
   /*                     NAMELIST PARTITION                        */
   /*****************************************************************/ 
 
   for(irow=0;irow<xfr_clist_get_rows(part_clist);irow++){
     gchar *buffer;
     gint stat_numb;
     /*states_clist=GTK_CLIST(get_notebook_child(states_nb,irow+1));*/
     states_clist=GTK_CLIST(gtk_clist_get_row_data(GTK_CLIST(part_clist),irow));
     stat_numb=xfr_clist_get_rows(states_clist);/*Number of states for this partition*/

     gtk_clist_get_text(part_clist,irow,PNUCLEUS,&buffer);
     assign(&namep_dv,buffer);
     
     gtk_clist_get_text(part_clist,irow,PMASS,&buffer);
     assign(&massp_dv,buffer);
     
     gtk_clist_get_text(part_clist,irow,PZ,&buffer);
     assign(&zp_dv,buffer); 
     
     gtk_clist_get_text(part_clist,irow,TNUCLEUS,&buffer);
     assign(&namet_dv,buffer); 
     
     gtk_clist_get_text(part_clist,irow,TMASS,&buffer);
     assign(&masst_dv,buffer);  
     
     gtk_clist_get_text(part_clist,irow,TZ,&buffer);
     assign(&zt_dv,buffer);   
     
     gtk_clist_get_text(part_clist,irow,QVALUE,&buffer);
     assign(&qval_dv,buffer);    

     gtk_clist_get_text(part_clist,irow,PWF1,&buffer);
     assign(&pwf1_dv,buffer);

     gtk_clist_get_text(part_clist,irow,XSEC,&buffer);
     if (g_strncasecmp(buffer,"F",1)==0)
       assign(&nex_dv,g_strdup_printf("-%i",stat_numb));
     else
       assign(&nex_dv,g_strdup_printf("%i",stat_numb));

     gtk_clist_get_text(part_clist,irow,READSTATES,&buffer);
     assign(&readstates_dv,buffer);
    
     writenl(&partition,f,NONZERO);

     if (ivalof(&readstates_dv)>0){
     g_print("For partition %i READSTATES>0, so I will not write states for this partition\n",irow+1);
     
     }
     /*Read states of this partition from states_clist in states_nb*/
     for (i=0;i<stat_numb;i++){
       gchar *buffer;
       enum {JP,COPYP,BANDP,EP,KKP,TP,CPOT,
	     JT,COPYT,BANDT,ET,KKT,TT,EXTRA,FEXCH,IGNORE,INFAM,OUTFAM};
       
       gtk_clist_get_text(states_clist,i,JP,&buffer);
       assign(&jp_dv,buffer);
      
       gtk_clist_get_text(states_clist,i,COPYP,&buffer);
       assign(&copyp_dv,buffer);
       
       gtk_clist_get_text(states_clist,i,BANDP,&buffer);
       assign(&bandp_dv,buffer);  
       
       gtk_clist_get_text(states_clist,i,EP,&buffer);
       assign(&ep_dv,buffer); 

       gtk_clist_get_text(states_clist,i,KKP,&buffer);
       assign(&kkp_dv,buffer); 

       gtk_clist_get_text(states_clist,i,TP,&buffer);
       assign(&tp_dv,buffer);    

       gtk_clist_get_text(states_clist,i,CPOT,&buffer);
       assign(&cpot_dv,buffer); 
       
       /* Disappeared in frxx.v9
       gtk_clist_get_text(states_clist,i,PWF,&buffer);
       assign(&pwf_dv,buffer);   
       */
       gtk_clist_get_text(states_clist,i,JT,&buffer);
       assign(&jt_dv,buffer);    

       gtk_clist_get_text(states_clist,i,COPYT,&buffer);
       assign(&copyt_dv,buffer); 

       gtk_clist_get_text(states_clist,i,BANDT,&buffer);
       assign(&bandt_dv,buffer);  

       gtk_clist_get_text(states_clist,i,ET,&buffer);
       assign(&et_dv,buffer) ;
       
       gtk_clist_get_text(states_clist,i,KKT,&buffer);
       assign(&kkt_dv,buffer); 
       
       gtk_clist_get_text(states_clist,i,TT,&buffer);
       assign(&tt_dv,buffer);    
       
       gtk_clist_get_text(states_clist,i,EXTRA,&buffer);
       assign(&extra_dv,buffer);    
       
       gtk_clist_get_text(states_clist,i,FEXCH,&buffer);
       assign(&fexch_dv,buffer);  
  
       gtk_clist_get_text(states_clist,i,IGNORE,&buffer);
       assign(&ignore_dv,buffer); 

       gtk_clist_get_text(states_clist,i,INFAM,&buffer);
       assign(&infam_dv,buffer); 
       
       gtk_clist_get_text(states_clist,i,OUTFAM,&buffer);
       assign(&outfam_dv,buffer); 

       /*       g_print("Voy a escribir namelist states...");*/

       /* METER AQUI FILE *g */
       if (ivalof(&readstates_dv)==0){
	 writenl(&states,f,NONZERO);
       }
       else{
	

       }

       /*g_print("ok\n");*/
       resetnl(&states);
     }
     fputs("\n",f);
   }
   fputs(" &partition /\n \n",f);
   fflush(f);
   
   /* g_print("Voy a escribir potentiales\n");*/
   /*****************************************************************/
   /*                          POTENTIALS                           */
   /*****************************************************************/ 
   for (irow=0;irow<xfr_clist_get_rows(pot_clist);irow++){
     enum{KP,TYPE,ITT,SHAPE,P1,P2,P3,P4,P5,P6,P7,ADD};
     gchar *type;
     gint itype;
     /*The step clist is attached to the row pointer data*/
     GtkWidget *step_clist=gtk_clist_get_row_data(pot_clist,irow);
     gtk_clist_get_text(pot_clist,irow,TYPE,&type);
     itype=atoi(type);

    
     if (itype==0){
       for (icol=0;icol<cou_pot.nvars;icol++){
       gchar *buffer;
       gtk_clist_get_text(pot_clist,irow,icol,&buffer);
       assign(cpot_vl[icol],buffer);
       /*g_print("buffer=%s\n",buffer);*/
       }
       
       writenl(&cou_pot,f,NONZERO);
     }
     else{
       for (icol=0;icol<spot.nvars;icol++){
       gchar *buffer;
       gtk_clist_get_text(pot_clist,irow,icol,&buffer);
       /* g_print("buffer=%s\n",buffer);*/
       assign(spot_vl[icol],buffer);
       }
       gtk_clist_get_text(pot_clist,irow,ADD,&buffer);
       if (g_strncasecmp(buffer,"T",1)==0 && ivalof(&type_dv)>0)
	 assign(&type_dv,g_strdup_printf("-%s",valof(&type_dv)));
       writenl(&spot,f,NONZERO);
     }

     /*Step namelist*/
     if (step_clist!=NULL){
       for (i=0;i<xfr_clist_get_rows(step_clist);i++){
	 gchar *buffer;
	 enum{IB,IBDESC,IA,IADESC,K,STR};

	 gtk_clist_get_text(GTK_CLIST(step_clist),i,IB,&buffer);
	 assign(&step_ib_dv,buffer);

	 gtk_clist_get_text(GTK_CLIST(step_clist),i,IA,&buffer);
	 assign(&step_ia_dv,buffer);

	 gtk_clist_get_text(GTK_CLIST(step_clist),i,K,&buffer);
	 assign(&step_k_dv,buffer);

	 gtk_clist_get_text(GTK_CLIST(step_clist),i,STR,&buffer);
	 assign(&step_str_dv,buffer); 

	 writenl(&step,f,NONZERO);
	 resetnl(&step);
       }
       fputs(" &step / \n \n",f);
       fflush(f);
     }
   }
   fputs(" &pot / \n \n",f);
   fflush(f);

   /*****************************************************************/
   /*                           OVERLAPS                            */
   /*****************************************************************/
   for (irow=0;irow<xfr_clist_get_rows(overlap_clist);irow++){
     enum {KN1,KN2,IC1,IC2,IN,KIND,CH1,NN,L,LMAX,SN,IA,J,
	   IB,KBPOT,KRPOT,BE,ISC,IPC,
	   NFL,NAM,AMPL,DM,NK,ER};      
     for (icol=0;icol<xfr_clist_get_columns(overlap_clist);icol++){
       gchar *buffer;
       gtk_clist_get_text(GTK_CLIST(overlap_clist),irow,icol,&buffer);
       assign(overlap_vl[icol],buffer);
     }
     writenl(&overlap,f,NONZERO);
   }
   fputs(" &overlap / \n \n",f);
   fflush(f);

   /*****************************************************************/
   /*                           COUPLINGS                           */
   /*****************************************************************/
   
   /*To avoid some redeclarations I perform this in a local way*/
   write_coupling(main_window,f);

   /* Acknowledgment and date */
   timet=time(NULL);
   date=ctime(&timet);
   fprintf(f,"Output code for fresco input written by xfresco version 2.1\n at %s \n",
	   date);

   /*End of writting*/
   fclose(f);
   gtk_statusbar_pop( GTK_STATUSBAR(statusbar),contid);
   g_print("Saving file %s\n\n",filename);
}

/** callbacks.c den buraya aldım...**/
void
kill_current_run (GtkWidget *main_window)
{
  if(idchild>0){
    /*    gchar *buffer=g_strdup("kill %i",idchild);*/
    g_print("Killing process %i...",idchild);
    if (system(g_strdup_printf("kill %i",idchild))){
      g_print("succeed\n\n");
      idchild=-1;
    }
    else{
      g_print("failed!\n\n");
    }
  }
  else{ 
    g_print("Umm..sorry. I found no running process\n\n");
  }

}


/** callbacks.c den buraya aldım...**/
void
run_file(GtkWidget *main_window,const gchar *filename)
{ 
  
  gchar *command;
  gchar *message;
  gint itest,return_value;
  gint pid;
  gint retval;
  guint context;
  gchar *outfile;
  GtkWidget *statusbar=lookup_widget(main_window,"statusbar");
 

  /* gint context_id=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),\
						"Statusbar");*/
  /* gchar *tmpfile; */
  /* gchar *echotest; */

  /* Build outfile name from current_filename */
  gchar *buffer=g_strdup(current_filename);
  g_strreverse(buffer);
  if (g_strncasecmp(buffer,"ni.",3)==0){
    gchar *tmp=g_strndup(current_filename,strlen(current_filename)-3);
    outfile=g_strdup_printf("%s.out",tmp);
  }
  else{
    outfile=g_strdup_printf("%s.out",current_filename);
  }
  
   context=gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar),
				       "run");

  /* Create temporal file with current information */
  /* tmpnam(tmpfile);*/
  /* g_print("Fichero temporal=%s",tmpfile);*/
  /* real_save_file (main_window, tmpfile); */
  /* g_print("tmp file saved ok\n");*/

  /* Command to execute fresco */
  command=g_strdup_printf("runfresco %s %s\n",current_filename,outfile);
  /*command=g_strdup_printf("fresco<%s>%s",tmpfile,outfile);*/

  
  /*if (FRESCO_IS_RUNNING==TRUE){
    g_warning("fresco is already running\n");
    return;
  }*/

  if ((pid=fork())<0){
    g_warning("Could not fork to execute fresco\n");
    return;
  }

  /*if (wait(NULL) !=pid)
     g_print("Child is running\n");*/

  if (pid==0){
    FRESCO_IS_RUNNING=TRUE;
    /* Child */
    /* printf("CHILD id is %d and return value is %d\n",
	   getpid(),return_value);*/

    g_print("Executing fresco with command:\n%s\n\n",command);
    /*g_print("%s\n",command);*/
    message=g_strdup("Executing fresco...");
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), context, message);
    /*retval=system(command);*/
    retval=execlp("runfresco","runfresco",current_filename,outfile,0);
    /*FRESCO_IS_RUNNING=FALSE;*/


    /* Check returned value */
    if (retval!=itest){
      g_print("**ERROR**:run fresco failed!!\n");
      message=g_strdup("fresco execution failed!");
      gtk_statusbar_push(GTK_STATUSBAR(statusbar), context, message);
    }else{
      g_print("\nFRESCO executed ok\n");
      message=g_strdup("fresco executed OK");
      gtk_statusbar_push(GTK_STATUSBAR(statusbar), context, message);
    }

     g_free(message);
     g_free(command);
     g_free(outfile);
     exit(0);
	
  }
  else{
     FRESCO_IS_RUNNING=TRUE;
    /* Parent */
    /* printf("PARENT id is %d\n",getpid()); */
     /* DIRTY TRICK!!!!!! */
     idchild=getpid()+2;
  }

  /*g_free(message);*/
  /*g_free(command);*/
  /*g_free(outfile);*/ 
  return;
}





/* WRITE COUPLINGS */

void 
write_coupling(GtkWidget *main_window,FILE *f){  
  GtkCList *coup_clist=GTK_CLIST(lookup_widget(main_window,"coup_clist"));
  gint irow,icol;
  gint i;
		
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
  /*Vardesc cfp_keep_dv=define("keep","0",XFR_NUMBER);*/


  /*Variables for namelist: SCALE*/
  /* Vardesc qscale0_dv=define("qscale(0)","0",XFR_NUMBER);
  Vardesc qscale1_dv=define("qscale(1)","0",XFR_NUMBER);
  Vardesc qscale2_dv=define("qscale(2)","0",XFR_NUMBER);
  Vardesc qscale3_dv=define("qscale(3)","0",XFR_NUMBER);
  Vardesc qscale4_dv=define("qscale(4)","0",XFR_NUMBER);
  Vardesc qscale5_dv=define("qscale(5)","0",XFR_NUMBER);
  Vardesc qscale6_dv=define("qscale(6)","0",XFR_NUMBER);
  Vardesc qscale7_dv=define("qscale(7)","0",XFR_NUMBER);
  Vardesc qscale8_dv=define("qscale(8)","0",XFR_NUMBER);
  Vardesc qscale9_dv=define("qscale(9)","0",XFR_NUMBER);
  Vardesc qscale10_dv=define("qscale(10)","0",XFR_NUMBER);
  Vardesc qscale11_dv=define("qscale(11)","0",XFR_NUMBER);*/

  /*Array of variables for namelist coupling*/
  Vardesc *coupling_vl[]={&icto_dv,  &icfrom_dv,  &kind_dv,
			  &ip1_dv,   &ip2_dv,     &ip3_dv,
			  &p1_dv,    &p2_dv,      
			  &rmax_dv,  &jmax_dv,
			  &kfrag_dv, &kcore_dv};

     
   /*Idem for namelist inel*/
   Vardesc *inel_vl[]={&inel_ib_dv,&inel_ia_dv,&inel_k_dv,
		       &inel_no_dv,&inel_kp_dv,&inel_a_dv};


   /*Idem for namelist cfp*/
   Vardesc *cfp_vl[]={&cfp_in_dv, &cfp_ib_dv, &cfp_ia_dv,
		      &cfp_kn_dv, &cfp_a_dv };

   /*Idem for namelist scale*/
   /*   Vardesc *scale_vl[]={&qscale_dv,};*/

   Namelist coupling={"COUPLING",coupling_vl,12};
   Namelist inel={"INEL",inel_vl,6};
   Namelist cfp={"CFP",cfp_vl,5};
   /* Namelist scale={"SCALE",scale_vl,12};*/

   enum{INEL_IB,INEL_IA,INEL_K,INEL_NO,INEL_KP,INEL_A};/*inel_clist */
   enum{CFP_IN,CFP_IB,CFP_IA,CFP_KN,CFP_A}; /* cfp_clist*/

 
   for (irow=0;irow<xfr_clist_get_rows(coup_clist);irow++){
     GtkWidget *data_clist=gtk_clist_get_row_data(coup_clist,irow);
     gint kind,ip3,ip1;
     for (icol=0;icol<xfr_clist_get_columns(coup_clist);icol++){
       gchar *buffer;
       gtk_clist_get_text(coup_clist,irow,icol,&buffer);
       assign(coupling_vl[icol],buffer);
     }
     kind=ivalof(&kind_dv);/*Kind of coupling*/
     ip3=ivalof(&ip3_dv);
     ip1=ivalof(&ip1_dv);
     if (kind<3 && data_clist!=NULL)
       assign(&ip1_dv,g_strdup_printf("%i",xfr_clist_get_rows(data_clist)));
     writenl(&coupling,f,NONZERO);

     
     /*Table of couplings*/
     switch (kind){
     case 1: case 2: 
 	g_assert(data_clist!=NULL);
	for (i=0;i<xfr_clist_get_rows(data_clist);i++){
	  gchar *text;
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,INEL_IB,&text);
	  assign(&inel_ib_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,INEL_IA,&text);
	  assign(&inel_ia_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,INEL_K,&text);
	  assign(&inel_k_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,INEL_NO,&text);
	  assign(&inel_no_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,INEL_KP,&text);
	  assign(&inel_kp_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,INEL_A,&text);
	  assign(&inel_a_dv,text); 

	  writenl(&inel,f,NONZERO);
	  resetnl(&inel);
	}
	/*I think this is not neccesary but I'm not sure*/
	fputs(" &INEL/ \n \n",f);
	break;

     case 3: case 4: case 5: case 6: case 7: case 8:/*cfp*/
        if (data_clist!=NULL){
	for (i=0;i<xfr_clist_get_rows(data_clist);i++){
	  gchar *text;
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,CFP_IN,&text);
	  assign(&cfp_in_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,CFP_IB,&text);
	  assign(&cfp_ib_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,CFP_IA,&text);
	  assign(&cfp_ia_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,CFP_KN,&text);
	  assign(&cfp_kn_dv,text);
	  
	  gtk_clist_get_text(GTK_CLIST(data_clist),i,CFP_A,&text);
	  assign(&cfp_a_dv,text);
	  
	  /* gtk_clist_get_text(GTK_CLIST(data_clist),i,CFP_KEEP,&text);
	  assign(&cfp_keep_dv,text);*/ 

	  writenl(&cfp,f,NONZERO);
	  resetnl(&cfp);
	}
        }
	fputs(" &cfp / \n \n",f);

	/* QSCALE */
	if ((kind==3 || kind==4) && ip3>9){
	  xfr_qscale_write(f, irow, abs(ip1));
	}

	break;
     }     
     resetnl(&coupling);
   }   
   fputs(" &coupling / \n \n",f);
   fflush(f);
}




/* &Scale
          qscale(0) = (   0.100    ,   1.000    ) /
*/
void 
xfr_qscale_write(FILE *f, gint row, gint qmax){
  gint iq;
  fprintf(f," &Scale ");
  for(iq=0;iq<=qmax;iq++){
    gint col=2*iq;
    gchar *nr;
    xfr_clist_get_text(GTK_CLIST(qscale_clist),row,2*iq,&nr);
    gchar *ni;
    xfr_clist_get_text(GTK_CLIST(qscale_clist),row,2*iq+1,&ni);
    fprintf(f,"    qscale(%i)=(%s,%s)\n",iq,nr,ni);
  }
  fprintf(f,"/ \n");
}









