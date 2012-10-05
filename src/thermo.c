
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fold_vars.h"
#include "utils.h"
#include "fold.h"
#include "trident.h"

double vfold(char* sequence) {
	void* struct1;
	double e1;
	struct1 = (char*)space(sizeof(char) * (strlen(sequence) + 1));
	temperature = 30;
	initialize_fold(strlen(sequence));
	e1 = fold(sequence, struct1);
	free_arrays();
	free(struct1);
	return e1;
}

/* Pairwise energy matrix
 * Energy between base pairs in a sequence
 * Columns and rows correspond to C G A U X
 * Thus a G<-->G pair has -10 kcal of energy.
 */
double pair_energy_mat[NUM_SCORED_BASES][NUM_SCORED_BASES] = { 
  /*         C     G      A     T   X   */
  /* C */   {0, -28.18,     0,  0,  0},
  /* G */   {0, -26.93,     0,  0,  0},
  /* A */   {0,      0, -9.77,  0,  0},
  /* U */   {0,      0, -9.94,  0,  0},
  /* X */   {0,      0,     0,  0,  0}};

double sequence_energy(const hit_struct* hit)
{
  double energy = 0;
  size_t qlength, rlength, i;
  size_t base_i, base_j;

  full_assert(hit != NULL,"ERROR: sequence_energy received a NULL pointer for the hit structure\n");
  full_assert(hit->alignment[0] != NULL && hit->alignment[2] != NULL,"ERROR: alignment sequence is a NULL pointer.\n");
  
  qlength = strlen(hit->alignment[0]);
  rlength = strlen(hit->alignment[2]);
  full_assert(qlength == rlength,"ERROR: the query and reference segments are not the correct lengths.\nSizes\nQuery segment: %lu\nRef Segment: %lu\n",qlength,rlength);
  
  i = 0;
  for(;i<qlength;i++)
    {
      base_i = hit->alignment[0][i];
      base_j = hit->alignment[2][i];
      if(bases[base_i] > 3 || bases[base_j] > 3)// (c,g,a,ut) = (0,1,2,3)
	continue;

      // Next two if-blocks ensure that only the correct energies are include for the specific match types, i.e. purine versus pyrimidine
      if(current_match_type == MATCH_DIRECT_REVERSE_HOOGSTEEN || current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN)
	{
	  if(bases[base_i] == 0 || bases[base_i] == 3)
	    continue;
	}
      if(current_match_type == MATCH_DIRECT_HOOGSTEEN || current_match_type == MATCH_INDIRECT_HOOGSTEEN)
	{
	  if(bases[base_i] == 1 || bases[base_i] == 2)
	    continue;
	}

      energy += pair_energy_mat[bases[base_i]][bases[base_j]];
    }
  
  return energy;

}

double get_miranda_energy(hit_struct *hit){
	double energy = 0;
	int i = 0;
	int j = 0;
	char foldsequence[5000];
	revstring(hit->alignment[0]);
	revstring(hit->rest[1]);
	revstring(hit->rest[4]);
	foldsequence[0] = '\0';
	for (i = 0; i < strlen(hit->rest[0]); i++) {
		foldsequence[j] = hit->rest[0][i];
		j++;
	}
	for (i = 0; i < strlen(hit->alignment[0]); i++) {
		if (hit->alignment[0][i] != '-') {
			foldsequence[j] = hit->alignment[0][i];
			j++;
		}
	}
	for (i = 0; i < strlen(hit->rest[3]); i++) {
		foldsequence[j] = hit->rest[3][i];
		j++;
	}
	for (i = 0; i < 7; i++) {
		foldsequence[j] = 'X';
		j++;
	}
	for (i = 0; i < strlen(hit->rest[4]); i++) {
		foldsequence[j] = hit->rest[4][i];
		j++;
	}
	for (i = 0; i < strlen(hit->alignment[2]); i++) {
		if (hit->alignment[2][i] != '-') {
			foldsequence[j] = hit->alignment[2][i];
			j++;
		}
	}
	for (i = 0; i < strlen(hit->rest[1]); i++) {
		foldsequence[j] = hit->rest[1][i];
		j++;
	}
	foldsequence[j] = '\0';
	/* printf("FOLD: %s\n", foldsequence);*/

	energy = vfold(foldsequence);

	revstring(hit->alignment[0]);
	revstring(hit->rest[1]);
	revstring(hit->rest[4]);
	return energy;
}

double get_energy(hit_struct* hit) {
  if(current_match_type == MATCH_MIRANDA)
    return get_miranda_energy(hit);
  
  return sequence_energy(hit);
}
