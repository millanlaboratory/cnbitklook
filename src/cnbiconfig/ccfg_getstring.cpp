/*
    Copyright (C) 2009-2016  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>
    Serafeim Perdikis <serafeim.perdikis@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CCfgXMLConfig.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void usage(void) { 
	printf("Usage: ccfg_getstring [OPTION]...\n\n");
	printf("  -x       path to XML file\n");
	printf("  -r       rootname\n");
	printf("  -p       nodepath to parameter\n");
	printf("  -h       display this help and exit\n");
}

int main(int argc, char *argv[]) {
	int opt;
	std::string val="", xmlfilepath, parampath, rootname = "cnbiconfig";
	while((opt = getopt(argc, argv, "x:r:p:")) != -1) {
		if(opt == 'x')
			xmlfilepath.assign(optarg);
		else if(opt == 'r')
			rootname.assign(optarg);
		else if(opt == 'p')
			parampath.assign(optarg);
		else {
			usage();
			return(opt == 'h') ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}
		
	CCfgXMLConfig* config;
	try {
		config = new CCfgXMLConfig(rootname);
		config->ImportFileEx(xmlfilepath);
	} catch(XMLException e) {
		std::cerr << "[ccfg_getstring] Exception: " << e << std::endl;
		return -1;
	}

	try {
		val = config->RootEx()->QuickStringEx(parampath);
	} catch(XMLException e){
		val="";
		std::cerr << "[ccfg_getstring] Exception: " << e << std::endl;
		std::cerr << "[ccfg_getstring] Parameter path is probably wrong" << std::endl;
		return -2;
	}
	std::cout<<val<<std::endl;
	return 0;
}
