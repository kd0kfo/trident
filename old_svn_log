const char revision_string[] = "$Id: revision_string.c 141 2012-09-24 18:34:37Z dcoss $";

#if 0 //Revision Log
------------------------------------------------------------------------
r135 | dcoss | 2012-09-11 15:30:00 -0500 (Tue, 11 Sep 2012) | 3 lines

Changed the parse to allow a reference id to not require the use of '|' separated fields, e.g. genomic offset.


------------------------------------------------------------------------
r132 | dcoss | 2012-09-11 14:40:10 -0500 (Tue, 11 Sep 2012) | 3 lines

Changed exit's to return None.


------------------------------------------------------------------------
r131 | dcoss | 2012-09-11 14:25:38 -0500 (Tue, 11 Sep 2012) | 3 lines

Updated documentation.


------------------------------------------------------------------------
r118 | dcoss | 2012-08-27 13:55:05 -0500 (Mon, 27 Aug 2012) | 3 lines

Incremented the minor version number.


------------------------------------------------------------------------
r117 | dcoss | 2012-08-27 13:46:34 -0500 (Mon, 27 Aug 2012) | 3 lines

Updated energy values after new calculations have been done for deprotonated C.


------------------------------------------------------------------------
r107 | dcoss | 2012-07-31 14:35:31 -0500 (Tue, 31 Jul 2012) | 7 lines

Updated schema.

Created a microrna file chopper.

Updated database models.


------------------------------------------------------------------------
r104 | dcoss | 2012-07-31 11:37:39 -0500 (Tue, 31 Jul 2012) | 3 lines

Removed compile. I don't believe it belongs here.


------------------------------------------------------------------------
r98 | dcoss | 2012-07-30 14:18:09 -0500 (Mon, 30 Jul 2012) | 3 lines

Added energy values for non-miranda match types based on free energy calculations.


------------------------------------------------------------------------
r95 | dcoss | 2012-07-30 09:59:34 -0500 (Mon, 30 Jul 2012) | 8 lines

Implemented Parser as iterator.

Factored out score token printing to str_score.

str_score now lists the score data alphabetically by type.



------------------------------------------------------------------------
r94 | dcoss | 2012-07-30 09:29:35 -0500 (Mon, 30 Jul 2012) | 3 lines

Changed the parser to use an interator to step through the score lines. This will make using the parser as part of another script much easier.


------------------------------------------------------------------------
r88 | dcoss | 2012-07-26 13:27:40 -0500 (Thu, 26 Jul 2012) | 3 lines

Removed config.log. It does not belong in the repo


------------------------------------------------------------------------
r87 | dcoss | 2012-07-26 13:22:55 -0500 (Thu, 26 Jul 2012) | 5 lines

Adapted chromosome_chopper to use different types of dna sequences besides chromosomes 1-22.

Changed the python module name to trident.


------------------------------------------------------------------------
r86 | dcoss | 2012-07-25 14:34:36 -0500 (Wed, 25 Jul 2012) | 2 lines

Fixed typo with match type based reversal

------------------------------------------------------------------------
r85 | dcoss | 2012-07-25 14:12:27 -0500 (Wed, 25 Jul 2012) | 3 lines

Changed package name to use a capital 'T'


------------------------------------------------------------------------
r84 | dcoss | 2012-07-25 14:02:26 -0500 (Wed, 25 Jul 2012) | 7 lines

Moved ple to trident. Did the same thing for ple.c and ple.h

Added CU type matching. The match type is now displayed at the end of the score line.

Removed tree.txt.


------------------------------------------------------------------------
r83 | dcoss | 2012-07-23 13:10:27 -0500 (Mon, 23 Jul 2012) | 3 lines

Put DNA scanning output behind the debug flag where it belongs.


------------------------------------------------------------------------
r82 | dcoss | 2012-07-20 15:17:18 -0500 (Fri, 20 Jul 2012) | 11 lines

Version 0.2

Fixed position labeling in output.

Incremented version number in configure.in. Included its use as the reported program version, rather than the SVN revision.

Removed Makefile's from SVN.




------------------------------------------------------------------------
r81 | spaugh | 2012-07-20 10:46:35 -0500 (Fri, 20 Jul 2012) | 1 line

Changed line 318 of output.c to reverse locations of indirect rather than parallel hits
------------------------------------------------------------------------
r80 | spaugh | 2012-07-18 22:18:38 -0500 (Wed, 18 Jul 2012) | 1 line

Fixed checkin errors in last commit
------------------------------------------------------------------------
r79 | spaugh | 2012-07-18 21:58:54 -0500 (Wed, 18 Jul 2012) | 1 line

Cleaned up outdated ancilary sources files in preparation for updating
------------------------------------------------------------------------
r70 | dcoss | 2012-07-17 11:30:24 -0500 (Tue, 17 Jul 2012) | 5 lines

Changed ref_coords to be the absolute position in the original dna file.

Created start and end position fiels for query and reference in the score dict


