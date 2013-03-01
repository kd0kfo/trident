#!/usr/bin/env python
"""
Breaks up microrna files into arbitrarily smaller sizes

Requires an input sequence, an output file prefix and a 
chunk size (number of 70-character lines).
"""

from sys import argv
import re


def error_out(msg):
    print(msg)
    exit(1)

def chopper(filename,chunk_size,specific_species = None):
    """
    Takes a microrna fasta file and splits it into smaller files.
    
    @param filename: Name of file to be split
    @type filename: str
    @param chunk_size: Maximum number of micrornas per new file
    @type chunk_size: int
    @param specific_species:  Optional name of a species to look for when extracting microrna. If this is not None, all other microrna will be ignored. Default: None
    @type specific_species: str
    @return: Number of files written
    """
    file = open(filename,"r")
    output_file = None
    chunk_counter = 0
    file_counter = 0

    write_seq = True

    for line in file.readlines():
        if len(line) == 0:
            continue
        new_line = ""
        if line[0] == '>':
            new_line = line
            species = new_line[1:].split(' ')[0]
            species = species.split('-')[0]
            if specific_species != None:
                if specific_species == species:
                    write_seq = True
                else:
                    write_seq = False
                    continue
            if chunk_counter == 0:
                if output_file != None:
                    output_file.close()
                filename = new_line[1:].split(' ')[0]
                if chunk_size > 1:
                    filename += "_and_more"
                filename = filename.replace('*','_star')
                filename += ".fa"
                output_file = open(filename,'w')
                file_counter += 1
            chunk_counter = (chunk_counter + 1) % chunk_size
        else:
            new_line = line

        if write_seq:
            output_file.write(new_line)
    #end of for loop
    if output_file != None:
        output_file.close()

    return file_counter

def main():
    from getopt import getopt
    
    (optlist, args) = getopt(argv[1:],'s:',['species'])

    specific_species = None

    for (opt, val) in optlist:
        while opt[0] == '-':
            opt = opt[1:]
        if opt in ['s', 'species']:
            specific_species = val
        else:
            print("Unknown option: '%s'" % opt)

    if len(args) < 2:
        error_out("Usage: %s <filename> <number of mirna's>" % argv[0])

    filename = args[0]
    mirna_count = int(args[1])
    if mirna_count < 0:
        error_out("Invalid size. Must be a positive integer")

    num_files = chopper(filename,mirna_count,specific_species)
    print("Divided %s into %d segments" % (filename, num_files))


if __name__ == "__main__":
    main()
