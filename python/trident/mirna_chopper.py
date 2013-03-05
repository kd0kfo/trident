#!/usr/bin/env python
"""
Breaks up microrna files into arbitrarily smaller sizes

Requires an input sequence, an output file prefix and a 
chunk size (number of 70-character lines).
"""

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
    infile = open(filename,"r")
    output_file = None
    chunk_counter = 0
    file_counter = 0

    write_seq = True
    for line in infile.readlines():
        line = line.strip()
        if len(line) == 0:
            continue
        new_line = ""
        if line[0] == '>':
            new_line = line
            species = " ".join(new_line[1:].split(' ')[2:4])
            print(species)
            if specific_species != None:
                if specific_species == species:
                    write_seq = True
                else:
                    write_seq = False
                    continue
            if chunk_counter == 0:
                if output_file != None:
                    output_file.close()
                filename = new_line[1:].strip().split(' ')[0]
                if chunk_size > 1:
                    filename += "_{0}".format(file_counter)
                filename = filename.replace('*','_star')
                filename += ".fa"
                output_file = open(filename,'w')
                file_counter += 1
            chunk_counter = (chunk_counter + 1) % chunk_size
        else:
            new_line = line

        if write_seq:
            output_file.write(new_line)
            output_file.write("\n")
    #end of for loop
    if output_file != None:
        output_file.close()

    return file_counter

