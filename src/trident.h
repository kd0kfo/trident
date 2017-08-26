/*! \file trident.h
 * \brief Trident header file 
 */


#include <stdio.h>

//! 
/*! Match Types */
/*! _REVERSE_HOOGSTEEN are purine match types */
/*! _HOOGSTEEN are pyrimidine match types */
enum {MATCH_MIRANDA=0,
	  MATCH_DIRECT_REVERSE_HOOGSTEEN,
	  MATCH_INDIRECT_REVERSE_HOOGSTEEN, 
	  MATCH_DIRECT_HOOGSTEEN, 
	  MATCH_INDIRECT_HOOGSTEEN, 
	  NUM_MATCH_TYPES};

static char* match_type_name[] = {"Miranda","Direct Reverse Hoogsteen","Indirect Reverse Hoogsteen","Direct Hoogsteen","Indirect Hoogsteen","Unknown"};

/*! Size of base-sized arrays (thus globally defined) */
#define NUM_SCORED_BASES 5

/** List of Nucleotide Bases
 *
 *  Converts ASCII to a base index (0,1,2,3,4,INT_MAX) = (c,g,a,u=t,place holders, all other characters 
 *
 *  See initialize_bases() in swat.c
 *
 */
extern int bases[];

/* Adjustable Algorithm Parameters*/
extern double scale;
extern int strict;
extern int debug;
extern double gap_open;
extern double gap_extend;
extern double score_threshold;/*!< Defines the threshold for hit scores, hits with score values greater than this will be reported */
extern double score_ceiling;
extern double energy_threshold;/*!< Defines the threshold for hit energies, hits with energy values less than this will be reported */
extern int length_5p_for_weighting;
extern int length_3p_for_weighting;
extern int key_value_pairs;
/*! If true, energy values are not calculated for hits */
extern int no_energy;
extern int outfile;
extern int truncated;
extern int restricted;
extern int verbosity, brief_output, rusage_output;// verbosity = quite verbose, brief = short form of output, without lose of information, rusage = report usage info

extern int global_match_types[NUM_MATCH_TYPES];
extern int current_match_type;// correspond to match type enum above.
extern char alignment_match_representations[];
extern int alignment_match_scores[];

/* Expandable string type (allocates memory as needed) */
struct ExpStringT;
typedef struct ExpStringT ExpString;
void create_ExpString(ExpString **es);
void destroy_ExpString(ExpString **es);
void clear_ExpString(ExpString *es);
int length_ExpString(ExpString *es);
void append_char_ExpString(ExpString *es, char c);
void append_string_ExpString(ExpString *es, char *c);
void append_int_ExpString(ExpString *es, int i);
char *access_ExpString(ExpString *es);

/* Structure to store individual hit information*/
typedef struct hit_struct
{
	double score;
	int query_start;
	int query_end;
	int ref_start;
	int ref_end;
	char* alignment[3];
	char* rest[6];
} hit_struct;

/* Score structure for non-optimal path detection*/
typedef struct score_struct
{
	int score;
	int path;
	int query_trace_end;
	int reference_trace_end;
} score_struct;

/* Structure for pair-wise restriction*/
#define MAX_PAIR_SEQ_ID_LEN 200
typedef struct pair_struct
{
	char identifier1[MAX_PAIR_SEQ_ID_LEN];
	char identifier2[MAX_PAIR_SEQ_ID_LEN];
} pair_struct;

/* Functions Declarations */
/*   in scan.c */
int find_targets(FILE*, FILE*, pair_struct*, int, char*);
/*   in swat.c */
void traceback(int**, int***, char*, char*, int, int, hit_struct*, double);
int testfor_overlap(int*, int*, int*, int, int);
void build_matrix(int**, int***, int**, int**, int**, int**, char*, char*, int, int, score_struct*, int*);
void get_nt_nt_seq_scores(int**, char*, char*, int, int);
void initialize_bases();
void initialize_scores();
/*   in seqio.c */
int readinseq(FILE*, char**, char**, char**, int *);
void initialize_seqio_buffers();
void destroy_seqio_buffers();
/*   in utils.c */
void revstring(char s[]);
void clear_hit(hit_struct*, int, int);
int cmpscores(const void*, const void*);
void string_toupper(char*);
/*   in output.c */
int parse_command_line(int, char**, char*, char*, char*, char*);
void printhit(char* query_id, int query_length, int reference_length, char* reference_id, hit_struct* hit, double energy, int keyval_mode, int is_parallel, FILE* fpout);
void print_parameters(char*, char*, FILE*);
/*   in pairs.c */
int load_pairs(FILE*, pair_struct**);
int find_pair(char*, char*, int, pair_struct*);
/*   in thermo.c */
double get_energy(hit_struct*);

/**
  * Check for condition. If condition is true, nothing happens. If condition is false, the provided
  * error message is displayed, using vargs, and the program exits. If errno is set, that exit value
  * is returned and the result of strerror is displayed. Otherwise, the exit value is 1.
  */
void full_assert(int condition,const char *format,...);

const char* str_match_type(int index);
void scaninfo_print(const char *format, ...);
void scaninfo_dump_matrix(int **a_nt_nt,int query_length,int reference_length);

FILE* json_open(const char *filename);
void json_score();
void json_close();
