import numpy as np
import matplotlib.pyplot as plt
from getopt import getopt
from sys import argv

short_opts = "s:t:"
long_opts = ["scale=","title="]

def print_usage():
    print("Usage: energy_score_plots.py [-s INT] <input file>")

def reduce_array(arr, sort_col, freq_col):
    unique_keys = np.unique(arr[:,sort_col])
    
    reduced_data = []
    for i in unique_keys:
        new_freq = np.sum(arr[arr[:,sort_col] == i][:,freq_col])
        reduced_data.append([i,new_freq])
    
    return np.array(reduced_data).reshape(-1,2)


# defaults
scale_factor = 1
the_title = "Frequency"

(opts,args) = getopt(argv[1:],short_opts,long_opts)

if len(args) != 1:
    print_usage()
    exit(1)

infilename = args[0]

for (opt,optarg) in opts:
    while opt[0] == '-':
        opt = opt[1:]
    if opt in ["s","scale"]:
        scale_factor = float(optarg)
    elif opt in ["t","title"]:
        the_title = optarg
    else:
        print("Unknown flag: %s" % opt)
        print_usage()
        exit(1)
    
data = np.genfromtxt(infilename, delimiter="\t",skiprows=1)

if scale_factor != 1:
    data[:,2] /= scale_factor
    print("Frequency column (2) was divided by %s" % scale_factor)


# Make plots
for plot_info in [("Energy (kcal)", 0,"energy_test.png"), ("Score", 1,"score_test.png")]:
    reduced_data = reduce_array(data,plot_info[1],2)
    plt.figure()
    plt.bar(reduced_data[:,0],reduced_data[:,1])
    plt.title(the_title)
    plt.xlabel(plot_info[0])
    plt.ylabel("Frequency")
    plt.savefig(plot_info[2])
