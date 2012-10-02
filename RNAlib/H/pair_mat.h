#include <ctype.h>
#define NBASES 8
/*@notnull@*/
static const char Law_and_Order[] = "_ACGUTXKI";
int BP_pair[NBASES][NBASES];
#if 0
  /* Original RNAlib values */
  /* Replace with dynamic, match type specific function to fill this matrix. */
/* _  A  C  G  U  X  K  I */
{{ 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 5, 0, 0, 5},
 { 0, 0, 0, 1, 0, 0, 0, 0},
 { 0, 0, 2, 0, 3, 0, 0, 0},
 { 0, 6, 0, 4, 0, 0, 0, 6},
 { 0, 0, 0, 0, 0, 0, 2, 0},
 { 0, 0, 0, 0, 0, 1, 0, 0},
 { 0, 6, 0, 0, 5, 0, 0, 0}};
// ple rules
/* _  A  C  G  U  X  K  I */
{{ 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 1, 0, 0, 1, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 1, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0}};
#endif

#define MAXALPHA 20       /* maximal length of alphabet */

static short alias[MAXALPHA+1];
static int pair[MAXALPHA+1][MAXALPHA+1];
/* rtype[pair[i][j]]:=pair[j][i] */
static int rtype[8] = {0, 2, 1, 4, 3, 6, 5, 7}; 

/* for backward compatibility */
#define ENCODE(C) encode_char(c)
extern  int encode_char(char c);

/*@+boolint +charint@*/
/*@null@*/
extern char *nonstandards;
extern void   nrerror(const char message[]);

extern void make_pair_matrix(void);
