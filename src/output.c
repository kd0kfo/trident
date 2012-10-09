/*! \file output.c
 *     \brief Handles output print and command line parsing
 *         
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "trident.h"

/** 
 * Function to print ASCII banner and version
 *
 */
void print_banner(FILE* fpout) {
  fprintf(fpout, "\n");
  fprintf(fpout, "   \\ ,-*-.   ,-*-. ,-*-.   ,-*-. ,-*-.   ,    \n");
  fprintf(fpout, "    X | | \\ / | | X | | \\ / | | X | | \\ /   \n");
  fprintf(fpout, "   / \\| | |X| | |/ \\| | |X| | |/ \\| | |X|   \n");
  fprintf(fpout, "      `-!-' `-!-'   `-!-' `-!-'   `-!-' `-     \n");
  fprintf(fpout, "\n");
  fprintf(fpout, "   %s %s microRNA dsDNA triplex algorithm\n", PACKAGE_NAME, VERSION);
  fprintf(fpout, "\n");
  fprintf(fpout, "                ,-*-.         ,-*-.            \n");
  fprintf(fpout, "               / | | \\       / | | \\         \n");
  fprintf(fpout, "         | | |/       \\| | |/       \\| |     \n");
  fprintf(fpout, "         `-!-'         `-!-'         `-!       \n");
  fprintf(fpout, "\n\n");
}

/** 
 * Function to print license
 *
 */
void print_license(FILE* fpout) {
	fprintf(fpout, "\n\n");
}

/** 
 * Function to print small license
 *
 */
void print_small_license(FILE* fpout) {
}

/** 
 * Function to print program usage
 *
 */
void print_usage() {
	printf("Usage:\t%s fi1e1 file2 [options..]\n", PACKAGE);
	printf("\nWhere:\t\'file1\' is a FASTA file\n\t with the microRNA sequence(s)\n\n");
	printf("\t\'file2\' is a FASTA file containing\n\t one strand of the dsDNA sequence(s)\n\t to be scanned.\n\n");
}

/** 
 * Function to print program command line options
 *
 */
void print_options() {
	char* inttobool[2][3];
	char* inttoboolr[2][3];
	strcpy((char*)inttobool[0], "off");
	strcpy((char*)inttobool[1], "on");
	strcpy((char*)inttoboolr[1], "off");
	strcpy((char*)inttoboolr[0], "on");
	print_banner(stdout);
	print_small_license(stdout);
	print_usage();
	printf("OPTIONS\n\n");
	printf(" --help -h\tDisplay this message\n");
	printf(" --version -v\tDisplay version information\n");
	printf(" --revision\tGet revision information\n");
	printf(" --license\tDisplay license information\n");
	printf("\nCore algorithm parameters:\n");
	printf(" -sc S\t\tSet score threshold to S\t\t[DEFAULT: %3.1f]\n", score_threshold);
	printf(" -en -E\t\tSet energy threshold to -E kcal/mol\t[DEFAULT: %3.1f]\n", energy_threshold);
	printf(" -scale Z\tSet scaling parameter to Z\t\t[DEFAULT: %3.1f]\n", scale);
	printf(" -strict\tDemand strict 5' seed pairing\t\t[DEFAULT: %s]\n", (char*)inttobool[strict]);
	printf("\nAlignment parameters:\n");
	printf(" -go -X\t\tSet gap-open penalty to -X\t\t[DEFAULT: %3.1f]\n", gap_extend);
	printf(" -ge -Y\t\tSet gap-extend penalty to -Y\t\t[DEFAULT: %3.1f]\n", gap_open);
	printf("\nGeneral Options:\n");
	printf(" -out file\tOutput results to file\t\t\t[DEFAULT: %s]\n", (char*)inttobool[outfile]);
	printf(" -scaninfo file\tOutput results to file\t\t\t[DEFAULT: off]\n");
	printf(" -quiet\t\tOutput fewer event notifications\t[DEFAULT: %s]\n", (char*)inttoboolr[verbosity]);
	printf(" -brief\t\tOutput only data\t\t\t[DEFAULT: %s]\n",(char*)inttoboolr[brief_output]);
	printf(" -rusage\t\tReport Usage Data\t\t\t[DEFAULT: %s]\n",(char*)inttoboolr[rusage_output]);
	printf(" -trim T\tTrim reference sequences to T nt\t[DEFAULT: %s]\n", (char*)inttobool[truncated]);
	printf(" -noenergy\tDo not perform thermodynamics\t\t[DEFAULT: %s]\n", (char*)inttobool[no_energy]);
	printf("\n -restrict file\tRestricts scans to those between\n");
	printf("             \tspecific miRNAs and UTRs\n");
	printf("             \tprovided in a pairwise\n");
	printf("             \ttab-separated file\t\t\t[DEFAULT: %s]\n", (char*)inttobool[restricted]);
	/*printing undocumented options*/
	printf("\nOther Options:\n");
	printf(" -keyval\tKey value pairs ??\t\t\t[DEFAULT:]\n");
	printf("\n\n");
}