------------------------------------------------------------------------
r69 | dcoss | 2012-07-17 10:48:14 -0500 (Tue, 17 Jul 2012) | 9 lines

Changed formatting of sequence positions to be relative to the
beginning of the original input file, making parallel and
antiparallel the same with respect to direction.

Added sequence offset and total sequence length to chromosome
chopper header lines.



------------------------------------------------------------------------
r68 | dcoss | 2012-07-16 13:08:41 -0500 (Mon, 16 Jul 2012) | 3 lines

Added a return value to chopper


------------------------------------------------------------------------
r67 | dcoss | 2012-07-16 09:15:19 -0500 (Mon, 16 Jul 2012) | 3 lines

Added fix for the CR character in each line of input


------------------------------------------------------------------------
r66 | dcoss | 2012-07-16 08:38:18 -0500 (Mon, 16 Jul 2012) | 6 lines

Created setup script to make installing the python scripts easier.

Made the functions of the scripts callable by creating a ple module.



------------------------------------------------------------------------
r59 | dcoss | 2012-07-12 13:54:02 -0500 (Thu, 12 Jul 2012) | 3 lines

Fixed sequence_energy: Added missing char --> base pair index conversion


------------------------------------------------------------------------
r52 | dcoss | 2012-07-12 09:03:47 -0500 (Thu, 12 Jul 2012) | 1 line

Backed up old we code, to be replaced by django project
------------------------------------------------------------------------
r51 | dcoss | 2012-07-10 15:05:36 -0500 (Tue, 10 Jul 2012) | 3 lines

Replaced place holder with actual chunk size


------------------------------------------------------------------------
r50 | dcoss | 2012-07-10 14:49:44 -0500 (Tue, 10 Jul 2012) | 3 lines

Started a script to segment fasta files.


------------------------------------------------------------------------
r49 | dcoss | 2012-07-09 11:11:50 -0500 (Mon, 09 Jul 2012) | 3 lines

Added debug and static enable flags for configure


------------------------------------------------------------------------
r48 | dcoss | 2012-07-09 09:46:19 -0500 (Mon, 09 Jul 2012) | 3 lines

Added setup.sh script to rebuild makefiles if configure.in is changed.


------------------------------------------------------------------------
r47 | dcoss | 2012-07-05 15:56:53 -0500 (Thu, 05 Jul 2012) | 3 lines

Added an rusage flag


------------------------------------------------------------------------
r46 | dcoss | 2012-07-05 13:06:07 -0500 (Thu, 05 Jul 2012) | 3 lines

Starting python output parser.


------------------------------------------------------------------------
r40 | dcoss | 2012-07-03 09:49:40 -0500 (Tue, 03 Jul 2012) | 3 lines

Fixed bug when calculating energy of a sequence with a gap.


------------------------------------------------------------------------
r39 | dcoss | 2012-06-29 15:43:56 -0500 (Fri, 29 Jun 2012) | 2 lines

Improved output

------------------------------------------------------------------------
r38 | dcoss | 2012-06-29 15:18:41 -0500 (Fri, 29 Jun 2012) | 4 lines

Fixed error in reverse_comp

Cleaned -brief output

------------------------------------------------------------------------
r37 | dcoss | 2012-06-28 13:45:09 -0500 (Thu, 28 Jun 2012) | 3 lines

Added a scaninfo_print function that will print information about the scan 
	algorithm to a file. This is invoked using the "-scaninfo" flag.

------------------------------------------------------------------------
r36 | dcoss | 2012-06-28 09:55:06 -0500 (Thu, 28 Jun 2012) | 3 lines

Added a brief output mode, where only the score lines are printed. In brief mode the sequence structure is added to this line as well, to provent a loss of information in brief mode.


------------------------------------------------------------------------
r35 | dcoss | 2012-06-27 16:18:06 -0500 (Wed, 27 Jun 2012) | 4 lines

Fixed problems in output formatting.

Updated strand end labels to correspond to parallel/antiparallel alignment

------------------------------------------------------------------------
r34 | dcoss | 2012-06-27 15:38:37 -0500 (Wed, 27 Jun 2012) | 3 lines

Added Parallel/Antiparallel alignment testing.


------------------------------------------------------------------------
r33 | dcoss | 2012-06-21 14:10:29 -0500 (Thu, 21 Jun 2012) | 9 lines

Changed ple to use pair wise energy calculation for non-miranda rules.
After a sequence is found, pairs will have their energies summed to provided
the energy of the alignment. The energy is stored a matrix (pair_energy_mat)
which is a 5x5 matrix where each row and column correspond to a base (see thermo.c).

Changed get_ple_revision to return the substring of the revision string that only contains the
revision number.


------------------------------------------------------------------------
r32 | dcoss | 2012-04-19 14:17:07 -0500 (Thu, 19 Apr 2012) | 5 lines

Integrated new revision tag into ple's output.

Removed redundant initialization from swat.c


------------------------------------------------------------------------
r31 | dcoss | 2012-04-19 14:10:56 -0500 (Thu, 19 Apr 2012) | 3 lines

Added Revision tag const char string.


