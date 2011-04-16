/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiconfig library

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

#include "CCfgTask.hpp"
#include <stdio.h>

CCfgTask::CCfgTask(void) {
	this->name.assign("undefined"); 	   
	this->description.assign("undefined");
	this->id = 0;
	this->trials = 0;     
	this->gdf = 0;   
	this->hwt = 0; 
}

void CCfgTask::Dump(void) {
	printf("[CCfgTask::Dump] Task:\n");
	printf("  ID:          %d\n", this->id);
	printf("  Name:        %s\n", this->name.c_str());
	printf("  Description: %s\n", this->description.c_str());
	printf("  Trials:      %d\n", this->trials);
	printf("  GDF:         %d\n", this->gdf);
	printf("  LPT:         %d\n", (int)(this->hwt));
	
	CCfgTaskConfigIt it = this->config.begin();
	printf("  XMLTypes:  ");
	if(it == this->config.end()) {
		printf("none\n");
		return;
	}
	while(true) {
		printf("%s",  it++->first.c_str());
		if(it == this->config.end()) {
			printf("\n");
			break;
		}
		printf(", ");
	}
}

CCfgTaskConfigIt CCfgTask::Begin(void) {
	CCfgTaskConfigIt it = this->config.begin();
	return it;
}

CCfgTaskConfigIt CCfgTask::End(void) {
	CCfgTaskConfigIt it = this->config.end();
	return it;
}

CCfgTaskConfigConstIt CCfgTask::Begin(void) const {
	CCfgTaskConfigConstIt it = this->config.begin();
	return it;
}

CCfgTaskConfigConstIt CCfgTask::End(void) const {
	CCfgTaskConfigConstIt it = this->config.end();
	return it;
}
		
bool CCfgTask::HasConfig(const std::string& name) {
	CCfgTaskConfigConstIt it = this->config.find(name);
	return(it != CCfgTask::End());
}
