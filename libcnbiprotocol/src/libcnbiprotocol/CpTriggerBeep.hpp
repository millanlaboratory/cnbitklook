/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#ifndef CPTRIGGERBEEP_HPP
#define CPTRIGGERBEEP_HPP

#include "CpTrigger.hpp"

/* 2009-10-17  Michele Tavella <michele.tavella@epfl.ch>
 * Tricky piece of doggie code to debug the trigger on a LPT-free machine
 */
 /*! \brief Beeper trigger for debugging
 */
class CpTriggerBeep : public CpTrigger {
	public:
		CpTriggerBeep(void);
		virtual ~CpTriggerBeep(void);

		virtual void Open(void);
		virtual void Close(void);
		virtual void Send(unsigned char data = 0);

	private:
		bool _open;
};

#endif
