/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPSOUND_HPP
#define CPSOUND_HPP

#include <libcnbicore/CcObject.hpp>
#include <SDL/SDL_mixer.h>

class CpSound : public CcObject {
	friend class CpPlayer;

	public:
		CpSound(void);
		virtual ~CpSound(void);
		void Load(std::string filename);
		void Free(void);

	public:
		std::string _filename;
		Mix_Chunk* _data;
};

#endif
