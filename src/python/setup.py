#!/usr/bin/env python

from distutils.core import setup, Extension

extension_sources = ['trident.py.c']
for i in ["output.c", "scan.c", "seqio.c", "swat.c", "pairs.c", "thermo.c", "utils.c", "ExpString.c", "version.c"]:
    extension_sources.append("../%s" % i)

for i in ["fold_vars.c", "energy_par.c", "utils.c", "fold.c", "params.c"]:
    extension_sources.append("../../RNAlib/%s" % i)

mod_core = Extension('core', sources = extension_sources, include_dirs=['..','../..',".","../../RNAlib","../../RNAlib/H"], define_macros=[("USE_PYTHON","1"),("HAVE_CONFIG_H","1")])

setup (name ='trident',
       version = '0.3',
       url = 'http://trident.stjude.org',
       license = 'GPL v3',
       description = 'Utility functions for analyzing microrna binding to DNA',
       author_email='David.Coss@stjude.org',
       packages = ['trident'],
       ext_package = 'trident',
       ext_modules = [mod_core])
