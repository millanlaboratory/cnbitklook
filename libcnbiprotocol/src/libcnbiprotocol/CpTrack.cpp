/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPTRACK_CPP 
#define CPTRACK_CPP 

#include "CpTrack.hpp" 

CpTrack::CpTrack(void) {
}

CpTrack::CpTrack(const std::string& track, char separator) {
	this->Split(track, separator);
}

CpTrack::~CpTrack(void) {
	this->_track.clear();
}
		
void CpTrack::Set(const std::string& track, char separator) {
	this->Split(track, separator);
}

std::string CpTrack::Get(unsigned int i) {
	if(i > this->_track.size())
		return std::string("");
	return this->_track[i];
}

unsigned int CpTrack::Size(void) {
	return this->_track.size();
}

void CpTrack::Split(const std::string& strack, char separator) {
    int startpos = 0;
    int endpos = 0;
	this->_track.clear();

    endpos = strack.find_first_of(separator, startpos);
	while(endpos != -1) {       
        this->_track.push_back(strack.substr(startpos, endpos - startpos));
        startpos = endpos + 1;
        endpos = strack.find_first_of(separator, startpos);
		if(endpos == -1)
            this->_track.push_back(strack.substr(startpos));
    }
}

#endif