------------------------------------------------------------------------
r30 | dcoss | 2012-03-28 15:59:22 -0500 (Wed, 28 Mar 2012) | 3 lines

Cleaned up lines inserted for debugging.


------------------------------------------------------------------------
r28 | dcoss | 2012-03-26 13:52:42 -0500 (Mon, 26 Mar 2012) | 5 lines

Added extra strand representation in output for Hoogsteen matching.

Fixed wobble base matching in miranda.


------------------------------------------------------------------------
r27 | dcoss | 2012-03-21 15:56:23 -0500 (Wed, 21 Mar 2012) | 3 lines

Added new test case where T --> U.


------------------------------------------------------------------------
r26 | dcoss | 2012-03-21 15:55:52 -0500 (Wed, 21 Mar 2012) | 3 lines

Working on adding indirect Hoogsteen. Work in progress.


------------------------------------------------------------------------
r25 | dcoss | 2012-03-21 14:03:32 -0500 (Wed, 21 Mar 2012) | 6 lines

Added a global variable, current_match_type, to indicate to match functions which type, i.e. miranda versus ple, is currently being examed. Updated functions that used the global_match_types array accordingly.

Changed -ple flag to -miranda, making ple rules the default. Currently, ple with do PLE or Miranda rules, but not both.



------------------------------------------------------------------------
r24 | dcoss | 2012-03-19 16:03:16 -0500 (Mon, 19 Mar 2012) | 5 lines

Restructured RNAlib to make it more friendly towards including headers (esp H/pair_mat.h). However, this breaks RNAlib if no fill_BP_pair function is supplied, i.e. the user must instruct RNAlib on how it should fill the pair definitions. This *should* be a feature. However, it makes RNAlib less like it was originally designed. It should now be rolled into this packages, should not be standalone anymore.

Changed ple.c to fill the BP_pair matrix according to ple match rules.


------------------------------------------------------------------------
r19 | dcoss | 2012-03-16 15:16:26 -0500 (Fri, 16 Mar 2012) | 3 lines

Adjusted scoring functions, esp get_nt_nt_seq_scores, to use both miranda rules and ple rules.


------------------------------------------------------------------------
r18 | spaugh | 2012-03-15 15:58:29 -0500 (Thu, 15 Mar 2012) | 1 line

updated ple.h for REVERSE
------------------------------------------------------------------------
r17 | spaugh | 2012-03-15 09:29:07 -0500 (Thu, 15 Mar 2012) | 1 line

Added additional test case expected results, changed | to $ in other examples,  $ looks weird, might need another char instead later
------------------------------------------------------------------------
r16 | spaugh | 2012-03-15 09:14:03 -0500 (Thu, 15 Mar 2012) | 1 line

Added -ple command line flag to output.c, changed binding parameters in swat.c for nt1==microRNA, nt2==dsDNA, added Reverse to Hoogsteen binding variables
------------------------------------------------------------------------
r15 | spaugh | 2012-03-15 08:56:25 -0500 (Thu, 15 Mar 2012) | 1 line

Added some additional basic examples
------------------------------------------------------------------------
r14 | spaugh | 2012-03-15 00:26:17 -0500 (Thu, 15 Mar 2012) | 1 line

Added example_result2.txt a second test case result
------------------------------------------------------------------------
r13 | dcoss | 2012-03-14 16:04:48 -0500 (Wed, 14 Mar 2012) | 4 lines

Changed swat to use arrays to store representations, which allow initialize_scores to operate on all match types. This will allow the user to specify which types should be searched at the command line.



------------------------------------------------------------------------
r12 | dcoss | 2012-03-14 15:13:28 -0500 (Wed, 14 Mar 2012) | 5 lines

Added missing header for FILE type.

Changed match times to enum, in favor of storing the target match types in an array.


------------------------------------------------------------------------
r11 | dcoss | 2012-03-14 15:06:10 -0500 (Wed, 14 Mar 2012) | 5 lines

Integrated base match functions into one function, by selecting the match type as a parameter.

Removed the build .deps directory.


------------------------------------------------------------------------
r10 | spaugh | 2012-03-14 12:02:42 -0500 (Wed, 14 Mar 2012) | 1 line

added testcases folder and an example result
------------------------------------------------------------------------
r9 | spaugh | 2012-03-13 17:07:49 -0500 (Tue, 13 Mar 2012) | 1 line

changed swat.c binding matrix, seems to work, now testing
------------------------------------------------------------------------
r8 | spaugh | 2012-03-13 15:56:11 -0500 (Tue, 13 Mar 2012) | 1 line

changed pair_mat.h to miranda like pairings
------------------------------------------------------------------------
r7 | spaugh | 2012-03-13 09:16:20 -0500 (Tue, 13 Mar 2012) | 1 line

Updated Author list
------------------------------------------------------------------------
r6 | spaugh | 2012-03-09 17:26:39 -0600 (Fri, 09 Mar 2012) | 1 line

Added ple code
------------------------------------------------------------------------
#endif //Revision Log
