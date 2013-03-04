#!/usr/bin/env python

def validate_file(infile):
    """
    Crawls an input file and validates the energy based on the sequence.
    
    Uses the trident.core python extension
    
    @param infile: File to be validated
    @type infile: file
    @return: No return value 
    """
    from trident import parser, core
    from trident.core import sequence_energy,compliment
    
    p = parser.Parser(infile)

    verbose = False

    score_counter = 0
    bad_score_counter = 0
    for score in p:
        if not score:
            raise parser.BrokenLine("Broken score line in {0}".format(infile.name))
        score_counter += 1
        query = score['query_seq']
        reference = score['reference_seq']
        if score['match_type'] == "indirect":
            reference = compliment(reference)
        energies = []
        for type_name in dir(core):
            if "MATCH" in type_name:
                if type_name in ["MATCH_DIRECT_REVERSE_HOOGSTEEN","MATCH_INDIRECT_REVERSE_HOOGSTEEN"] and score['base_type'] == "pyrimidine":
                    continue
                elif type_name in ["MATCH_DIRECT_HOOGSTEEN","MATCH_INDIRECT_HOOGSTEEN"] and score['base_type'] == "purine":
                    continue
                energy = sequence_energy(query,reference,core.__getattribute__(type_name))
                energy = round(energy,3)
                if verbose:
                    print("Energy (%s,%s,%s): %f" % (query,reference,type_name, energy))
                if not energy in energies:
                    energies.append(energy)
        if not float(score['energy']) in energies:
            bad_score_counter += 1
            print("Bad score energy in score number %d.\nHave: %s\nExpected: %s" % (score_counter, score['energy'], energies))

    print("%d of %d scores are bad" % (bad_score_counter,score_counter))

