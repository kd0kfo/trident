#!/bin/sh

echo Running Doxygen
doxygen Doxyfile

echo
echo Document Trident Python Class
epydoc -o python_api --name trident ../python/trident

echo
echo Running Javadoc on Hadoop code
javadoc -d java_api ../hadoop/*.java ../hadoop/TridentHadoop/*.java


RST2MAN=rst2man.py
which $RST2MAN  &> /dev/null

if [[ $? -ne 0 ]];then
    echo Could not find $RST2MAN. Skipping...
else
    if [[ ! -d man/man1 ]];then
	mkdir man/man1
    fi
    
    $RST2MAN trident.rst > man/man1/trident.1
fi