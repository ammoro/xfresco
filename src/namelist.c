#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <gtk/gtk.h>
#include "xfresco.h"
#include "namelist.h"



extern void glade_util_show_message_box(gchar *cadena);
extern GtkWidget*
glade_util_create_dialog_with_buttons (const gchar * message,
				       gint nbuttons,
				       const gchar * buttons[],
				       gint default_button,
				       GtkSignalFunc signal_handlers[],
				       gpointer data);
extern void on_confirm_exit(GtkWidget *button,gpointer data);

extern gint nwarnings;

enum {WHITE,
      SLASH,
      EQUAL,
      JUMP,
      ENDFILE,
      OTHER};

extern void xfr_print(const gchar *cadena,...);

/*enum{OK,EMPTY,END_OF_LIST};*/

/*Creates a variable of type Vardesc with name "name" and initial value "deflt"
 *The variable consits of a structure with 3 fields: 
 * -> var: the value of the variable
 * -> name: the name of the variable
 * -> deflt: the initial value(default)
 */
Vardesc 
define(gchar *name,gchar *value,guint type){
  GArray *garray=g_array_new(FALSE,TRUE,sizeof(GString));
  GString *gstr=g_string_new(value);
  gint dims[]={1,1,0};
  Vardesc var_dv;

  g_array_append_val(garray,*gstr);

  var_dv.garray=garray;
  var_dv.name=name;
  var_dv.deflt=value;/* default value */
  var_dv.start=dims[0];
  var_dv.end=dims[1];
  var_dv.dim=dims[2];
  var_dv.read=FALSE;
  var_dv.type=type;
  return(var_dv);
}

Vardesc 
define2(gchar *name,gchar *value,guint type){
  GArray *garray=g_array_new(FALSE,TRUE,sizeof(GString));
  GArray *carray=g_array_new(FALSE,TRUE,sizeof(GString));
  GString *gstr=g_string_new(value);
  GString *gstr2=g_string_new(value);
  gint dims[]={1,1,0};
  /*gint dims[]={0,0,0};*/
  Vardesc var_dv;

  g_array_append_val(garray,*gstr);
  g_array_append_val(carray,*gstr2);

  var_dv.garray=garray;
  var_dv.carray=carray;
  var_dv.name=name;
  var_dv.deflt=value;/* default value */
  var_dv.start=dims[0];
  var_dv.end=dims[1];
  var_dv.dim=dims[2];
  var_dv.read=FALSE;
  var_dv.type=type;
  return(var_dv);
}


/*Idem to "define", but for dimensioned variables -> v(dim1:dim2,dim)*/
Vardesc
define_array(gchar *name,gchar *value,gint dim1, gint dim2, gint dim,guint type){
  gint i;
  Vardesc var_dv;
  GArray *garray=g_array_new(FALSE,TRUE,sizeof(GString));
  gint nvals=dim2-dim1+1;
 
  for (i=0;i<nvals;i++){
    GString *gstr=g_string_new(value);
    g_array_append_val(garray,*gstr);  
  }

  var_dv.garray=garray; 
  var_dv.name=g_strdup(name);
  var_dv.deflt=g_strdup(value);
  var_dv.start=dim1;
  var_dv.end=dim2;
  var_dv.dim=dim;
  var_dv.read=FALSE;
  /*  var_dv.type=XFR_NUMBER; */
  var_dv.type=type;
  return(var_dv);
}



Vardesc
define_carray(gchar *name,gchar *value,gint dim1, gint dim2, gint dim,guint type){
  gint i;
  Vardesc var_dv;
  GArray *garray=g_array_new(FALSE,TRUE,sizeof(GString));
  GArray *carray=g_array_new(FALSE,TRUE,sizeof(GString));
  gint nvals=dim2-dim1+1;
 
  xfr_print("Array:%s\n",name);
  xfr_print("Default values %s\n",value);
  xfr_print("Numero de variables: %i\n",nvals);
  xfr_print("dims(%i:%i,%i)\n=",dim1,dim2,dim);

  for (i=0;i<nvals;i++){
    GString *gstr=g_string_new(value);
    GString *cgstr=g_string_new(value);
    g_array_append_val(garray,*gstr);
    g_array_append_val(carray,*cgstr);
  }

  var_dv.garray=garray; 
  var_dv.carray=carray;
  var_dv.name=g_strdup(name);
  var_dv.deflt=g_strdup(value);
  var_dv.start=dim1;
  var_dv.end=dim2;
  var_dv.dim=dim;
  var_dv.read=FALSE;
  var_dv.type=type;
  return(var_dv);
}




