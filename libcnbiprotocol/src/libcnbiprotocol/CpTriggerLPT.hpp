/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#ifndef CPTRIGGERLPT_HPP
#define CPTRIGGERLPT_HPP

#include "CpTrigger.hpp"
#include <lpttrigger.h>

/*! \brief LPT trigger via liblpttrigger
 */
class CpTriggerLPT : public CpTrigger {
	public:
		/* 2009-10-12  Michele Tavella <michele.tavella@epfl.ch>
		 * Bubs says 50ms for windows
		 */
		CpTriggerLPT(int port = -1, unsigned int dt = 15, 
				unsigned char level =  0);
		virtual ~CpTriggerLPT(void);

		virtual void Open(void);
		virtual void Close(void);
		virtual void Send(CpTriggerByte data = 0);

	private:
		int _port;
		unsigned int _dt;
		unsigned char _level;
		struct lpttrigger* _trigger;
		bool _open;
};

#endif
