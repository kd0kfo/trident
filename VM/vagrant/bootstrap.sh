#!/bin/sh
# Provisioning script for trident VM
WGET="wget --no-check-certificate"
PYTHONHOME=/opt/python/2.7.7
export PATH=$PYTHONHOME/bin:$PATH
export LD_LIBRARY_PATH=$PYTHONHOME/lib:$LD_LIBRARY_PATH

function error_out() {
	echo Testing Failed on line $1.
	exit 1
}

trap 'error_out ${LINENO}' ERR
 
yum install -y gcc gcc-gfortran gcc-c++ blas-devel lapack-devel git zlib-devel openssl-devel autoconf automake make

cd /usr/src
if ! test -d trident;then
	git clone /tridentrepo trident
fi
cd /usr/src
if ! test -f /var/pythonbuilt ;then
	# Build python 2.7
	$WGET https://www.python.org/ftp/python/2.7.7/Python-2.7.7.tgz
	tar xzvf Python-2.7.7.tgz
	cd Python-2.7.7
	./configure --prefix=$PYTHONHOME --enable-shared
	make install

	# install easy_install and pip
	$WGET https://bootstrap.pypa.io/ez_setup.py -O /dev/stdout | $PYTHONHOME/bin/python
	date > /var/pythonbuilt
fi
easy_install pip
pip install Django==1.4

cd /usr/src/trident
sh setup.sh
./configure 
make
make install

cd python
pip install -r requirements.txt
python setup.py install

cd ../
./unit_test.sh

echo This trident machine was built using Vagrant on $(date) > /etc/motd