/*Returns the value hold by the variable var_dv of type Vardesc*/
gchar *
valof(Vardesc *var_dv){ 
  GString *gstring;
  GArray *garray=var_dv->garray;

  gstring=&g_array_index(garray,GString,0);	
  return(gstring->str);
}

/*Returns the abs value hold by the variable var_dv of type Vardesc*/
gchar *
abs_valof(Vardesc *var_dv){ 
  GString *gstring;
  gchar *buffer;
  gchar *minus="-";
  GArray *garray=var_dv->garray;

  gstring=&g_array_index(garray,GString,0);
  buffer=g_strdup(gstring->str);

/*  if (g_strcasecmp(g_strndup(buffer,1),minus)==0) */
  if (g_ascii_strcasecmp(g_strndup(buffer,1),minus)==0) 
    g_string_erase(gstring,0,1);
  /*
  g_free(minus);
  g_free(buffer);*/
  return(gstring->str);
}


/*Converts to integer the value allocated by var_dv*/
gint
ivalof(Vardesc *var_dv){
  gint i=atoi(valof(var_dv));
  return(i);
}

/* Returns the value of the nth variable allocated in the variable var_dv*/
gchar *
valof_n(Vardesc *var_dv,gint n){
  GArray *garray=var_dv->garray;
  GString *gstring;
/*  g_return_if_fail(garray!=NULL); */
    g_return_val_if_fail(garray!=NULL,"1");

  if (garray->len <n) return("0");
  gstring=&g_array_index(garray,GString,n);
  return(gstring->str);
}

/* Returns the value of the IMAG part of the nth variable allocated in the variable var_dv*/
gchar *
valof_nimag(Vardesc *var_dv,gint n){
  GArray *carray=var_dv->carray;
  GString *gstring;
  /*  g_return_if_fail(garray!=NULL); */
    g_return_val_if_fail(carray!=NULL,"1");
  if (carray->len <n || carray==NULL ) return("0");
  gstring=&g_array_index(carray,GString,n);
  return(gstring->str);
}

gint
dimof(Vardesc *var_dv){
  gint nvals;
  
  gint imin=var_dv->start;
  gint imax=var_dv->end;
  /*gint step=var_dv->step;*/
  nvals=(imax-imin)+1;  
  return(nvals);
}



/*Assigns a value to a variable of type Vardesc
 *The value is stored in the field "var" of the structure*/
void
assign(Vardesc *var_dv,gchar *val){
  GArray *garray=var_dv->garray;
  GString *gstring=&g_array_index(garray,GString,0);
  g_string_assign(gstring,val);
}

/*Assigns a value to a element i of an array of type Vardesc
 *The value is stored in the field "var" of the structure*/
void
assign_n(Vardesc *var_dv,gint i,gchar *val){
  GArray *garray=var_dv->garray;
  GString *gstring=&g_array_index(garray,GString,i);
  g_string_assign(gstring,val);
}

/*Resets the values of the namelist "nl" to their default values*/
void
resetnl(Namelist *nl){
  gint i;
  Vardesc **varlist=nl->vars;
  
  for(i=0;i<nl->nvars;i++){
    gint j;
    Vardesc *vd=*(varlist+i);
    GArray *garray=vd->garray;
    GArray *carray=vd->carray;
    gchar *deflt=g_strdup(vd->deflt);
    
    vd->read=FALSE;


    for(j=0;j<garray->len;j++){
      GString *gstring=&g_array_index(garray,GString,j);
      /* GString *gstring2=NULL;*/

      if (vd->type==XFR_COMPLEX){
	g_print("\nResetting component %i of variable %s",j,vd->name);
	GString *gstring2=&g_array_index(carray,GString,j);
	g_string_assign(gstring2,deflt); 
      }

      /*
	xfr_print("Variable:%s Actual value:%s Default:%s\n",\
	(**(varlist+i)).name,var->str,deflt);*/
      g_string_assign(gstring,deflt);
     
    }
  }
}


