
bin_PROGRAMS = trident

trident_SOURCES = trident.c output.c scan.c seqio.c swat.c pairs.c thermo.c utils.c ExpString.c version.c
trident_CFLAGS=-I../RNAlib/ -I../RNAlib/H
noinst_HEADERS = trident.h

LDADD = ../RNAlib/libRNA.a

version.c: 
	git rev-parse HEAD | awk ' BEGIN {print "#include \"version.h\""} {print "const char * build_git_sha = \"" $$0"\";"} END {}' > version.c
	date | awk 'BEGIN {} {print "const char * build_git_time = \""$$0"\";"} END {} ' >> version.c

.PHONY: version.c