/** 
 * Function to parse command line options
 *
 */
int parse_command_line(int argc, char* argv[], char* filename1, char* filename2,
		char* fileout, char* pairs_file) {
	int i = 0;
	char* endptr;
	for (i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")
				|| !strcmp(argv[i], "--license") || !strcmp(argv[i], "-license")) {
			print_banner(stdout);
			print_license(stdout);
			exit(0);
		}
		if (!strcmp(argv[i],"--revision")){
		    extern const char revision_string[];
		    printf("%s",revision_string);
		    exit(0);		    
		  }
		if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")
				|| !strcmp(argv[i], "--h") || !strcmp(argv[i], "-help")
				|| !strcmp(argv[i], "-usage")) {
			print_options();
			exit(0);
		}
	}
	if (argc > 2) {
		/* This should contain a microRNA FASTA Sequence (query)*/
		strcpy(filename1, argv[1]);
		/* This should contain UTR FASTA Sequence(s) (reference)*/
		strcpy(filename2, argv[2]);
		for (i = 3; i < argc; i++) {
			if (!strcmp(argv[i], "-go") && argc > i + 1) {
				gap_open = atoi(argv[i + 1]);
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-ge") && argc > i + 1) {
				gap_extend = atoi(argv[i + 1]);
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-scale") && argc > i + 1) {
				scale = strtod(argv[i + 1], &endptr);
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-noenergy") || !strcmp(argv[i], "--noenergy")) {
				no_energy = 1;
				continue;
			}
			if (!strcmp(argv[i], "-debug")) {
				debug = 1;
				continue;
			}
			if (!strcmp(argv[i], "-strict")) {
				strict = 1;
				continue;
			}
			if (!strcmp(argv[i], "-keyval")) {
				key_value_pairs = 1;
				continue;
			}
			if (!strcmp(argv[i], "-out") && argc > i + 1) {
				strcpy(fileout, argv[i + 1]);
				outfile = 1;
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-scaninfo") && argc > i + 1) {
			  extern FILE *scaninfo_file;
			  scaninfo_file = fopen(argv[i+1],"w");
			  full_assert(scaninfo_file != NULL,"Could not open scaninfo file.\n");
			  i++;
			  continue;
			}
			if (!strcmp(argv[i], "-en") && argc > i + 1) {
				energy_threshold = atoi(argv[i + 1]);
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-sc") && argc > i + 1) {
				score_threshold = atoi(argv[i + 1]);
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-trim") && argc > i + 1) {
				truncated = atoi(argv[i + 1]);
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-restrict") && argc > i + 1) {
				restricted = 1;
				strcpy(pairs_file, argv[i + 1]);
				i++;
				continue;
			}
			if (!strcmp(argv[i], "-quiet")) {
				verbosity = 0;
				continue;
			}
			if (strncmp(argv[i], "-brief", 6) == 0)
			  {
			    verbosity = 0;
			    brief_output = 1;
			    continue;
			  }
			if (strncmp(argv[i], "-rusage", 7) == 0)
			  {
			    rusage_output = 1;
			    continue;
			  }
			if (!strcmp(argv[i], "-miranda")) {
			  global_match_types[MATCH_MIRANDA] = true;
			  global_match_types[MATCH_DIRECT_REVERSE_HOOGSTEEN] = false;
			  global_match_types[MATCH_INDIRECT_REVERSE_HOOGSTEEN] = false;
			  global_match_types[MATCH_DIRECT_HOOGSTEEN] = false;
			  global_match_types[MATCH_INDIRECT_HOOGSTEEN] = false;
			  continue;
			}

			/* not a recognized argument */
			fprintf(stderr,"Error: unrecognized command line argument: %s\nsee %s --help\n",argv[i],argv[0]);
			exit(1);
		}
		if (!outfile && !brief_output)
		    {
		      print_banner(stdout);
		      print_small_license(stdout);
		    }
	} else {
		/* No input*/
		print_banner(stdout);
		print_small_license(stdout);
		print_usage(stdout);
		exit(0);
	}
	return 1;
}

