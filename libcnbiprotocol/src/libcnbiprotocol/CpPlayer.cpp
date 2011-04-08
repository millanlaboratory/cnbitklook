/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPPLAYER_CPP
#define CPPLAYER_CPP

#include "CpPlayer.hpp"
#include <SDL/SDL_mixer.h>
#include <libcnbicore/CcTime.hpp>

CpPlayer::CpPlayer(void) { 
	this->_playing = -1;
}

CpPlayer::~CpPlayer(void) { 
}

void CpPlayer::Play(CpSound* sound, int ch, int loops) {
	this->_playing = Mix_PlayChannel(ch, sound->_data, loops);
	if(this->_playing != -1)
		return;
	printf("[CpPlayer::Play] Unable to play WAV file: %s\n",
			Mix_GetError());
}

bool CpPlayer::IsPlaying(void) {
	return(Mix_Playing(this->_playing) != 0);
}

void CpPlayer::Wait(int ms) {
	while(Mix_Playing(this->_playing) != 0)
		CcTime::Sleep(ms);
	this->_playing = -1;
}

#endif