/*This function searches and assign the variables of a given namelist 
  on a given file.
  Features:
  - It returns one of the following:
       {LIST_READ_OK,EMPTY_LIST,LIST_NOT_FOUND,END_OF_FILE} 
  - The function ignore possible quotes in variable value.
  - The search is case insesitive for both the namelists and variables.
  */
gint
readnl(Namelist *mylist,FILE *f){
  gint i=-1;
  gint iread,c;
  guint type=XFR_NUMBER;
  gint VAR_IN_LIST=FALSE;
  gint EndOfList=FALSE;
  gint EndOfFile=FALSE;
  gint LISTFOUND=FALSE;
  gint EmptyList=TRUE;
  const gchar *listname=g_strdup(mylist->name);
  Vardesc **varlist=mylist->vars;
  /* gchar *namevar=NULL;*/
  const  gchar *word=g_strdup("");
 
  GString *bufitem=g_string_new("");
  
  /*Search for beginning of namelist,ie, the character "&" */
  xfr_print("\n readnl:searching for list: <%s> \n",listname);
  while((c=fgetc(f))!=EOF){
    if (c=='&') {
      /*Read the name of the found namelist */  
      i=read_word(f,&word,XFR_STRING);
      xfr_print("readnl: list found:<%s>\n",word);
      
      /*      if (g_strcasecmp(word,listname)!=0){ */
      if (g_ascii_strcasecmp(word,listname)!=0){
	gchar *buffer=g_strdup_printf("WARNING!!:Searching for list %s but list %s found instead.\n I will ignore this namelist and continue reading.",
				      listname,word);
        xfr_print("%s\n",buffer);
	nwarnings+=1;
        if (nwarnings>MAXERRORS) {
	  g_print("Too many errors. Aborting!");
	  exit(1);
	}
	glade_util_show_message_box(buffer);
      }
      else{
	xfr_print("...found\n  ");
	LISTFOUND=TRUE;
	break;
      }
    }
  }
  
  if (LISTFOUND==FALSE){
/*	if (g_strcasecmp(listname,"COUPLING")==0){  */
    if (g_ascii_strcasecmp(listname,"COUPLING")==0){
      g_warning("End of file trying to read COUPLING namelist\n");
    }
    else{
      gchar *buffer=g_strdup_printf("WARNING!!:Could not find namelist <%s>",
				    listname);
      nwarnings+=1;
      if (nwarnings>MAXERRORS) {
	g_print("Too many errors. Aborting!");
	exit(1);
      }
      /* glade_util_show_message_box(buffer); */
      g_warning("Could not find namelist <%s>",listname);
    }
    return(LIST_NOT_FOUND);
  }


  if (i=='/') return(EMPTY_LIST); /*Empty List: ej. &pot/ */
    
  /* Read variables */
  while(EndOfList==FALSE){	 
    switch((iread=read_word(f,&word,XFR_STRING))){
    case EOF:
      EndOfFile=TRUE;
      g_error("\nEnd of file reached!!");
      g_free(bufitem);
      return(END_OF_FILE);
      break;

    case '/':
      /*End of list*/
      EndOfList=TRUE;
      if (EmptyList==TRUE){
	/*xfr_print("...end of empty list found \n");*/
	return(EMPTY_LIST);
      }
      else{
	/*xfr_print("... end of non-empty list found\n");*/
	/*xfr_print("Voy a revolver %i\n",LIST_READ_OK);*/
	return(LIST_READ_OK);
      }
      break;
     
     
    case ' ':
      /* g_print("Variable name was followed by ws\n");*/
       while((c=fgetc(f))!='='){
	 if (c==EOF){
	   g_error("End of file found trying to get character '='\n");
           exit(0);
	 }
       }
      
    default:
      /*At this point the variable name has just been read*/
      /*xfr_print("variable read:%s",word);*/
      EmptyList=FALSE;
      g_string_sprintf(bufitem,word);
      break;
    }
 
    
    /*Search this variable in namelist*/
    VAR_IN_LIST=FALSE;
    for(i=0;i<mylist->nvars;i++){
      gint imin=1;gint imax=1;gint dim=1;
      /*guint type;  type of variable */
      guint alpha;
      Vardesc *vd=*(varlist+i);
      gchar *namevar=g_strdup(vd->name);
     
      type=vd->type;
     
      /* namevar=vd->name;*/
      if (g_strcasecmp(namevar,bufitem->str)==0){
	GArray *garray=vd->garray;
	gint j,nvals;
	EmptyList=FALSE;

	if (iread=='('){
	  read_dims(f,&imin,&imax,&dim);
	  if (imin==-1) imin=vd->start;
	  if (imax==-1) imax=vd->end;
	  if (dim==-1) dim=vd->dim;
	  /* xfr_print("read dims:%s(%i:%i,%i)\n", namevar,imin,imax,dim); */

	  /* COMENTED IN V. 1.5 */
	  /* vd->start=imin; */
/* 	  vd->end  =imax; */
/* 	  vd->dim =dim; */

	}
	else{
	  /* if (imin:imax,inc) is not given we assume default values*/
	  imin=vd->start;
	  imax=vd->end;
	  dim=vd->dim;
	}
	vd->read=TRUE;/* mark the variable to know that it has been read*/

	nvals=(imax-imin)+1;

	if (nvals>garray->len){
	  nwarnings+=1;
	  if (nwarnings>MAXERRORS) too_many_warnings();

	  g_print("\n**WARNING** Forced to read %i vals for var %s, \
     but dimension of variable only allows %i\n",nvals,vd->name,garray->len);
	  nvals=garray->len;
	}
	
	if (nvals>1)
	xfr_print("\nVoy a leer %i valores para la variable:%s: ",\
		  nvals,namevar);
	/*if (nvals>1){ */
	if (garray->len>1){
	  if (dim>0)
	    xfr_print("%s(%i:%i,%i)=",namevar,imin,imax,dim);
	  else
	    xfr_print("%s(%i:%i)=",namevar,imin,imax);
	  alpha=FALSE;
	}
	else{
	  imin=1;imax=1;
	  xfr_print("%s=",namevar);
	  alpha=TRUE;
	}

	/* if(nvals>1)g_print("Reading %i values for %s\n",nvals,namevar); */

       /*Now read value(s) for this variable*/
	for (j=0;j<nvals;j++){
	  GString *var = &g_array_index(garray,GString,j+imin-(vd->start));
	  gint retchar=0;
	 
	  if (EndOfList==TRUE) break;

	  /* ------------------------------------------------  */	 
	  if (vd->type==XFR_COMPLEX){
	   GArray *carray=vd->carray; 
	   GString *cvar= &g_array_index(carray,GString,j+imin-(vd->start)); 
	   gchar *real=g_strdup(vd->deflt);
	   gchar *imag=g_strdup("0.0");
	    /*retchar='a';*/
	    retchar=read_complex(f,&real,&imag);
	    /* g_print("vd->start=%i\n=",vd->start);*/
	    /* g_print("(%s,%s)",real,imag); */ 
	    /* g_print("\ncvar->str=%s",cvar->str); */
	    /* g_print("\n lens=%i,%i",garray->len,carray->len); */
	    g_string_assign(var,real);
	    g_string_assign(cvar,imag); 

	   /*  if (carray->len>j+1) */
/* 	          g_string_assign(cvar,imag);  */
/* 	    else */
/* 	      g_print("ERROR "); */	   

	    /* This should be equivalent to: */
	    /*assign_n(vd,j+imin-(vd->start),real); */
		     
/* 	    cmplx->real=g_strdup(real); */
/* 	    cmplx->imag=g_strdup(imag); */

/*	    xfr_print("(%s,%s) ",var->str,cvar->str);*/
	    VAR_IN_LIST=TRUE;
	  }
	  else if ((vd->type==XFR_STRING)|| (vd->type==XFR_DELSTR)){
	    retchar=read_word(f,&word,vd->type);
	    g_string_assign(var,word);
	    xfr_print("%s",var->str);
	     VAR_IN_LIST=TRUE;
	  }
	  else{
	    retchar=read_real(f,&word,type);  /*gfloat *number;*/
	    /* fscanf(f,"%f",&number); */
	    /* word=g_strdup_printf("%f",number); */
	    if (nvals>1 && isalpha(retchar)!=0) break;
	    g_string_assign(var,word);
	    if (nvals>1){
	      xfr_print("\n j=%i => <%s> ",j,var->str);
	    }else{xfr_print("%s ",var->str);}
	    VAR_IN_LIST=TRUE;
	  }
	  /* -------------------------------------------------- */


	  /* -------------------------------------------------- */
	  switch(retchar){
	  case (EOF):
	    EndOfFile=TRUE;
	    nwarnings+=1;
	    if (nwarnings>MAXERRORS) too_many_warnings();
	    g_warning("End of file found trying to \
read value for variable %s",\
		    bufitem->str);
	    g_free(bufitem);
	    return(END_OF_FILE);
	    break;

	  case ('/'):
	    VAR_IN_LIST=TRUE;
	    EndOfList=TRUE;
	    /*g_print("End of namelist found");*/
	    break;

	  default:
	    /*if (nvals>1)g_print("retchar=%c\n",retchar);*/
	    if (nvals>1 && isalpha(retchar)!=0) break;
	  
	  }
	 /* -------------------------------------------------- */
	 

	  /* Si nvals>1 y el caracter leido es una letra */
	  if (nvals>1 && isalpha(retchar)!=0) {
	    /* g_print("\n Alphanumeric character %c \n",retchar); */
	    /* xfr_print("Caracter devuelto leyendo array:<%c>\n",c);*/
	    break;
	  } /* nvals */
	} /* for (j=0;nvals) */
      } /* s_sgrcomp */
    } /* for (i=0, nvars */
  

    if (VAR_IN_LIST==FALSE) {
      gint j,nvals;
      gint imin=1;gint imax=1;gint dim=1;
      nwarnings+=1;
      if (nwarnings>MAXERRORS)
	if (too_many_warnings()==0) return(-1);
      g_warning("Variable '%s' not in list\n",bufitem->str);
      /*Anyway, we have to read the value */
      if (iread=='(') read_dims(f,&imin,&imax,&dim);
      
      /*nvals=(imax-imin)/dim+1*/;

      if (dim==0){
	g_warning("Dim=0 for variable %s=",bufitem->str);
	g_print("%s(%i,%i,%i)\n",\
		bufitem->str,imin,imax,dim);
	dim=1;
      }
	
      nvals=(div(imax-imin,dim).quot)+1;
      for (j=0;j<nvals;j++) read_word(f,&word,type);      
    }


    g_string_assign(bufitem,"");
}  /* readnl */

  /*Free all allocated variables*/
  g_string_free(bufitem,TRUE);

  return(LIST_READ_OK);
}





