#!/usr/bin/env python

from setuptools import setup,Extension

extension_sources = ['trident.py.c']
for i in ["output.c", "scan.c", "seqio.c", "swat.c", "pairs.c", "thermo.c", "utils.c", "ExpString.c", "version.c"]:
    extension_sources.append("../src/%s" % i)

for i in ["fold_vars.c", "energy_par.c", "utils.c", "fold.c", "params.c"]:
    extension_sources.append("../RNAlib/%s" % i)

mod_core = Extension('core', sources = extension_sources, include_dirs=['..','../src',"../RNAlib","../RNAlib/H"], define_macros=[("USE_PYTHON","1"),("HAVE_CONFIG_H","1")])

the_scripts = ['scripts/energy_score_combiner', 'scripts/energy_score_plots',
               'scripts/energy_score_ranker','scripts/energy_score_mapper',
               'scripts/chromosome_chopper', 'scripts/energy_score_interpolator',
               'scripts/energy_score_classifier', 'scripts/mapper',
               'scripts/mirna_chopper','scripts/score_converter',
               'scripts/score-gene_counter', 'scripts/score-gene_distance',
               'scripts/kmer_counter']

setup (name ='trident',
       version = '1.0.0',
       url = 'http://trident.stjude.org',
       license = 'GPL v3',
       description = 'Utility functions for analyzing Trident Results.',
       author='David Coss',
       author_email='David.Coss@stjude.org',
       packages = ['trident'],
       scripts = the_scripts,
       ext_package = 'trident',
       ext_modules = [mod_core],
       depends = ['../config.h'])

