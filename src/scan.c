/*! \file scan.c
 *     \brief Handles scanning for input sequences
 *         
 */


#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

#include "trident.h"

/* summary of best hits for reporting - for convenient argument passing */
typedef struct HitSummaryT
{
  double scan_score;
  int no_hits;
  double max_hit;
  double max_score;
  double total_score;
  ExpString *position_list;
} HitSummary;

const int INITIAL_STRING_SIZE = 64;


/**
 * Determines whether a score lies within thresholds.
 *
 * \param score
 */
int good_score(int score)
{
	if(score < score_threshold)
		return 0;
	if(score_ceiling == 0)
		return 1;
	if(score <= score_ceiling)
		return 1;
	return 0;
}

/**
 * Does the alignment between reference and query sequence
 *
 * \param best
 * \param track
 * \param a_nt_nt
 * \param b_gap_nt
 * \param c_nt_gap
 * \param nt_nt_score
 * \param query_length
 * \param reference_length
 * \param verbose
 * \param hit_summary
 * \param query_id
 *
 */
double do_alignment(int** best, int*** track, int** a_nt_nt, int** b_gap_nt, int** c_nt_gap, int** nt_nt_score,
		    char* query_sequence, char* reference_sequence, score_struct* scores,
		    int query_length, int reference_length, int verbose,
		    HitSummary* hit_summary, char* query_id, char* reference_id,
		    hit_struct* hit, int parallel_antiparallel, FILE* fpout) {
  int i = 0;
  int j = 0;
  int utr_offset3p;
  int utr_offset5p;
  double energy = 0;
  double scan_score = 0;
  int good_call = 0;
  int diff = 0;
  char strict_query_construct[200];
  char strict_alignment_construct[200];
  int non_gap_count = 0;
  int perfect_match_count = 0;
  int gap_count = 0;
  int mypos = 0;
  double hit_score;
  int tmp_integer;
  int* good_ones_starts_j, *good_ones_ends_j, good_ones_count;
  int scores_length = 0;
  good_ones_count = -1;
  good_ones_starts_j = (int*)calloc(reference_length, sizeof(int));
  good_ones_ends_j = (int*)calloc(reference_length, sizeof(int));
  hit_summary->no_hits = 0;
  hit_summary->max_hit = 0;
  hit_summary->max_score = 0;
  hit_summary->scan_score = 0;
  hit_summary->total_score = 0;
  clear_ExpString(hit_summary->position_list);
  get_nt_nt_seq_scores(nt_nt_score, query_sequence, reference_sequence, query_length, reference_length);
  build_matrix(best, track, a_nt_nt, b_gap_nt, c_nt_gap, nt_nt_score, query_sequence, reference_sequence, query_length, reference_length, scores, &scores_length);
  scaninfo_print("Have %d good score(s)\n",scores_length+1);
  for (i = 0; i <= scores_length; i++) {
    scaninfo_print("Score: %d\n",scores[i].score);
    utr_offset3p = 0;
    utr_offset5p = 0;
    good_call = 1;
    clear_hit(hit, query_length, reference_length);
    hit_score = scores[i].score;
    if (good_score(hit_score)) {
    	if (debug) {
    		fprintf(fpout, "[%d] score i j path ->> %d %d %d %d\n", i, scores[i].score,
    				scores[i].query_trace_end, scores[i].reference_trace_end, scores[i].path);
    		fflush(fpout);
    	}
      traceback(best, track, query_sequence, reference_sequence, scores[i].query_trace_end, scores[i].reference_trace_end, hit, hit_score);
      good_call = testfor_overlap(good_ones_starts_j, good_ones_ends_j, &good_ones_count,
				  hit->ref_start, hit->ref_end);
      if (good_call == 1) {
      	good_ones_starts_j[good_ones_count] = hit->ref_start;
      	good_ones_ends_j[good_ones_count] = hit->ref_end;
      }
      /*miRNA alignment, convert un-aligned nt to lowercase, aligned nt to uppercase
       * hit->rest[0, 1, 2] = The 5' unaligned regions in the query, alignment and ref
       * hit->rest[3, 4, 5] = The 3' unaligned regions in the query, alignment and ref*/
      if (hit->query_start >= 1) {
	for (j = 0; j <= hit->query_start - 1; j++) {
	  diff = hit->query_start - j;
	  hit->rest[0][j] = tolower(query_sequence[j]);
	  tmp_integer = hit->ref_start - diff;
	  if (tmp_integer >= 0) {
	    hit->rest[1][j] = tolower(reference_sequence[tmp_integer]);
	    utr_offset3p++;
	  } else {
	    hit->rest[1][j] = '-';
	  }
	  hit->rest[2][j] = ' ';
	}
      }
      if (hit->query_end < query_length) {
	for (j = hit->query_end; j < query_length; j++) {
	  diff = j - hit->query_end;
	  hit->rest[3][j - hit->query_end] = tolower(query_sequence[j]);
	  tmp_integer = hit->ref_end + diff;
	  if (tmp_integer < reference_length) {
	    hit->rest[4][j - hit->query_end] = tolower(reference_sequence[tmp_integer]);
	    utr_offset5p++;
	  } else {
	    hit->rest[4][j - hit->query_end] = '-';
	  }
	  hit->rest[5][j - hit->query_end] = ' ';
	}
      }
      string_toupper(hit->alignment[0]);
      string_toupper(hit->alignment[2]);
      /* Adjusting for offset due to local alignment in next two lines*/
      hit->ref_end += (utr_offset5p - 1);
      hit->ref_start -= utr_offset3p;
      mypos = 0;
      non_gap_count = perfect_match_count = gap_count = 0;
      /* looks for strict seed matches*/
      if (strict) {
	sprintf(strict_query_construct, "%s%s%s", hit->rest[3], hit->alignment[0], hit->rest[0]);
	sprintf(strict_alignment_construct, "%s%s%s", hit->rest[5], hit->alignment[1], hit->rest[2]);
	/*
	  printf("   QueTE:    3' %s5'\n                %s\n   RTT:    5' %s 3'\n\n",
	  hit->alignment[0], hit->alignment[1], hit->alignment[2]);
	  printf("strict_query_construct >%s:\n", strict_query_construct);
	  printf("strict_alignment_construct >%s:\n", strict_alignment_construct);
	*/
	/*traverse the alignment*/
	for (j = 0; j < strlen(strict_query_construct); j++) {
	  if (strict_query_construct[j] != '-') {
	    /*if no gaps in the miRNA alignment*/
	    mypos++;
	  }
	  if ((mypos >= 2) && (mypos <= 8)) {
	    if (strict_alignment_construct[j] != ' ') {
	      /*if no gaps in the alignment i.e. if `|` or `:`*/
	      non_gap_count++;
	    }
	    if (strict_alignment_construct[j] == '|') {
	      perfect_match_count++;
	    }
	    if (strict_query_construct[j] == '-') {
	      gap_count++;
	    }
	  }
	  if (mypos == 8) break;
	  /*printf("On Pos: %d [%d]-->%c %c\t[%d:%d]\n", mypos, j, strict_query_construct[j], strict_alignment_construct[j], count1, count2);*/
	}
	/*fail if less than 7 perfect matches or any gaps*/
	if (non_gap_count < 7 || perfect_match_count < 7 || gap_count > 0) {
	  good_call = 0;
	}
      }
      /*printf("Model Analysis: %d %d %d %d\n", count1, count2, count3, count4);*/
      energy = get_energy(hit);

      if (no_energy || energy < energy_threshold) {
				/* good_call, a good alignment that passes score, energy, etc. requirements*/
				if (good_call) {
					scan_score += (energy * -1);
					hit_summary->no_hits++;
					append_char_ExpString(hit_summary->position_list,' ');
					append_int_ExpString(hit_summary->position_list,hit->ref_start + 1);
					if (energy < hit_summary->max_hit) {
						hit_summary->max_hit = energy;
					}
					hit_summary->total_score += hit->score;
					if (hit->score > hit_summary->max_score) {
						hit_summary->max_score = hit->score;
					}
					printhit(query_id, query_length, reference_length, reference_id, hit, energy, key_value_pairs, parallel_antiparallel, fpout);
				}
      }
    }
    scores[i].score = 0;
    scores[i].path = 0;
    scores[i].query_trace_end = 0;
    scores[i].reference_trace_end = 0;
  }
  free(good_ones_ends_j);
  free(good_ones_starts_j);
  return scan_score;
}