/* -------------------------------------------------- */
gint 
read_real(FILE *f, const gchar **real,guint type){
  gint c;
  gint retchar;
  GString *gsreal=g_string_new("");
  guint READ=FALSE;
  gint QUOTE_FOUND=FALSE;
  guint RETVAL=-2;
  guint rdigit=FALSE;
  
  while((c=fgetc(f))!=EOF){
    switch(c){
    case EOF: 
      return(END_OF_FILE);
      break;

    case '\'':
      if (QUOTE_FOUND==FALSE) {
	QUOTE_FOUND=TRUE;
      }
      else{
	RETVAL=OTHER;
      }
      break;
      
    case '/':
      /* EndOfList=TRUE; */
      /*xfr_print(" End of list found");*/
      /* if (EmptyList==TRUE) return(EMPTY_LIST); */
      /* g_string_assign(var,word); */
      return(c);
      break;
	    
    case '=':
      g_print("\nInternal ERROR: '=' found after\n ");
      g_print("reading value of variable %s\n. Please\n",real);
      g_print("report to moro@us.es together with\n");
      g_print("the input that caused the error\n");
      exit(0);
      break;

    case '\n':
      if (READ==TRUE){ 
	*real=g_strdup(gsreal->str);
	return(c);
      }
      /* g_print("\nSalto de linea pero no he leido nada:<%s>",	\
	 g_strdup(gsreal->str));*/
      break;

      
    case ' ':
      if (READ==TRUE){ 
	*real=g_strdup(gsreal->str);
	return(c);
      }
      break;
      
    default:	 
      if (iscntrl(c)!=0){
	xfr_print("\nCaracter de control:<%c>-><%i> after word <%s>...",c,c,
		g_strdup(gsreal->str));
	if (READ==TRUE) {return(c);} else {break;}
      }
 
      if (isdigit(c)!=0) rdigit=TRUE; /*A digit character has been read*/ 
      if (isalpha(c)!=0 && type!=XFR_STRING && type!=XFR_DELSTR \
	  && type!=XFR_BOOLEAN){ 
	if ((c=='e' || c=='E' || c=='d' || c=='D') && rdigit==TRUE){
	
	/* We have read exponential notation character :e,E,d,D*/
	}
	else{
	  ungetc(c,f);
	  *real=g_strdup(gsreal->str);
	  nwarnings+=1;
	  if (nwarnings>MAXERRORS) too_many_warnings();
	  g_warning("Read unexpected character `%c' after word '%s'\n",
		    c,*real);
	  return(c);
	}
      }

/*       if (type==XFR_NUMBER && isalpha(c)!=0) return(c); */
      g_string_sprintfa(gsreal,"%c",c);  
      *real=g_strdup(gsreal->str); 
      READ=TRUE;
      /*      xfr_print("\nread_real: word=%s\n ",word); */
      break;
    
    } /* switch */
  }
  }


