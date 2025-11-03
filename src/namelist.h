#include <stdio.h>


/*Writting mode*/
enum {ALL,
      NONZERO};


typedef struct _Vardesc Vardesc;


struct _Vardesc
{
  GArray *garray;/*GArray to hold an array of GStrings with the values */
  GArray *carray; /* Garray to store imaginary part */
  gchar *name; /*Name of the variable*/
  gchar *deflt;/*Default value*/
  gint start; 
  gint end;
  gint dim; /* v(start:end,dim) */
  gboolean read;/* TRUE=variable has been found in input file */
  guint type;
};


typedef struct _Namelist Namelist;

struct _Namelist
{
  gchar *name;
  Vardesc **vars;
  gint nvars;
};


/* Not used now, but could be used in future for complex variables */
typedef struct _complex Complex; 

struct _complex
{
  gchar *real;
  gchar *imag;
};
/* ------------ */

gint
readnl(Namelist *, FILE *);

void
resetnl(Namelist *nl);


gint
writenl(Namelist *mylist,FILE *f,guint mode);

gint
idchar(gint *);

Vardesc 
define(gchar *name,gchar *deflt,guint type);

Vardesc 
define2(gchar *name,gchar *deflt,guint type);

Vardesc 
define_array(gchar *name,gchar *deflt,gint i1,gint i2,gint step,guint type);

Vardesc
define_carray(gchar *name,gchar *value,gint dim1, gint dim2, gint dim,guint type);

gchar *
valof(Vardesc *);

gchar *
valof_n(Vardesc *var_dv,gint n);

gchar *
valof_nimag(Vardesc *var_dv,gint n);

gchar *
abs_valof(Vardesc *);

gint 
dimof(Vardesc *var_dv);

gint
ivalof(Vardesc *);

void
assign(Vardesc *,gchar *);

void
assign_n(Vardesc *,gint n,gchar *);

gint
read_word(FILE *f,const gchar **word,guint type);

gint
read_real(FILE *f, const gchar **real, guint tipo );


/* 23/07/2007 */
/* gint
   read_word(FILE *f,gchar **word,guint type); 

gint
read_real(FILE *f, gchar **real, guint tipo ); */

gint
read_complex(FILE *f, gchar **real, gchar **imag);

gint
read_complex2(FILE *f, gchar **real, gchar **imag);

gint
read_g(FILE *f,gchar **word);

void
read_dims(FILE *f,gint *i1,gint *i2,gint *istep);

void
freenl(Namelist *nl);


guint
check_max_column(gint columnas, FILE *f);

gint
too_many_warnings();
