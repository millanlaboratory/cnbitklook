/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCFLAGS_HPP
#define CCFLAGS_HPP

/* 09-09-28  Michele Tavella <michele.tavella@epfl.ch>
 * 
 * By enabling this flag, after calling each i_* CallBack*, 
 * a virtual function called p* is executed.
 *
 * This option does not affect the way the things work, but 
 * it is possible to custumize things a little more deeply, 
 * switching from "uses a" to "is a".
 *
 * If unsure, do not define it! You will save you CPU a couple 
 * microseconds.
 */
//#define CCENABLE_PLACEHOLDERS

/* 09-09-29  Michele Tavella <michele.tavella@epfl.ch>
 * Non-blocking sockets
 */
#define CCCORE_1KB				      1024 
#define CCCORE_1MB				   1048576
#define CCASYNC_WAIT_ACCEPT		   250.000
#define CCASYNC_WAIT_CONNECT		 1.000
#define CCASYNC_WAIT_RECV			 5.000
#define CCASYNC_WAIT_DRAW			 0.020
#define CCASYNC_WAIT_BEEP			 0.200
#define CCASYNC_WAIT_EVENTS 		 0.050
#define CCASYNC_WAIT_THREAD		 	15.000
#define CCASYNC_WAIT_POLL			 0.500
#define CCASYNC_WAIT_MACCEPT	   250.000
#define CCASYNC_WAIT_MRECV			 5.000

/*! \mainpage libcnbicore
 *
 * \section sec_intro Introduction
 * libcnbicore implements the low-level functionalities of the CNBIToolkit
 * library collection. It provides C++ classes that implement threading,
 * networking and triggering. Furthermore, it provides some interfaces used by
 * other libraries in the CNBIToolkit.
 *
 * \subsection sec_developers Developers
 * The development of libcnbicore is lead by
 * <PRE>
 * Michele Tavella <michele.tavella@epfl.ch></PRE>
 * Each file in the library reports the name of the author and his email
 * address. In case of questions, report directly to the author.
 * If the file is edited by multiple authors, get in touch with the first one in
 * the list.
 *
 * \subsection sec_license Licensing and distribution
 * This code is property of
 * <PRE>
 * Michele Tavella <michele.tavella@epfl.ch></PRE>
 * and cannot be redistributed without his permission.
 *
 * \section sec_install Installation
 * \subsection sec_install_step1 Dependencies
 * <PRE>
 * liblpttrigger
 * libtransport</PRE>
 *
 * \subsection sec_release Release
 * <PRE>
 * sh autogen.sh
 * mkdir build
 * cd build/ 
 * ../configure --prefix=$HOME/Build/ CXXFLAGS="-Wall" CFLAGS="-Wall"
 * make
 * make install</PRE>
 * 
 * \subsection sec_optimized Release (optimized)
 * <PRE>
 * sh autogen.sh
 * mkdir build
 * cd build/ 
 * ../configure --prefix=$HOME/Build/ --disable-shared CXXFLAGS="-O3 -Wall" CFLAGS="-O3 -Wall"
 * make
 * make install</PRE>
 *
 * \subsection sec_debug Debug 
 * <PRE>
 * sh autogen.sh
 * mkdir build
 * cd build/ 
 * ../configure --prefix=$HOME/Devel/ --disable-shared CXXFLAGS="-g3 -O0 -Wall" CFLAGS="-g3 -O0 -Wall"
 * make
 * make install</PRE>
 *
 * \section sec_doc Documentation
 * \subsection sec_doxygen Compile the Doxygen documentation 
 * <PRE>
 * cd build/
 * make doxygen-doc</PRE>
 *
 * \subsection sec_examples Examples
 * Examples are located in:
 * <PRE>
 * examples/</PRE>
 *
 * \section sec_notes Notes, to-do list etc.
 *    - Current release
 *         -# Fully check CcCore and implement hashtable
 *    - Future work
 *         -# Optimize CcNSServer
 *         -# Make CcNSLanguage fully object-oriented
 */

#endif
