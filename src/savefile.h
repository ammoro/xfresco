#include <stdio.h>
#include "xfresco.h"

void
save_as (GtkWidget *main_window);

void
real_save_file(GtkWidget *main_window,const gchar *filename);

/** bunlar savefile.c ye save+run için eklendi. **/
/**-------------------19 Eylül 2007 ---------------**/
void
kill_current_run (GtkWidget *main_window); 

void
run_file(GtkWidget *main_window,const gchar *filename);
/**--------------------------------------------------**/
void 
write_coupling(GtkWidget *ref,FILE *f);


void 
xfr_qscale_write(FILE *f, gint row, gint qmax);
