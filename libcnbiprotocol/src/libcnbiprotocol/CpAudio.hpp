/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPAUDIO_HPP
#define CPAUDIO_HPP

#include <libcnbicore/CcObject.hpp>

/*! \brief Audio subsistem
 */
class CpAudio : public CcObject {
	protected:
		CpAudio(void);
		~CpAudio(void);
	public:
		static CpAudio* Open(int rate = 44100, int channels = 2, 
				int buffers = 512);
		void Close(void);
		void SetVolume(float volume);
		float GetVolume(void);
	private:
		static CpAudio* Instance(void);
		void Destroy(void);
		static unsigned int Refcount(void);
		void Release(void);

	private:
		static CpAudio* _instance;
		static unsigned int _refCount;
};


#endif
