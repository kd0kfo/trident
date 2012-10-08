#!/usr/bin/env python

import unittest
import trident.core

class TestSequenceEnergy(unittest.TestCase):
    def setUp(self):
        self.query_seqs = ["uAUGUAUGAAGAAAUGUAAGGu","uAUGUAUGAAGAAAUGUAAGgu"]
        self.ref_seqs = ["aAAGAAAGAAGAAAAGAAAGGa","tAGGAAAGAAAAGAAGAAAGaa"]
        self.energies = [-249.51,-185.88]
        self.match_types = [trident.core.MATCH_DIRECT_REVERSE_HOOGSTEEN,trident.core.MATCH_DIRECT_REVERSE_HOOGSTEEN]

    def test_energy(self):
        for i in [0,1]:
            query_seq = self.query_seqs[i]
            ref_seq = self.ref_seqs[i]
            energy = self.energies[i]
            match_type = self.match_types[i]
            self.assertTrue(abs(trident.core.sequence_energy(query_seq,ref_seq,match_type) - energy) < 1e-12)

#>mir1,mir1idealizedpurinestrand3to5,240.00,-249.51,2 21,13 34,20,75.00%,75.00%,uAUGUAUGAAGAAAUGUAAGGu, $ $ $ $$$$$$$ $ $$$$ ,aAAGAAAGAAGAAAAGAAAGGa,antiparallel,direct,purine
#>mir1,mir1idealizedpurinestrand3to5,156.00,-185.88,3 21,14 35,19,63.16%,63.16%,uAUGUAUGAAGAAAUGUAAGgu, $ $ $ $$$ $ $ $ $$$  ,tAGGAAAGAAAAGAAGAAAGaa,parallel,direct,purine


if __name__ == "__main__":
    unittest.main()
