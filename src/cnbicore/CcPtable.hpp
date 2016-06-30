/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCPTABLE_HPP 
#define CCPTABLE_HPP 

#include "CcSemaphore.hpp"
#include "CcObject.hpp"
#include <map>
#include <sys/types.h>

/*! \brief Process table
 */
class CcPtable : public CcObject {
	public:
		static CcPtable* Instance(void);
		static void Release(void);
		static unsigned int Refcount(void);
		static void Add(pid_t);
		static void Replace(pid_t);
		static void Remove(pid_t);
		static bool Has(pid_t);
		static unsigned int Size(void);
		static void SetDead(pid_t);
		static bool IsDead(pid_t);
		static bool IsAlive(pid_t);
		static void Dump(void);
	private:
	protected:
		CcPtable(void);
		~CcPtable(void);
		static void Destroy(void);

	public:
	private:
		static CcPtable* _instance;
		static unsigned int _refCount;
	protected:
		static std::map<pid_t, bool> _table;
		static CcSemaphore _semlock;
};

#endif
