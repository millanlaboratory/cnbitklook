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

#ifndef CCCALLBACK_HPP
#define CCCALLBACK_HPP

#include "CcException.hpp"

/*! \brief Callback template class
 *
 * Implements a callback.
 */
template <class Sender>
class CcCallback0 {
	public:
		CcCallback0(void) {
			this->_method = NULL;
			this->_instance = NULL;
		}
		
		typedef void (Sender::*Method)(void);
		
		virtual void Execute(void) const {
			if(IsRegistered() == false)
				return;  
			(this->_instance->*_method)();
		}

		void Register(Sender *instance, Method function) {
			this->_method = function;
			this->_instance = instance;
		}

		bool IsRegistered(void) const {
			return (this->_instance != NULL);
		}

	private:
		Sender *_instance;
		Method _method;
};

/*! \brief Callback template class (1 argument)
 *
 * Implements a callback with a single argument.
 */
template <class Sender, typename Arg>
class CcCallback1 {
	public:
		CcCallback1(void) {
			this->_method = NULL;
			this->_instance = NULL;
		}
		
		typedef void (Sender::*Method)(Arg argument);
		
		virtual void Execute(Arg argument) const {
			if(IsRegistered() == false)
				return;  
			(this->_instance->*_method)(argument);
		}

		void Register(Sender *instance, Method function) {
			this->_method = function;
			this->_instance = instance;
		}

		bool IsRegistered(void) const {
			return (this->_instance != NULL);
		}

	private:
		Sender *_instance;
		Method _method;
};

/*! \brief Callback template class (2 arguments)
 *
 * Implements a callback with two arguments.
 */
template <class Sender, typename Arg1, typename Arg2>
class CcCallback2 {
	public:
		CcCallback2(void) {
			this->_method = NULL;
			this->_instance = NULL; 
		}

		typedef void (Sender::*Method)(Arg1 argument, Arg2 argument2);
		
		virtual void Execute(Arg1 argument1, Arg2 argument2) const {
			if(IsRegistered() == false)
				return;  
			(this->_instance->*_method)(argument1, argument2);
		}

		void Register(Sender *instance, Method function) {
			this->_method = function;
			this->_instance = instance;
		}

		bool IsRegistered(void) const {
			return (this->_instance != NULL);
		}

	private:
		Sender *_instance;
		Method _method;
};

/*! \brief Callback template class (3 arguments)
 *
 * Implements a callback with three arguments.
 */
template <class Sender, typename Arg1, typename Arg2,
		 typename Arg3>
class CcCallback3 {
	public:
		CcCallback3(void) {
			this->_method = NULL;
			this->_instance = NULL; 
		}

		typedef void (Sender::*Method)(Arg1 argument, Arg2 argument2,
				Arg3 argument3);
		
		virtual void Execute(Arg1 argument1, Arg2 argument2, Arg3
				argument3) const {
			if(IsRegistered() == false)
				return;  
			(this->_instance->*_method)(argument1, argument2, argument3);
		}

		void Register(Sender *instance, Method function) {
			this->_method = function;
			this->_instance = instance;
		}

		bool IsRegistered(void) const {
			return (this->_instance != NULL);
		}

	private:
		Sender *_instance;
		Method _method;
};


#define CBRegister(sender_event, receiver_type, receiver_ptr) 	\
	sender_event.Register((receiver_type*)receiver_ptr, 		\
			&receiver_type::Handle);

#endif
