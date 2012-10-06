#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <config.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "trident.h"

int cmpscores(const void* p1, const void* p2) {
	score_struct* s1 = (score_struct*)p1;
	score_struct* s2 = (score_struct*)p2;
	/* primary key: score */
	if (s1->score < s2->score) return 1;
	if (s1->score > s2->score) return -1;
	/* secondary key: earlier end in reference */
	if (s1->reference_trace_end > s2->reference_trace_end) return 1;
	if (s1->reference_trace_end < s2->reference_trace_end) return -1;
	return 0;
}

void clear_hit(hit_struct* hit, int query_length, int reference_length) {
	hit->score = 0;
	hit->query_start = 0;
	hit->query_end = 0;
	hit->ref_start = 0;
	hit->ref_end = 0;
	memset(hit->alignment[0], '\0', query_length + reference_length);
	memset(hit->alignment[1], '\0', query_length + reference_length);
	memset(hit->alignment[2], '\0', query_length + reference_length);
	memset(hit->rest[0], '\0', query_length + 10);
	memset(hit->rest[1], '\0', query_length + 10);
	memset(hit->rest[2], '\0', query_length + 10);
	memset(hit->rest[3], '\0', query_length + 10);
	memset(hit->rest[4], '\0', query_length + 10);
	memset(hit->rest[5], '\0', query_length + 10);
}

void revstring(char s[]) {
	int c, i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void string_toupper(char *s) {
	while (*s) {
		*s = toupper(*s);
		s++;
	}
}

void reverse_comp(char *reference_sequence,size_t reference_length)
{
  int index;

  if(reference_sequence == NULL)
    return;
  
  revstring(reference_sequence);

  for(index = 0;index < reference_length;index++,reference_sequence++)
    {
      switch(*reference_sequence)
	{
	case 'A':
	  *reference_sequence = 'U';
	  break;
	case 'a':
	  *reference_sequence = 'u';
	  break;
	case 'G':
	  *reference_sequence = 'C';
	  break;
	case 'g':
	  *reference_sequence = 'c';
	  break;
	case 'C':
	  *reference_sequence = 'G';
	  break;
	case 'c':
	  *reference_sequence = 'g';
	  break;
	case 'U':case 'T':
	  *reference_sequence = 'A';
	  break;
	case 'u':case 't':
	  *reference_sequence = 'a';
	  break;
	case 0:
	  return;
	  break;
	default:
	  break;
	}
    }
}

void compliment(char *reference_sequence,size_t reference_length)
{
  int index;

  if(reference_sequence == NULL)
    return;
  
  for(index = 0;index < reference_length;index++,reference_sequence++)
    {
      switch(*reference_sequence)
	{
	case 'a':
	  *reference_sequence = 't';
	  break;
	case 'A':
	  *reference_sequence = 'T';
	  break;
	case 'g':
	  *reference_sequence = 'c';
	  break;
	case 'G':
	  *reference_sequence = 'C';
	  break;
	case 'c':
	  *reference_sequence = 'g';
	  break;
	case 'C':
	  *reference_sequence = 'G';
	  break;
	case 'u':case 't':
	  *reference_sequence = 'a';
	  break;
	case 'U':case 'T':
	  *reference_sequence = 'A';
	  break;
	case 0:
	  return;
	  break;
	default:
	  break;
	}
    }
}

void convert_u_to_t(char *reference_sequence,size_t reference_length)
{
  int index;

  if(reference_sequence == NULL)
    return;
  
  for(index = 0;index < reference_length;index++,reference_sequence++)
    {
      switch(*reference_sequence)
	{
	case 'u': 
	  *reference_sequence = 't';
	  break;
	case 'U':
	  *reference_sequence = 'T';
	  break;
	case 0:
	  return;// End of sequence
	default:
	  break;
	}
    }
}


void reason_exit_vargs(va_list args, const char *format)
{
  vfprintf(stderr,format,args);
  va_end(args);
  if(errno != 0)
  {
      fprintf(stderr,"Reason (%d): %s\n",errno,strerror(errno));
      exit(errno);
  }
  exit(1);
}

void reason_exit(const char *format, ...)
{
  va_list args;
  va_start(args,format);
  reason_exit_vargs(args,format);
}

void full_assert(int condition,const char *format,...)
{
    va_list args;
    if(condition)
        return;
    va_start(args,format);
    reason_exit_vargs(args,format);
} 

FILE *scaninfo_file = NULL;
void scaninfo_print(const char *format, ...)
{
  va_list args;
  if(scaninfo_file == NULL)
    return;
  va_start(args,format);
  vfprintf(scaninfo_file,format,args);
  va_end(args);
  
}

const char* str_match_type(int index)
{
  extern char* match_type_name[];

  if(index < 0 || index >= NUM_MATCH_TYPES)
    return match_type_name[NUM_MATCH_TYPES];

  return match_type_name[index];
}

void scaninfo_dump_matrix(int **a_nt_nt,int query_length,int reference_length)
{
  int i,j;
  if(a_nt_nt == NULL)
    return;

  i = 0;
  for(;i<query_length;i++)
    {
      j = 0;
      for(;j<reference_length;j++)
	{
	  scaninfo_print("%d  ",a_nt_nt[i][j]);
	}
      scaninfo_print("\n");
    }
}

#include "H/pair_mat.h"
void fill_BP_pair()
{
  memset(BP_pair,0,NBASES*NBASES*sizeof(int)); 
  switch(current_match_type)
    {
    case MATCH_DIRECT_REVERSE_HOOGSTEEN:    case MATCH_INDIRECT_REVERSE_HOOGSTEEN:

      BP_pair[1][1] = 1;
      BP_pair[3][3] = 1;
      break;
    case MATCH_DIRECT_HOOGSTEEN:    case MATCH_INDIRECT_HOOGSTEEN:
      BP_pair[4][1] = 1;
      BP_pair[2][3] = 1;
      break;
      /*    case MATCH_INDIRECT_REVERSE_HOOGSTEEN:
      BP_pair[1][4] = 1;
      BP_pair[3][2] = 1;
      BP_pair[4][4] = 1;
      break;*/
    case MATCH_MIRANDA:
      BP_pair[1][4] = 5;
      BP_pair[1][7] = 5;
      BP_pair[2][3] = 1;
      BP_pair[3][2] = 2;
      BP_pair[3][4] = 3;
      BP_pair[4][1] = 6;
      BP_pair[4][3] = 4;
      BP_pair[4][7] = 6;
      BP_pair[5][6] = 2;
      BP_pair[6][5] = 1;
      BP_pair[7][1] = 6;
      BP_pair[7][4] = 5;
      break;
    default:
      break;
    }
/* _  A  C  G  U  X  K  I 
{{ 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 5, 0, 0, 5},
 { 0, 0, 0, 1, 0, 0, 0, 0},
 { 0, 0, 2, 0, 3, 0, 0, 0},
 { 0, 6, 0, 4, 0, 0, 0, 6},
 { 0, 0, 0, 0, 0, 0, 2, 0},
 { 0, 0, 0, 0, 0, 1, 0, 0},
 { 0, 6, 0, 0, 5, 0, 0, 0}};
// trident rules
/* _  A  C  G  U  X  K  I 
{{ 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 1, 0, 0, 1, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 1, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0}};
  */
}