/**
 * Loads sequences and sets up the alignment run for finding targets
 */
int find_targets(FILE* query_fp, FILE* fpout, pair_struct* pairs, int total_pairs, char* filename) {
  FILE* reference_fp = 0;
  int** best;			/* Best score of all three states (nt-nt, nt-gap, gap-nt*/
  int*** track;			/* Traceback Matrix		*/
  int** a_nt_nt;			/* best score for state nt-nt*/
  int** b_gap_nt;			/* best score for state gap-nt*/
  int** c_nt_gap;			/* best score for state nt-gap*/
  int** nt_nt_score;		/* nt nt match matrix for easy lookup*/
  int utr_processed = 0;
  int mirna_processed = 0;
  int i = 0,parallel_antiparallel;
  hit_struct hit;			/*Struct to store hit information*/
  score_struct* scores;		/*Score struct for non-optimal path detection*/
  HitSummary hit_summary;		/*Summary of best hits for reporting*/
  double end_score = 0.0;
  char* query_sequence = malloc(INITIAL_STRING_SIZE * sizeof(char));
  int query_length;
  char* query_description = malloc(INITIAL_STRING_SIZE * sizeof(char));
  char* query_id = malloc(INITIAL_STRING_SIZE * sizeof(char));
  int reference_length;
  char* reference_sequence = malloc(INITIAL_STRING_SIZE * sizeof(char));
  char* reference_description = malloc(INITIAL_STRING_SIZE * sizeof(char));
  char* reference_id = malloc(INITIAL_STRING_SIZE * sizeof(char));
  int initial_length_5p_for_weighting = length_5p_for_weighting;
  int initial_length_3p_for_weighting = length_3p_for_weighting;
  hit_summary.position_list = 0;
  create_ExpString(&(hit_summary.position_list));
  /* Read the query sequence(s) (microRNA(s)) from a FASTA file*/
  scaninfo_print("Entered find_targets\n");
  while (readinseq(query_fp, &query_sequence, &query_description, &query_id, &query_length)) {
    
    scaninfo_print("Read miRNA sequence: %s\n",query_sequence);
    
    if (verbosity || debug) {
      fprintf(fpout, "Read Sequence:%s %s(%d nt)\n", query_id, query_description, query_length);
    }
    /* Reverse the query (microRNA) sequence for aligning*/
    revstring(query_sequence);
    /* Loop over all reference sequences in FASTA file*/
    /* Do full scan for each						*/
    if ((reference_fp = fopen(filename, "r")) == NULL) {
      fprintf(stderr, "Error: Cannot open file %s\n", filename);
      exit(1);
    }
    while (readinseq(reference_fp, &reference_sequence, &reference_description, &reference_id, &reference_length)) {
      int match_type_counter = 0;
      scaninfo_print("Read DNA sequence: %s\n",reference_sequence);
      for(;match_type_counter < NUM_MATCH_TYPES;match_type_counter++)
	{
	  if(global_match_types[match_type_counter] == false)
	    continue;
	  current_match_type = match_type_counter;
	  scaninfo_print("Checking match type: %s\n",str_match_type(current_match_type));
	  
	  // parallel_antiparallel = 1 for parallel
	  parallel_antiparallel = 0;// Loop over parallel and antiparallel alignment cases
	  for(;parallel_antiparallel < 2;parallel_antiparallel++)
	    {
	      if(parallel_antiparallel)
		scaninfo_print("Checking parallel alignment\n");
	      else
		scaninfo_print("Checking antiparallel alignment\n");
	      
	      if(parallel_antiparallel == 1)
		{
		  if(current_match_type == MATCH_MIRANDA)
		    break;
		  revstring(reference_sequence);
		}
	      
	      if (restricted) {
	      	if (debug && (strlen(query_id) >= MAX_PAIR_SEQ_ID_LEN || strlen(reference_id) >= MAX_PAIR_SEQ_ID_LEN)) {
	      		fprintf(fpout, "WARNING: Sequence ID is too large for pair structure. Will truncate.\n");
	      	}
	      	if (!find_pair(query_id, reference_id, total_pairs, pairs)) {
				/* printf("Skipped: %s vs %s (length %d)\n", query_id, reference_id, reference_length);*/
				continue;
			}
	      }
	      /* Keep track of the number of sequences scanned so far*/
	      utr_processed++;
	      if (verbosity || debug) {
		fprintf(fpout, "Read Sequence:%s %s(%d nt)\n", reference_id,
			reference_description, reference_length);
	      }
	      if (truncated && reference_length > truncated) {
		reference_sequence[truncated] = '\0';
		reference_length = truncated;
	      }
	  
	      // Re-initialize match type specific information
	      initialize_scores(current_match_type);
	      if(current_match_type == MATCH_MIRANDA)
		{
		  length_5p_for_weighting = initial_length_5p_for_weighting;
		  length_3p_for_weighting = query_length - length_5p_for_weighting;
		}
	      else
		{
		  length_5p_for_weighting = query_length;// Removes weighting for seed site for these methods.
		  length_3p_for_weighting = 0;
		  if(current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN
		     || current_match_type == MATCH_INDIRECT_HOOGSTEEN) 
		    reverse_comp(reference_sequence,reference_length);
		}
	      	      
	      if(debug)
		printf("Scanning DNA: %s\n",reference_sequence);

	      /* Initialize the hit / alignment constructs for this sequence*/
	      hit.alignment[0] = calloc(query_length + reference_length, sizeof(char));
	      hit.alignment[1] = calloc(query_length + reference_length, sizeof(char));
	      hit.alignment[2] = calloc(query_length + reference_length, sizeof(char));
	      hit.rest[0] = calloc(query_length + 10, sizeof(char));
	      hit.rest[1] = calloc(query_length + 10, sizeof(char));
	      hit.rest[2] = calloc(query_length + 10, sizeof(char));
	      hit.rest[3] = calloc(query_length + 10, sizeof(char));
	      hit.rest[4] = calloc(query_length + 10, sizeof(char));
	      hit.rest[5] = calloc(query_length + 10, sizeof(char));
	      /* Structure for sub-optimal score list*/
	      scores = (score_struct*)calloc(query_length * reference_length, sizeof(score_struct));
	      /* Initialize the three alignment matrices*/
	      /*TODO: use multidimensional arrays rather than arrays of pointers to arrays. Also, don't calloc/free every iteration; reuse instead */
	      best = calloc((query_length + 1), sizeof(int*));
	      track = (int***)calloc(4, sizeof(int**));
	      a_nt_nt = calloc((query_length + 1), sizeof(int*));
	      b_gap_nt = calloc((query_length + 1), sizeof(int*));
	      c_nt_gap = calloc((query_length + 1), sizeof(int*));
	      nt_nt_score = calloc((query_length + 1), sizeof(int*));
	      /*Initialize 4-D call-back matrix*/
	      for (i = 0; i < 4; i++) {
		track[i]= (int**)calloc((query_length + 1), sizeof(int*));
	      }
	      for (i = 0; i <= query_length; i++) {
		best[i] = calloc((reference_length + 1), sizeof(int));
		track[0][i] = (int*)calloc((reference_length + 1), sizeof(int));
		track[1][i] = (int*)calloc((reference_length + 1), sizeof(int));
		track[2][i] = (int*)calloc((reference_length + 1), sizeof(int));
		track[3][i] = (int*)calloc((reference_length + 1), sizeof(int));
		a_nt_nt[i] = calloc((reference_length + 1), sizeof(int));
		b_gap_nt[i] = calloc((reference_length + 1), sizeof(int));
		c_nt_gap[i] = calloc((reference_length + 1), sizeof(int));
		nt_nt_score[i] = calloc((reference_length + 1), sizeof(int));
		best[i][0] = a_nt_nt[i][0] = nt_nt_score[i][0] = 0;
		track[0][i][0] = track[1][i][0] = track[2][i][0] = track[3][i][0] = 0;
		b_gap_nt[i][0] = 0;
		c_nt_gap[i][0] = 0;
		/*b_gap_nt[i][0] = -10000; c_nt_gap[i][0] = gap_open + (i * gap_extend);*/
	      }
	      for (i = 0; i < reference_length + 1; i++) {
		best[0][i] = a_nt_nt[0][i] = nt_nt_score[0][i] = 0;
		b_gap_nt[0][i] = 0;
		c_nt_gap[0][i] = 0;
		/*b_gap_nt[0][i] = gap_open + (i * gap_extend); c_nt_gap[0][i] = -10000;*/
		track[0][0][i] = track[1][0][i] = track[2][0][i] = track[3][0][i] = 0;
	      }
	      if(verbosity || debug) {
		fprintf(fpout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
		fprintf(fpout, "Performing Scan: %s vs %s\n", query_id, reference_id);
		fprintf(fpout, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
		fflush(fpout);
	      }
	      end_score = do_alignment(best, track, a_nt_nt, b_gap_nt, c_nt_gap, nt_nt_score,
				       query_sequence, reference_sequence, scores, query_length, reference_length,
				       1, &hit_summary, query_id, reference_id, &hit, parallel_antiparallel, fpout);
	      if (verbosity || debug) {
		fprintf(fpout, "Score for this Scan:\n");
	      }
	      if (end_score > 0.0) {
		if(!brief_output)
		  {
		    fprintf(fpout, "Seq1,Seq2,Tot Score,Tot Energy,Max Score,Max Energy,Strand,Len1,Len2,Positions\n");
		    if (!no_energy) {
		      fprintf(fpout, ">>%s\t%s\t%2.2f\t-%2.2f\t%2.2f\t%2.2f\t%d\t%d\t%d\t%s\n",
			      query_id, reference_id, hit_summary.total_score,
			      end_score, hit_summary.max_score, hit_summary.max_hit,
			      utr_processed, query_length, reference_length,
			      access_ExpString(hit_summary.position_list));
		    } else {
		      fprintf(fpout, ">>%s\t%s\t%2.2f\t0.0\t%2.2f\t0.0\t%d\t%d\t%d\t%s\n",
			      query_id, reference_id, hit_summary.total_score,
			      hit_summary.max_score, utr_processed, query_length, reference_length,
			      access_ExpString(hit_summary.position_list));
		    }
		  }
		fflush(fpout);
	      } else {
		if (verbosity || debug) {
		  fprintf(fpout, "No Hits Found above Threshold\n");
		}
	      }
	      if (verbosity || debug || end_score > 0.0) {
		if(!brief_output)
		  fprintf(fpout, "Complete\n\n");
	      }
	      fflush(fpout);
	      for (i = query_length; i >= 0; i--) {
		free(nt_nt_score[i]);
		free(c_nt_gap[i]);
		free(b_gap_nt[i]);
		free(a_nt_nt[i]);
		free(track[3][i]);
		free(track[2][i]);
		free(track[1][i]);
		free(track[0][i]);
		free(best[i]);
	      }
	      for (i = 3; i >= 0; i--) {
		free(track[i]);
	      }
	      free(nt_nt_score);
	      free(c_nt_gap);
	      free(b_gap_nt);
	      free(a_nt_nt);
	      free(track);
	      free(best);
	      free(scores);
	      free(hit.rest[5]);
	      free(hit.rest[4]);
	      free(hit.rest[3]);
	      free(hit.rest[2]);
	      free(hit.rest[1]);
	      free(hit.rest[0]);
	      free(hit.alignment[2]);
	      free(hit.alignment[1]);
	      free(hit.alignment[0]);

	      // Strand is reversed for indirect hoogsteen. "unreverse" for other match types.
	      if(current_match_type == MATCH_INDIRECT_REVERSE_HOOGSTEEN
		 || current_match_type == MATCH_INDIRECT_HOOGSTEEN)
		reverse_comp(reference_sequence,reference_length);

	      if(parallel_antiparallel == 1)
		{
		  if(current_match_type == MATCH_MIRANDA)
		    break;
		  revstring(reference_sequence);// back to original state
		}
	    }// End of parallel/antiparallel loop
	}// End of match type loop
    }// End of reference string loop
    fclose(reference_fp);
    mirna_processed++;
  }// End of query loop
  if(!brief_output)
    fprintf(fpout, "Scan Complete\n\n");
  destroy_ExpString(&(hit_summary.position_list));
  fflush(fpout);
  free(reference_id);
  free(reference_description);
  free(reference_sequence);
  free(query_id);
  free(query_description);
  free(query_sequence);
  return 1;
}
