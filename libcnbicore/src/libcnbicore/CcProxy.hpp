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

#ifndef CCPROXY_HPP
#define CCPROXY_HPP

/*! \brief Callback proxy for simple types
 *
 * Simple types do not need specialized proxy classes. 
 * Here you have one that works out of the box for simple callbacks.
 */
class CcProxy {
	public:
		virtual void HandleBool(bool message) { }
		virtual void HandleChar(char message) { }
		virtual void HandleDouble(double message) { }
		virtual void HandleFloat(float message) { }
		virtual void HandleInt(int message) { }
		virtual void HandleLong(long message) { }
		virtual void HandleUInt(unsigned int message) { }
};

#endif
