/*! \file pairs.c
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "trident.h"

/** 
 * Loads in a flat-file containing target pairs to scan
 */
int readinpairs(FILE* fp , pair_struct** pairs) {
	char BUFFER[400];
	int n_pairs = 0;
	int N_pairs = 16;
	if (!(*pairs = malloc(N_pairs * sizeof(pair_struct)))) {
		fprintf(stderr, "Cannot obtain %ld pair structs\n", (long)N_pairs);
		exit(1);
	}
	while (fgets(BUFFER, 400, fp)) {
		if (n_pairs >= N_pairs) {
			N_pairs *= 1.44;
			if (!(*pairs = realloc(*pairs, N_pairs * sizeof(pair_struct)))) {
				fprintf(stderr, "Cannot realloc to %ld pair structs\n", (long)N_pairs);
				exit(1);
			}
		}
		sscanf(BUFFER, "%s\t%s", (*pairs)[n_pairs].identifier1, (*pairs)[n_pairs].identifier2);
		n_pairs++;
	}
	return n_pairs;
}


int sort_pairs(const void* s1, const void* s2) {
	pair_struct* a = (pair_struct*)s1;
	pair_struct* b = (pair_struct*)s2;
	if (strcmp(a->identifier1, b->identifier1) == 0) {
		return strcmp(a->identifier2, b->identifier2);
	} else {
		return strcmp(a->identifier1, b->identifier1);
	}
}

int load_pairs(FILE* fp_pairs, pair_struct** pairs) {
	int total_pairs = readinpairs(fp_pairs, pairs);
	qsort(*pairs, total_pairs, sizeof(pair_struct), sort_pairs);
	return total_pairs;
}

int find_pair(char* ident1, char* ident2, int total_pairs, pair_struct* pairs) {
	pair_struct search_key;
	strncpy(search_key.identifier1, ident1, MAX_PAIR_SEQ_ID_LEN-1);
	strncpy(search_key.identifier2, ident2, MAX_PAIR_SEQ_ID_LEN-1);
	search_key.identifier1[MAX_PAIR_SEQ_ID_LEN-1] = 0;
	search_key.identifier2[MAX_PAIR_SEQ_ID_LEN-1] = 0;
	return bsearch(&search_key, pairs, total_pairs, sizeof(pair_struct), sort_pairs) != 0;
}