/* ----------------------------------------------------------
   Read value of complex variable  in the form (real, imag) 
   ---------------------------------------------------------- */
gint
read_complex(FILE *f, gchar **real, gchar **imag){
  gint c;
  GString *gsreal=g_string_new("");
  GString *gsimag=g_string_new("");

  guint REALREAD=FALSE;/* becomes TRUE when real part has been read */
  guint IMAGREAD=FALSE;

  while((c=fgetc(f))!=EOF){
    switch(c){
    case '(':  /* do nothing */
      break;

    case ',':  /* we have just read the real part */
      REALREAD=TRUE;
      *real=g_strstrip(g_strdup(gsreal->str));
      g_string_free(gsreal,TRUE);
      /*g_print("\n Real part:%s\n",*real);*/ 
      break;

    case ')':
      IMAGREAD=TRUE;
      /*  *imag=g_strdup("0.0000"); TEST */
      *imag=g_strstrip(g_strdup(gsimag->str)); 
      g_string_free(gsimag,TRUE);
      /*g_print("\n Imag part:<%s>\n",*imag);*/ 
      return(c);
      break;

    default:
      if (REALREAD==FALSE){
	g_string_sprintfa(gsreal,"%c",c); 
      }
      else if (IMAGREAD==FALSE){
	g_string_sprintfa(gsimag,"%c",c); 
      }
      else{
	g_print("\n Code should not reach this line \n");	
	g_print("\n so, I am afraid that this is a bug\n");
	g_print("\n Sorry... \n");
      }
      break;
    }
  }

  g_print("\n Unexpected end of file reading complex variable !!\n");
  return(c);

}





   
/* ------------------------------------------------------------------- 
 Read a sequence of characters that ends on one of the following:
   - blank space,&,=,/, EOF
  The returned value is one of the following members of the following enum:
  enum{WHITE,SLASH,EQUAL,JUMP,EOF,OTHER};
----------------------------------------------------------------------- */
gint
read_word(FILE *f,const gchar **word,guint type){
  int c;
  gint FOUND=FALSE;
  gint QUOTE_FOUND=FALSE;

  /*When a word is read, RETVAL takes a value different from -2*/
  gint RETVAL=-2;
  guint rdigit=FALSE;
  
  
  GString *gword=g_string_new("");

  /*We must allow reading numbers with exponential notation: 
    1.2E+3  1.3d-3, etc*/
  while((c=fgetc(f))!=EOF){
    if (isdigit(c)!=0) rdigit=TRUE; /*A digit character has been read*/
 
    if (isalpha(c)!=0 && type!=XFR_STRING && type!=XFR_DELSTR \
	&& type!=XFR_BOOLEAN){ 
      if ((c=='e' || c=='E' || c=='d' || c=='D') && rdigit==TRUE){
	/* We have read exponential notation character :e,E,d,D*/
      }
      else{
	ungetc(c,f);
	*word=g_strdup(gword->str);
	nwarnings+=1;
	if (nwarnings>MAXERRORS) too_many_warnings();
	g_warning("\nUnexpected character `%c' after word '%s'\n",
		c,*word);
	return(c);
      }
    }
    
    /*if (alpha==FALSE && isalpha(c)!=0){*/

    switch(c){
    case ' ':
      if (type==XFR_DELSTR){
	/* TEST */
	/*g_string_sprintfa(gword,"%c",c); Uncomment???*/ 
	/* TEST */

	/*  g_print("in read_word, white character read, word=<%s>",gword->str);*/
	break;
      }
      else if (FOUND==TRUE && type!=XFR_DELSTR) {
	/* g_print("In read_word, white character read, word=<%s>",gword->str);*/
	RETVAL=WHITE;
      }
      break;
      

    case '\'':
      if (QUOTE_FOUND==FALSE) {
	QUOTE_FOUND=TRUE;
      }
      else{
	RETVAL=OTHER;
      }
      break;

    case '/': 
      /*xfr_print("Slash->End of list:<%c>",c);*/
      RETVAL=SLASH;
      break;
      
    case '=':
      /*xfr_print("equal:<%i>",c);*/
      RETVAL=EQUAL;
     break;
     
    case EOF: 
      nwarnings+=1;
      if (nwarnings>MAXERRORS) too_many_warnings();
      g_warning("End of file found trying to read a string\n");
      /*exit(0);*/
      RETVAL=EOF;
      break;
      
    case '\n':
      /*xfr_print("Salto:<%i>",c);*/
     if (FOUND==TRUE){
       /*xfr_print("Break line. FOUND=TRUE word=<%s>",gword->str);*/
       RETVAL=JUMP;
     }
     else{
       /*xfr_print("Break line. FOUND=FALSE word=<%s>\n",gword->str);*/
     }
     break;
     
    case '(':
      /*Dimensioned variable */
      *word=g_strdup(gword->str);
      return(c);
      break;

    case ',': case ')':
      /* Complex variable? */
      *word=g_strdup(gword->str);
      g_print("read_word: I found a character %c after word %s \n",\
	      c,g_strdup(gword->str));
      return(c); 
      break;

    default:            
      if (iscntrl(c)!=0){
	/*xfr_print("Caracter de control:<%c>-><%i> after word <%s>\n",c,c,
	  g_strdup(gword->str));*/
	if (FOUND==TRUE) RETVAL=OTHER;
      }
      else{
	FOUND=TRUE;
	g_string_sprintfa(gword,"%c",c);
      }
      break;
    } /* switch */
    
    if (RETVAL!=-2){
      *word=g_strdup(gword->str);
      g_string_free(gword,TRUE);
      return(c);
    }
  }
  /*Code shouldn't reach here, but it is neccesary to avoid warning messages*/
  g_print("Control fuera de while!");
  return (c);
}



