/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPPLAYER_HPP
#define CPPLAYER_HPP

#include "CpSound.hpp"
#include <libcnbicore/CcObject.hpp>

class CpPlayer : public CcObject {
	public:
		CpPlayer(void); 
		virtual ~CpPlayer(void);
		
		void Play(CpSound* sound, int ch = -1, int loops = 0);
		bool IsPlaying(void);
		void Wait(int ms = 5);
	
	private:
		int _playing;
		CpSound* _sound;
};
#endif
