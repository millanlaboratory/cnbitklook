/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbicore library

    The libndf library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCLO_CPP
#define CCLO_CPP

#include "CcCore.hpp"
#include "CcLogger.hpp"

#define CcLog(level, message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(level, message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogConfig(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelConfig,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogException(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelException,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogDebug(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelDebug,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogInfo(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelInfo,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogWarning(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelWarning,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogError(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelError,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogFatal(message)\
	{\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelFatal,\
				message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	}

#define CcLogS(level, message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(level, (char*)message,\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogConfigS(message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelConfig,\
				message.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogExceptionS(message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelException,\
				message.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogDebugS(message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelDebug,\
				message.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogInfoS(message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelInfo,\
				message.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogWarningS(message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelWarning,\
				message.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogErrorS(message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelError,\
				message.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogFatalS(message, ...)\
	{\
	message << __VA_ARGS__;\
	CcCore::logger.AddEntry(CcLogEntry(CcCore::LevelFatal,\
				message.str(),\
				__PRETTY_FUNCTION__, __FILE__, __LINE__));\
	message.str("");\
	}

#define CcLogConfigSH(message, header, ...)\
	CcLogConfigS(message, "[" << header << "] " << __VA_ARGS__);

#define CcLogExceptionSH(message, header, ...)\
	CcLogExceptionS(message, "[" << header << "] " << __VA_ARGS__);

#define CcLogDebugSH(message, header, ...)\
	CcLogDebugS(message, "[" << header << "] " << __VA_ARGS__);

#define CcLogInfoSH(message, header, ...)\
	CcLogInfoS(message, "[" << header << "] " << __VA_ARGS__);

#define CcLogWarningSH(message, header, ...)\
	CcLogWarningS(message, "[" << header << "] " << __VA_ARGS__);

#define CcLogErrorSH(message, header, ...)\
	CcLogErrorS(message, "[" << header << "] " << __VA_ARGS__);

#define CcLogFatalSH(message, header, ...)\
	CcLogFatalS(message, "[" << header << "] " << __VA_ARGS__);

#endif