/** 
 * Function to print parameters
 *
 */
void print_parameters(char* filename1, char* filename2, FILE* fpout) {
	if (outfile) {
		print_banner(fpout);
		print_small_license(fpout);
	}
	/* Display current parameter settings*/
	fprintf(fpout, "Query Filename: \t%s\n", filename1);
	fprintf(fpout, "Reference Filename:\t%s\n", filename2);
}

bool coordinates_reversed(int is_parallel)
{
  return (is_parallel && current_match_type == MATCH_DIRECT_REVERSE_HOOGSTEEN)
	|| (!is_parallel && current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN)    || (is_parallel && current_match_type == MATCH_DIRECT_HOOGSTEEN)
    || (!is_parallel && current_match_type == MATCH_INDIRECT_HOOGSTEEN);
}

/** 
 * Function to print hit structure
 *
 */
void print_hit_structure(FILE *fpout, hit_struct *hit, int query_length, int reference_length, int alignment_length, double identity, double similarity, int is_parallel)
{
  int ref_start, ref_end;
  if(fpout == NULL)
    return;
  if(hit == NULL)
    {
      fprintf(stderr,"ERROR: hit structure is NULL\n");
      return;
    }
  char left_label, right_label;
  
  if(is_parallel)
    {
      // Labels for the Reference strand
      left_label = '3';
      right_label = '5';
    }
  else
    {
      left_label = '5';
      right_label = '3';
    }
  
  ref_start = hit->ref_start + 1;
  ref_end = hit->ref_end + 1;
  if(coordinates_reversed(is_parallel))
    {
      int temp = ref_end;
      ref_end = reference_length - ref_start + 1;
      ref_start = reference_length - temp + 1;
    }
  
  fprintf(fpout, "\n   Forward:\tScore: %f  Q:%d to %d  R:%s%d to %d%s Align Len (%d) (%3.2f%%) (%3.2f%%)\n\n", 
		  hit->score, (query_length - hit->query_end + 1),
		  (query_length - hit->query_start),
		  ((current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN) ? " c(" : ""),ref_start, ref_end, ((current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN) ? ")" : "") , alignment_length, identity, similarity);
  fprintf(fpout, "   Query:    3' %s%s%s 5'\n                %s%s%s\n",
		  hit->rest[0], hit->alignment[0], hit->rest[3], hit->rest[2],
		  hit->alignment[1], hit->rest[5]);

  if(current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN)
    {
      compliment(hit->rest[1]);
      compliment(hit->rest[4]);
      compliment(hit->alignment[2]);
    }
  
  fprintf(fpout, "   Ref:      %c' %s%s%s %c'\n",left_label,hit->rest[1], hit->alignment[2], hit->rest[4],right_label);
  if(current_match_type == MATCH_DIRECT_REVERSE_HOOGSTEEN || current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN)
    {
      compliment(hit->rest[1]);
      compliment(hit->rest[4]);
      compliment(hit->alignment[2]);
		    
      fprintf(fpout, "             %c' %s%s%s %c'\n\n",right_label,hit->rest[1], hit->alignment[2], hit->rest[4],left_label);

      compliment(hit->rest[1]);
      compliment(hit->rest[4]);
      compliment(hit->alignment[2]);
    }

}


/** 
 * Function to print hit
 *
 */

