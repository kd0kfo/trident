#!/usr/bin/env python
"""
Breaks up chromosome files into arbitrarily smaller sizes

Requires an input sequence, an output file prefix and a 
chunk size (number of 70-character lines).
"""

from sys import argv
import re


def error_out(msg):
    print(msg)
    exit(1)
def create_outfile_name(prefix,file_counter):
    return "%s-%d" % (prefix,file_counter)

def outfile_exists(prefix,file_counter):
    """
    Indictates wither or not the file that would be created already exists.

    @param prefix: File prefix
    @type prefix: String
    @param file_counter: Index of file being created
    @type file_counter: int
    @return: bool
    """
    from os.path import isfile
    return isfile(create_outfile_name(prefix,file_counter))

def open_outfile(prefix,file_counter):
    return open(create_outfile_name(prefix,file_counter) ,"w")

def create_header(old_header,chunksize,seq_size):
    import datetime
    header = ">chr"
    if old_header.find("chromosome") != -1:
        species = re.findall(r"\|\s*(\S*)\s*(\S*)\s*chromosome",old_header)
        m = re.findall(r"chromosome\s*(\w*)",old_header)
        if len(m) == 0:
            error_out("Missing chromosome label")
            header += m[0] + "|"
    elif old_header.find("mitochondrion"):
        species = re.findall(r"\|\s*(\S*)\s*(\S*)\s*mitochondrion",old_header)
        header += "mitochondrion|";
    else:
        header+= "Unknown Sequence Type|"
    header += "%s|" # to be filled in at file write
    header += r"%d|" # offset of segment in sequence, zero indexed
    header += "%d|" % seq_size # total sequence length
    header += str(chunksize) + "|"
    rightnow = datetime.date.today()
    header += rightnow.isoformat().replace('-','') + "|"
    m = re.findall(r"\s*(\S*) Primary Assembly",old_header)
    if len(m) == 0:
        header += "Unknown Assembly|"
    else:
        header += m[0] + "|"
    if len(species) == 0 or len(species[0]) < 2 or len(species[0][0]) == 0:
        error_out("Missing species name")
    species = species[0]
    header += "[%s.%s]" % (species[0][0],species[1])
    return header + "\n"
    
def chopper(filename,prefix,chunk_size,overwrite = True):
    """
    Opens the specified file and breaks it into 'chunk_size' number of lines.
    The results are placed in files, using the filename scheme prefix-index,
    where index is a zero-indexed value listing its position in the
    original file.

    @param overwrite: Indicates whether files should be overwritten. If False, the files are left unmodified.
    @type overwrite: bool
    """
    file = open(filename,"r")

    header = file.readline()
    if len(header) == 0 or header[0] != '>':
        error_out("Improper Header")
                
    nchars = 0
    for line in file:
        nchars += len(line.strip())
    file.seek(0)
    file.readline()
        
    header = create_header(header,chunk_size,nchars)

    file_counter = 1
    seq_offset = 0

    # Even if overwrite is True and we are not writing
    # to a file, we need to keep track of lines and file_counter 
    # values. So we need to turn writing on and off for each 
    # individual segment. This will be done using inhibit_writing
    inhibit_writing = False
    if not overwrite and outfile_exists(prefix,file_counter):
        inhibit_writing = True

    outfile = None
    if not inhibit_writing:
        outfile = open_outfile(prefix,file_counter)
        outfile.write(header % (file_counter,seq_offset))

    file_counter += 1
    counter = 1
    
    for line in file:
        if counter > 0 and counter % chunk_size == 0:
            if not inhibit_writing:
                outfile.write(line)
                outfile.close()
            if not overwrite and outfile_exists(prefix,file_counter):
                inhibit_writing = True
            if not inhibit_writing:
                outfile = open_outfile(prefix,file_counter)
                outfile.write(header % (file_counter,seq_offset))
            file_counter += 1
            counter = 0
        if not inhibit_writing:
            outfile.write(line)
        seq_offset += len(line.strip())
        counter += 1

    if outfile:
        outfile.close()

    return file_counter-1

def main():
    
    if len(argv) < 4:
        error_out("Usage: %s <filename> <output prefix> <chunk size>" % argv[0])

    filename = argv[1]
    prefix = argv[2]
    chunk_size = int(argv[3])
    if chunk_size < 2:
        error_out("Invalid size. Must be a positive integer")
        chunk_size -= 1 # decrement for use with mod

    num_files = chopper(filename,prefix,chunk_size)
    print("Divided %s into %d segments" % (filename, num_files))


if __name__ == "__main__":
    main()
