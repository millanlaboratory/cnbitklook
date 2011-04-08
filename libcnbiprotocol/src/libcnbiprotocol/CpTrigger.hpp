/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#ifndef CPTRIGGER_HPP
#define CPTRIGGER_HPP

#include <libcnbicore/CcObject.hpp>

typedef unsigned char CpTriggerByte;

/*! \brief Interface for trigger objects
 *
 * Provides a  generic interface for triggers (LPT, TCP and so on).
 */
class CpTrigger : public CcObject {
	public:
		CpTrigger(void);
		virtual ~CpTrigger(void);

		virtual void Open(void) = 0;
		virtual void Close(void) = 0;
		
		/* 2009-10-09  Michele Tavella <michele.tavella@epfl.ch>
		 * Not really sure about this.
		 * It holds for the LPT and the TTY triggers, not really sure for the
		 * others...
		 */
		virtual void Send(CpTriggerByte data = 0) = 0;
};

#endif
