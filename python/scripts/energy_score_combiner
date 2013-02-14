#!/usr/bin/env python

from sys import stdout,argv
from getopt import getopt
import numpy as np
from progressbar import ProgressBar,Percentage,Bar

def print_usage():
    print("Usage: energy_score_combiner.py [-o FILE] FILE1 FILE2 FILE3 ...")
    print("")
    print("Combines multiple frequency files")

# defaults
output = stdout

short_opts = "o:"
long_opts = ["output"]

(opts,args) = getopt(argv[1:],short_opts,long_opts)

if not args:
    print_usage()
    exit(1)

for (opt,optarg) in opts:
    while opt[0] == "-":
        opt = opt[1:]
    if opt in ["o","output"]:
        output = open(optarg,"w")
    else:
        print("Unknown flag: %s" % opt)
        print_usage()
        exit(1)
        
hit_map = {}

progress_bar = None
progress_bar_counter = 0 
for filename in args:
    keys = np.genfromtxt(filename,delimiter="\t",usecols=0,dtype=str)
    vals = np.genfromtxt(filename,delimiter="\t",usecols=1,dtype=int)
    unique_keys = np.unique(keys)
    
    if output != stdout:
        print("Parsing %s" % filename)
        progress_bar = ProgressBar(widgets = [Percentage(), Bar()], maxval=len(unique_keys)).start()
        progress_bar_counter = 0 
    for key in unique_keys:
        total = np.sum(vals[keys==key])
        if not key in hit_map:
            hit_map[key] = total
        else:
            hit_map[key] += total
        if progress_bar:
            progress_bar_counter += 1
            progress_bar.update(progress_bar_counter)
    if progress_bar:
        print("") # Reset LR
        
for hit in sorted(hit_map.iterkeys()):
    output.write("%s\t%s\n" % (hit,hit_map[hit]))

        
        
        
            