#!/usr/bin/env python
"""
parser.py is used to parse the output of trident, translating the score lines into database entries.

This may run as a standalone script or as a module, trident.parser

"""

import sys, os 

score_keys = ['query_id','reference_id','score','energy','query_coords','ref_coords','length','identity','similarity','query_seq','match_seq','reference_seq','orientation','match_type', 'base_type','query_start', 'query_end', 'ref_start', 'ref_end']

def parser_usage():
    """
    Display usage of trident
    """
    print("Usage: %s <filename>" % sys.argv[0])
    print("Parses the output of trident")

def score_str_to_dict(line):
    if len(line) == 0:
        return None
    if line[0] == '>': 
        # This is a score line
        line = line[1:]
    if not len(line) or line[0] == '>': 
        # Broken line or summary score, respectively
        return None
    line = line.strip()
    tokens = line.split(',')
    if len(tokens) != len(score_keys) - 4: # 4: 2x(query_coords) + 2x(ref_coords)
        from sys import stderr
        stderr.write("Invalid score line: \"%s\"\n" % line)
        return None
            
    score = dict(zip(score_keys,tokens))
    ref_id = score['reference_id']
    if "|" in ref_id:
        tokens = ref_id.split('|')
        if len(tokens) < 3:
            from sys import stderr
            stderr.write("Missing sequence offset.\nSequence: %s" % line)
            return None
        offset = int(tokens[2])
    else:
        offset = 0

    tokens = score['ref_coords'].split(' ')
    if len(tokens) < 2:
        from sys import stderr
        stderr.write("Broken reference coordinates.\n")
        return None
    
    score['ref_start'] = int(tokens[0]) + offset
    score['ref_end'] = int(tokens[1]) + offset
    tokens = score['query_coords'].split(' ')
    if len(tokens) < 2:
        from sys import stderr
        stderr.write("Broken query coordinates.\n")
        return None

    score['query_start'] = int(tokens[0])
    score['query_end'] = int(tokens[1])
    return score # End of score_str_to_dict

def parse_file(file):
    """
    Prints score information to stdout.

    Parameters: File Object
    """
    
    score = None
    line = file.readline().strip()
    return score_str_to_dict(line)    

def str_score(score):
    retval = ""
    for name in sorted(score.iterkeys()):
        val = score[name]
        if name == "ref_coords":
            retval += "%s: %d %d\n" %(name,  score['ref_start'],  score['ref_end'])
        elif name == "query_coords":
            retval += "%s: %d %d\n" %(name,  score['query_start'],  score['query_end'])
        elif name in ['ref_start', 'ref_end', 'query_start', 'query_end']:
            continue
        else:
            retval += "%s: %s\n" % (name, val)
    return retval

class Parser:
    def __init__(self,file):
        self.file = file
    
    def __iter__(self):
        return self

    def next(self):
        retval = parse_file(self.file)
        if retval == None or len(retval) == 0:
            raise StopIteration
        return retval



def main():
    from getopt import getopt
    from sys import argv

    verbose = False
    
    (optlist,args) = getopt(argv[1:],'v',['version'])
    
    for (opt,val) in optlist:
        while opt[0] == '-':
            opt = opt[1:]
        if opt in 'version':
            print("1.0")
            exit(0)
        else:
            from sys import stderr
            stderr.write("Unknown option '%s'\n" % opt)
            exit(1)
    
    if len(args) == 0:
        parser_usage()
        exit(1)
    filename = args[0]
    if not os.path.isfile(filename):
        print("\"%s\" is not a file\n" % filename)
        parser_usage()
        exit(1)

    with open(filename,'r') as file:
        p = Parser(file)
        for score in p:
            print(str_score(score))

if __name__ == "__main__": main()


