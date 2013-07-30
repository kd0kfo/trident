=======
trident
=======
----------------------------------------------------
Heuristic microRNA-DNA binding site search algorithm
----------------------------------------------------

:Author: David.Coss@stjude.org
:Date:   2013-07-30
:Copyright: GPLv3.0
:Version: 0.4.3
:Manual section: 1
:Manual group: Trident

SYNOPSIS
========

  trident <query fasta> <reference fasta> [options]

DESCRIPTION
===========

Trident performs a search of microRNA - DNA triplex forming sites by assigning both a heuristic score and base pair binding energy to each possible alignment of microRNA and DNA strands. For each alignment location, trident calculates energy and score values for Direct and Indirect Hoogsteen and Reverse Hoogsteen binding types. If heuristic score and energy are over specified thresholds, the matching site is reported.

OPTIONS
=======

--help, -h     Display this message
--version, -v  Display version information
--revision     Get revision information
--license      Display license information

Core algorithm parameters
-------------------------

--sc S          Set score threshold to S                [DEFAULT: 140.0]
--ceil S        Set upper limit to score equal to S     [DEFAULT: off]
--en E          Set energy threshold to -E kcal/mol     [DEFAULT: 1.0]
--scale Z       Set scaling parameter to Z              [DEFAULT: 4.0]
--strict        Demand strict 5' seed pairing           [DEFAULT: off]

Alignment parameters
--------------------

--go X          Set gap-open penalty to X              [DEFAULT: -4.0]
--ge Y          Set gap-extend penalty to Y            [DEFAULT: -9.0]

General Options
---------------

--out file      Output results to file                  [DEFAULT: off]
--quiet         Output fewer event notifications        [DEFAULT: off]
--brief         Output only data                        [DEFAULT: on]
--rusage        Report Usage Data                       [DEFAULT: on]
--trim T        Trim reference sequences to T nt        [DEFAULT: off]
--noenergy      Do not perform thermodynamics           [DEFAULT: off]
--restrict file		Restricts scans to those between specific miRNAs and UTRs provided in a pairwise tab-separated file                     [DEFAULT: off]

--scaninfo file	 Output results to file                  [DEFAULT: off]

Example
=======

trident hsa_all.fasta hs_ref_GRCh37.p5_chr21.fa -out hsa_chr21.out -brief
    Runs trident using the provides microrna strands and H. sapiens chromosome 21. Brief output is used, producing one-line data output per hit found above the default threshold. Output is saved in the file hsa_chr21.out.

trident mmu_all.fasta mm_ref_GRCm38_chr18.fa -miranda
    Uses miranda algorithm to search the M. musculus chromosome 18 for hits against the provided microrna. Output is sent to standard output.


BUGS
====

Report bugs to trident-dev@list.stjude.org
