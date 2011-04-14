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

#include <libcnbicore/CcBasic.hpp>
#include <stdio.h>

using namespace std;

int main(void) {
	CcCore::OpenLogger("ccprocess", 
			CcCore::TerminalColors, CcCore::LevelInfo);
	
	cout << CcPtable::Size() << endl;
	CcPtable::Add(1000);
	CcPtable::Add(1001);
	CcPtable::Add(1002);
	CcPtable::Add(1003);
	try { CcPtable::Add(1003); } catch(CcException e) { }
	
	CcPtable::Dump();

	cout << "Dead: " <<  CcPtable::IsDead(1000) << endl;
	cout << "Dead: " <<  CcPtable::IsDead(1001) << endl;
	cout << "Dead: " <<  CcPtable::IsDead(1002) << endl;
	cout << "Dead: " <<  CcPtable::IsDead(1003) << endl;
	try { CcPtable::IsDead(1); } catch(CcException e) { }

	cout << "Alive: " <<  CcPtable::IsAlive(1000) << endl;
	cout << "Alive: " <<  CcPtable::IsAlive(1001) << endl;
	cout << "Alive: " <<  CcPtable::IsAlive(1002) << endl;
	cout << "Alive: " <<  CcPtable::IsAlive(1003) << endl;
	try { CcPtable::IsAlive(1); } catch(CcException e) { }
	
	CcPtable::SetDead(1000);
	CcPtable::SetDead(1001);
	CcPtable::SetDead(1002);
	CcPtable::SetDead(1003);
	try { CcPtable::SetDead(1); } catch(CcException e) { }
	
	CcPtable::Dump();

	cout << "Alive: " <<  CcPtable::IsAlive(1000) << endl;
	cout << "Alive: " <<  CcPtable::IsAlive(1001) << endl;
	cout << "Alive: " <<  CcPtable::IsAlive(1002) << endl;
	cout << "Alive: " <<  CcPtable::IsAlive(1003) << endl;
	try { CcPtable::IsAlive(1); } catch(CcException e) { }

	CcPtable::Remove(1000);
	CcPtable::Remove(1001);
	CcPtable::Remove(1002);
	CcPtable::Remove(1003);
	try { CcPtable::Remove(1); } catch(CcException e) { }
	
	cout << "Has: " <<  CcPtable::Has(1000) << endl;
	cout << "Has: " <<  CcPtable::Has(1001) << endl;
	cout << "Has: " <<  CcPtable::Has(1002) << endl;
	cout << "Has: " <<  CcPtable::Has(1003) << endl;
	cout << "Has: " <<  CcPtable::Has(1) << endl;
	
	CcPtable::Dump();
	
	CcPtable::Release();
	return 0;
}
