#!/bin/sh

echo Running Doxygen
doxygen Doxyfile

echo
echo Document Trident Python Class
epydoc -o python_api --name trident ../src/python/trident
