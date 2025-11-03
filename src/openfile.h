gint 
real_open_file (GtkWidget *main_window, const gchar *file);


void
put_partition(GtkWidget *partitions, Namelist *);

void
put_states(GtkWidget *,Namelist *);


GtkWidget *
append_states_page(GtkWidget *,gint);



void
put_potential(Namelist *,GList **);


gint
read_couplings(FILE *f);



