/* ---------------------------------------------------------------------------
  We have to read something like:
  var(imin:imax,dim)= value1 value2 value3 .... or
  var(imin:imax) ->dim by default or
  var(imin) ->imax and dim by default
  var ->imin,imax,dim by defauult

  At this point the var name and the character '(' has already
  been read
  ---------------------------------------------------------------------------- */
void
read_dims(FILE *f, gint *imin,gint *imax, gint *dim){
  gint c;
  GString *gstrmin=g_string_new("");
  GString *gstrmax=g_string_new("");
  GString *gstrdim=g_string_new("");
  guint IMINREAD=FALSE;
  guint IMAXREAD=FALSE;
  guint DIMREAD=FALSE;
  *imin=*imax=*dim=-1; 

  /*Read imin*/
  while((c=fgetc(f))!='='){
    switch(c){
    case ':':
      if (IMINREAD==FALSE){
	/* g_print("imin just read: imin=%s \n",gstrmin->str); */
	*imin=atoi(gstrmin->str);
	IMINREAD=TRUE; /* imin has just been read */
      }
      else{
	*imax=atoi(gstrmax->str);
	IMAXREAD=TRUE;
      }
     
      break;

    case ')':
      /*      g_print("( found and imax=%s  \n",gstrmax->str);*/
      if (IMINREAD==FALSE)  *imin=atoi(gstrmin->str);
      if (gstrmax->len==0){
	/* g_print("imax not specified  gstrmax->len=%i \n",gstrmax->len);*/
	*imax=*imin;
      }
      else{
	*imax=atoi(gstrmax->str);
      }
      if (gstrdim->str>0) *dim=atoi(gstrdim->str);
      IMAXREAD=TRUE;
      break;

    case(EOF):
      g_print("Unexpected end of file reading dimensioned variable !!\n");
      return;

    case ',':
      IMAXREAD=TRUE;
      *imax=atoi(gstrmax->str);
      break;

    default:
      if (IMINREAD==FALSE){
	g_string_sprintfa(gstrmin,"%c",c); 
      }
      else if (IMAXREAD==FALSE){
	g_string_sprintfa(gstrmax,"%c",c); 
      }
      else{
	g_string_sprintfa(gstrdim,"%c",c);
      }
      break;
    }
  }
  /* g_print("En read_dims: imin=%i, imax=%i, dim=%i \n",*imin,*imax,*dim);*/
    return;
} 
  






