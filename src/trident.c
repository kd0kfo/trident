/*! \file trident.c
 *     \brief Main trident source
 *         
 *     This program is designed to find Hoogsteen like interactions between double stranded DNA and single stranded RNA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>

#ifdef USE_RUSAGE
#include <sys/resource.h>
#endif

#include "trident.h"

/**
 * Defines the character representations of the nucleotide base pairing types 
 *
 * Traditional Watson-Crick base pairings are represented by |
 * 
 * Hoogsteen type pairings are represented by by either $ if a direct hit or ; if an indirect hit
 *
 * 
 * Direct is defined as a hit found on the strand provided
 * 
 * Indirect is defined as a hit found on the complement of the strand provided
 *
 */
void initialize_representations()
{
  alignment_match_representations[MATCH_MIRANDA] = '|';
  alignment_match_representations[MATCH_DIRECT_REVERSE_HOOGSTEEN] = '$';
  alignment_match_representations[MATCH_DIRECT_HOOGSTEEN] = '$';
  alignment_match_representations[MATCH_INDIRECT_REVERSE_HOOGSTEEN] = ';';
  alignment_match_representations[MATCH_INDIRECT_HOOGSTEEN] = ';';
}

/**
 * Defines globals to be initialized
 *
 */
void initialize_globals() {
  initialize_representations();
  initialize_bases(); /* Prepare the generic base lookup array*/
  initialize_seqio_buffers();
}

/**
 * Defines globals to be destroyed
 *
 */

void destroy_globals() {
	destroy_seqio_buffers();
}

/**
 * Defines the match types that are used by default
 *
 * 
 *
 */
void setup_match_types()
{
  memset(global_match_types,false,NUM_MATCH_TYPES*sizeof(int));
  global_match_types[MATCH_DIRECT_REVERSE_HOOGSTEEN] = true;
  global_match_types[MATCH_INDIRECT_REVERSE_HOOGSTEEN] = true;
  global_match_types[MATCH_DIRECT_HOOGSTEEN] = true;
  global_match_types[MATCH_INDIRECT_HOOGSTEEN] = true;
    
}

int main (int argc, char* argv[]) {
	char filename1[200];
	char filename2[200];
	char pairs_file[200];
	char fileout[200];
	FILE* query_fp = 0;
	FILE* reference_fp = 0;
	FILE* fp_pairs = 0;
	FILE* fpout = stdout;
	int total_pairs = 0;
	pair_struct* pairs = 0;
	extern FILE *scaninfo_file;

	setup_match_types();

	/* Set Default Parameter Values*/

	length_5p_for_weighting = 8; /* The 5' sequence length to be weighted  except for the last residue */
	scale = 4.0;			/* The 5' miRNA scaling parameter */
	strict = 0;			/* Strict seed model on/off*/
	debug = 0;			/* Debugging mode on/off*/
	key_value_pairs = 0;
	gap_open = -9.0;		/* Gap-open Penalty*/
	gap_extend = -4.0;		/* Gap-extend Penalty*/
	score_threshold = 140.0;	/* SW Score Threshold for reporting hits*/
	energy_threshold = 1.0;		/* Energy Threshold (DG) for reporting hits*/
	verbosity = 1;	                /* Verbose mode on/off*/
	brief_output = 0; 	                /* Brief output off by default */
	rusage_output = 0; 	                /* rusage output off by default */
	outfile = 0;			/* Dump to file on/off*/
	truncated = 0;			/* Truncate sequences on/off*/
	no_energy = 0;			/* Turn off Vienna Energy Calcs - FASTER*/
	restricted = 0;			/* Perform restricted search space*/
	parse_command_line(argc, argv, filename1, filename2, fileout, pairs_file);
	if (gap_open > 0.0 || gap_extend > 0.0) {
		fprintf(stderr, "Error: gap penalties may not be greater than 0\n");
		return 1;
	}
	if (truncated < 0) {
		fprintf(stderr, "Error: negative value give for UTR truncation\n");
		return 1;
	}
	if ((query_fp = fopen(filename1, "r")) == NULL) {
		fprintf(stderr, "Error: Cannot open file %s\n", filename1);
		return 1;
	}
	if ((reference_fp = fopen(filename2, "r")) == NULL) {
		fprintf(stderr, "Error: Cannot open file %s\n", filename2);
		return 1;
	}
	fclose(reference_fp);
	if ((outfile) && ((fpout = fopen(fileout, "w")) == NULL)) {
		fprintf(stderr, "Error: Cannot create output file %s\n", fileout);
		return 1;
	}
	if (restricted) {
		if ((fp_pairs = fopen(pairs_file, "r")) == NULL) {
			fprintf(stderr, "Error: Cannot open restrict pairs file %s\n", pairs_file);
			return 1;
		}
		/* Initialize the pairs list for restriced searches*/
		total_pairs = load_pairs(fp_pairs, &pairs);
		fclose(fp_pairs);
	}
	fflush(fpout);
	initialize_globals();
	if(!brief_output)
	  print_parameters(filename1, filename2, fpout);
	if (restricted && verbosity) {
		printf("Performing Restricted Scan on:%d pairs\n", total_pairs);
	}
	find_targets(query_fp, fpout, pairs, total_pairs, filename2);

#ifdef USE_RUSAGE
	if(rusage_output)
	  {
	    struct rusage ru;
	    if(getrusage(RUSAGE_SELF,&ru) != 0)
	      {
		fprintf(stderr,"Could not get rusage data\n");
		if(errno)
		  fprintf(stderr,"Reason: %s",strerror(errno));
	      }
	    else
	      {
		printf("User CPU time: %ld.%06ld\n",ru.ru_utime.tv_sec,ru.ru_utime.tv_usec);
		printf("Max RSS: %ld KB\n",ru.ru_maxrss);
		
	      }
	  }
#endif // RUSAGE

	destroy_globals();
	if (outfile) fclose(fpout);
	if (scaninfo_file != stdout && scaninfo_file != stderr && scaninfo_file != NULL)
	  fclose(scaninfo_file);
	fclose(query_fp);
	return 0;
}
