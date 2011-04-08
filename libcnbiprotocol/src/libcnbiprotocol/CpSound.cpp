/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPSOUND_CPP
#define CPSOUND_CPP

#include "CpSound.hpp"

CpSound::CpSound(void) {
	CcObject::SetName("CpSound");
	this->_data = NULL;
}

CpSound::~CpSound(void) {
}

void CpSound::Load(std::string filename) {
	this->_filename = filename;
	if(this->_data != NULL)
		this->Free();

	this->_data = Mix_LoadWAV(this->_filename.c_str());
	
	if(this->_data != NULL)
		return;

	printf("[CpSound::Load] Unable to load %s: %s\n", this->_filename.c_str(), Mix_GetError());
}

void CpSound::Free(void) {
	if(this->_data == NULL) 
		return;

	Mix_FreeChunk(this->_data);
	this->_data = NULL;
}

#endif