/* This function writes on file "f" the content of namelist "mynamelist" in
 * namelist format
 * mode=ALL->Write all variables, including those with value "0"
 *     =NONZERO->Write non-zero variables 
 */
gint
writenl(Namelist *mylist,FILE *f,guint mode){
  gint i,j;
  gchar *nl_name=mylist->name;
  Vardesc **varlist=mylist->vars; /*Variable list*/
  gint columns=0;

  fprintf(f," &%s ",nl_name);
  columns+=strlen(nl_name)+2;
  /*xfr_print("En writenl ...\n");*/
  
  for(i=0;i<mylist->nvars;i++){
    Vardesc *var_dv=*(varlist+i);
    gchar *namevar=var_dv->name; /*variable name*/
    gchar *defval=var_dv->deflt; /*default value*/
    GArray *garray=var_dv->garray;
    gint nvals=garray->len;
    guint ARRAY=FALSE;

    if (nvals>1){
      gint imin=var_dv->start;
      gint imax=var_dv->end;
      gint dim=var_dv->dim;
      
      columns+=strlen(namevar)+8;
      columns=check_max_column(columns,f);
      if (dim>0)
	fprintf(f,"%s(%i:%i%s%i)=",namevar,imin,imax,ARRCHAR,dim);
      else
	fprintf(f,"%s(%i:%i)=",namevar,imin,imax);
      ARRAY=TRUE;
     
    }
    
    for (j=0;j<nvals;j++){
      GString *var=&g_array_index(garray,GString,j);/*GString with the value*/
      gchar *value=var->str;
      guint type=var_dv->type;
 
      if (ARRAY==TRUE) {
	/*xfr_print("Value for array %s=%s ",namevar,value);*/
	columns+=strlen(value)+1;
	columns=check_max_column(columns,f);
	fprintf(f," %s ",value);
	
      }
      else{
	if (mode==NONZERO && var->len!=0 && g_strcasecmp(value,"0")!=0){
	  /*xfr_print("var=%s type=%i\n",namevar,type);*/
	  columns+=strlen(namevar)+strlen(value)+2;
	  columns=check_max_column(columns,f);
	  /* added in version 2.0 */
	  if (g_strcasecmp(value,defval)!=0){
	    if (type!=XFR_STRING) fprintf(f,"%s=%s ",namevar,value);
	    else fprintf(f,"%s='%s' ",namevar,value);
	  }
	}
      } 
      /* xfr_print("En writenl:%s=%s\n",namevar,value);*/
    }
  } 
  fputs(" /\n",f);
return(0);
}

