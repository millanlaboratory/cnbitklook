#!/bin/bash
#09-09-23  Michele Tavella <michele.tavella@epfl.ch>

find . -iname "*.in" | xargs rm -rf
find . -iname "Makefile" | xargs rm -rf

rm -rvf build-aux 

rm -fv 	m4/ltsugar.m4 \
		m4/libtool.m4 \
		m4/ltversion.m4 \
		m4/lt~obsolete.m4 \
		m4/ltoptions.m4 \
		aclocal.m4

rm -vf configure \
	   config.log \
       config.status \
	   config/stamp-h1 \
	   config/config.h

rm -vfr autom4te.cache
rm -vf INSTALL

rm -vf tags

rm -vrf src/.deps \
        examples/.deps \
        tests/.deps

rm -vf libtool
