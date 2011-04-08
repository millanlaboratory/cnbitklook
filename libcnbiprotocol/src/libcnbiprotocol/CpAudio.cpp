/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPAUDIO_CPP
#define CPAUDIO_CPP

#include "CpAudio.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <math.h>

/* Initialization */
CpAudio* CpAudio::_instance = NULL;
unsigned int CpAudio::_refCount = 0;

CpAudio::CpAudio(void) { 
	CcObject::SetName("CpAudio");
}

CpAudio::~CpAudio(void) { 
}

CpAudio* CpAudio::Open(int rate, int channels, int buffers) {
	if(CpAudio::_refCount > 0) {
		printf("[CpAudio::Open] Audio device already open\n");
		return CpAudio::Instance();
	}

	if(SDL_Init(SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return NULL;
	}

	if(Mix_OpenAudio(rate, AUDIO_S16, channels, buffers) != 0) {
		printf("Unable to open audio!\n");
		return NULL;
	}

	printf("[CpAudio::Open] Opening audio device\n");
	return CpAudio::Instance();
}

void CpAudio::Close(void) {
	this->Release();

	if(CpAudio::_refCount > 0) {
		printf("[CpAudio::Closed] Audio device still used\n");
		return;
	}

	printf("[CpAudio::Closed] Closing audio device\n");
	Mix_CloseAudio();
	SDL_Quit();
}

void CpAudio::SetVolume(float volume) {
	if(volume > 1.00f)
		volume = 1.00f;
	if(volume < 0.00f)
		volume = 0.00f;

	Mix_VolumeMusic((int)floor(MIX_MAX_VOLUME*volume));
}

float CpAudio::GetVolume(void) {
	int ivolume = Mix_VolumeMusic(-1);
	return (float)(ivolume/MIX_MAX_VOLUME);
}

CpAudio* CpAudio::Instance(void) {
	if(_instance == NULL)
		_instance = new CpAudio;
	++CpAudio::_refCount;
	return _instance;
}

void CpAudio::Destroy(void) {
	if(_instance != NULL) 
		delete _instance;
}

unsigned int CpAudio::Refcount(void) {
	return CpAudio::_refCount;
}

void CpAudio::Release(void) {
	if(--CpAudio::_refCount < 1) 
		CpAudio::Destroy();
}

#endif
