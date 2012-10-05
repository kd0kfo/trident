#!/usr/bin/env python

from distutils.core import setup, Extension
 
mod_core = Extension('core', sources = ['trident.py.c','../thermo.c'], include_dirs=['..',"."], define_macros=[("USE_PYTHON","1"),("HAVE_CONFIG_H","1")])

setup (name ='trident',
       version = '0.3',
       url = 'http://trident.stjude.org',
       license = 'GPL v3',
       description = 'Utility functions for analyzing microrna binding to DNA',
       author_email='David.Coss@stjude.org',
       packages = ['trident'],
       ext_package = 'trident',
       ext_modules = [mod_core])
