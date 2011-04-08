/*
 *  Copyright (C) 2009 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol.
 */

#ifndef CCTRIGGERFACTORY_HPP
#define CCTRIGGERFACTORY_HPP

#include "CpTrigger.hpp"
#include "CpTriggerLPT.hpp"
#include "CpTriggerBeep.hpp"

class CpTriggerFactory {
	public:
		static CpTrigger* Find(void);
};
#endif
