#ifndef PACKAGE_DATA_DIR
#define PACKAGE_DATA_DIR ""
#endif

#define COLUMNS_PARTITIONS 10
#define COLUMNS_STATES 17
#define COLUMNS_POT 12
#define COLUMNS_STEP 6
#define COLUMNS_COUP 10
#define COLUMNS_INEL 6
#define COLUMNS_CFP 5
#define COLUMNS_OVERLAP 25

#define QSCALEDIM 8

#define MAXERRORS 20
#define MAXCOLS  40

#define ARRCHAR ":"



/*Return values when reading namelists */
enum {LIST_READ_OK,
      END_OF_FILE,
      LIST_NOT_FOUND,
      EMPTY_LIST,
      LAST_LIST_READ};



/* Type of variable to be read or written*/
/* XFR_BOOLEAN: T, F
   XFR_INT: integer
   XFR_DOUBLE: double
   XFR_STRING: string
   XFR_DELSTR: string delimited by ' or "
   XFR_COMPLEX: complex
*/

enum { 
  XFR_BOOLEAN,
  XFR_INT,
  XFR_DOUBLE,
  XFR_NUMBER,
  XFR_STRING,
  XFR_DELSTR,
  XFR_COMPLEX};


/* CLIST partition */
enum {PNUCLEUS,
      PMASS,PZ,
      TNUCLEUS,
      TMASS,
      TZ,
      QVALUE,
      PWF1,
      NEX,
      READSTATES};