void printhit(char* query_id, int query_length, int reference_length, char* reference_id, hit_struct* hit, double energy, int keyval_mode, int is_parallel, FILE* fpout) {
	double similarity = 0;
	double identity = 0;
	int alignment_length = 0;
	int i = 0;
	int ref_start, ref_end;
	char strand_type[] = "pyrimidine";

	if(current_match_type == MATCH_DIRECT_REVERSE_HOOGSTEEN || current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN)
	  {
	    strcpy(strand_type,"purine");
	  }
	else if(current_match_type == MATCH_MIRANDA)
	  {
	    strcpy(strand_type,"miranda");
	  }
	
	alignment_length = strlen(hit->alignment[0]);
	for (i = 0; i < alignment_length; i++) {
		if (hit->alignment[1][i] == '|' || hit->alignment[1][i] == ';' || hit->alignment[1][i] == '$') {
			similarity++;
			identity++;
		}
		if (hit->alignment[1][i] == ':') {
			similarity++;
		}
	}
	similarity = (similarity / (double)alignment_length) * 100;
	identity = (identity / (double)alignment_length) * 100;
	revstring(hit->alignment[0]);
	revstring(hit->alignment[1]);
	revstring(hit->alignment[2]);
	if (no_energy){
		energy=0.0;
	}

	ref_start = hit->ref_start + 1;
	ref_end = hit->ref_end+1;
	if(coordinates_reversed(is_parallel))
	  {
	    int temp = ref_end;
	    ref_end = reference_length - ref_start+1;
	    ref_start = reference_length - temp+1;
	    }
	
	if (!keyval_mode) {
	  if(!brief_output)
	    print_hit_structure(fpout,hit,query_length, reference_length,alignment_length,identity, similarity, is_parallel);
	  if (!no_energy) {
	    if(!brief_output)
	      fprintf(fpout, "   Energy:  %f kCal/Mol\n", energy);
	  }
	  if(!brief_output)
	    fprintf(fpout, "\nScores for this hit:\n");
	  fprintf(fpout, ">%s,%s,%2.2f,%2.2f,%d %d,%d %d,%d,%3.2f%%,%3.2f%%", query_id, reference_id, hit->score, energy, (query_length - hit->query_end + 1), (query_length - hit->query_start),ref_start, ref_end, alignment_length, identity, similarity);
	  
	  // append structure to hit line
	  fprintf(fpout,",%s%s%s", hit->rest[0], hit->alignment[0], hit->rest[3]);
	  fprintf(fpout,",%s%s%s", hit->rest[2], hit->alignment[1], hit->rest[5]);

	  if(current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN
	     || current_match_type == MATCH_INDIRECT_HOOGSTEEN)
	    {
	      compliment(hit->rest[1]);
	      compliment(hit->rest[4]);
	      compliment(hit->alignment[2]);
	    }
		
	  fprintf(fpout,",%s%s%s", hit->rest[1], hit->alignment[2], hit->rest[4]);

	  if(current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN
	     || current_match_type == MATCH_INDIRECT_HOOGSTEEN)
	    {
	      compliment(hit->rest[1]);
	      compliment(hit->rest[4]);
	      compliment(hit->alignment[2]);
	    }

	  if(is_parallel)
	    fprintf(fpout,",parallel");
	  else
	    fprintf(fpout,",antiparallel");
	  
	  // tag the match type
	  fprintf(fpout,",");
	  switch(current_match_type)
	    {
	    case MATCH_DIRECT_REVERSE_HOOGSTEEN: case MATCH_DIRECT_HOOGSTEEN:
	      fprintf(fpout,"direct");
	      break;
	    case MATCH_INDIRECT_REVERSE_HOOGSTEEN: case MATCH_INDIRECT_HOOGSTEEN:
	      fprintf(fpout,"indirect");
	      break;
	    case MATCH_MIRANDA:
	      fprintf(fpout,"miranda");
	      break;
	    default:
	      fprintf(fpout,"unknown");
	      break;
	    }

	  fprintf(fpout,",%s",strand_type);
	  
	  fprintf(fpout,"\n");

	} else {
		fprintf(fpout,
				"//hit_info\tquery_id=%s\treference_id=%s\tscore=%f\t"
				"energy=%f\tquery_start=%d\tquery_end=%d\tref_start=%d\tref_end=%d\t"
				"aln_length=%d\tidentity=%f\tsimilarity=%f\taln_mirna=%s%s%s\taln_map=%s%s%s\taln_utr=%s%s%s\n",
				query_id, reference_id, hit->score, energy,
				(query_length - hit->query_end + 1), (query_length - hit->query_start ),
				hit->ref_start + 1, hit->ref_end + 1,
				alignment_length, identity, similarity,
				hit->rest[0], hit->alignment[0], hit->rest[3],
				hit->rest[2], hit->alignment[1], hit->rest[5],
				hit->rest[1], hit->alignment[2], hit->rest[4]);
	}
}