guint
check_max_column(gint col, FILE *ff){
  if (col>50) {
    /* xfr_print("Supero 50 columnas"); */
    fputs("\n\t ",ff);
    col=7;
  }
  return(col);
}
	       

/*freenl: frees the namelist and its variables*/
void
freenl(Namelist *mylist){
  Vardesc **varlist=mylist->vars; /*Variable list*/
  gint i;

  for(i=0;i<mylist->nvars;i++){
    Vardesc *var_dv=*(varlist+i);
    GArray *garray=var_dv->garray;
    /* gint nvals=garray->len;*/
    g_array_free(garray,FALSE);
    
    if (var_dv->type==XFR_COMPLEX){
      GArray *carray=var_dv->carray;
      g_array_free(carray,FALSE);
    }
       
  }
}



gint
too_many_warnings(){
  GtkWidget *dialog;
  const gchar *buttons[] = {"OK","Cancel"};
  /* GtkSignalFunc handlers[] = {on_confirm_exit, NULL }; */
  GCallback handlers[]={G_CALLBACK(on_confirm_exit), NULL };
  
  dialog = glade_util_create_dialog_with_buttons ("Too many errors. Abort?",
						  2, 
						  buttons, 2, 
						  handlers,
						  NULL);

  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  gtk_window_set_modal (GTK_WINDOW (dialog), FALSE);
  gtk_widget_show (dialog);
  return(0);
}

