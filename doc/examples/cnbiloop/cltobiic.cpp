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

#include <cnbiloop/ClTobiIc.hpp>
#include <iostream>

using namespace std;

int main(void) {
	CcCore::OpenLogger("cltobiic");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	
	ICMessage m1, m2;
	ICSerializerRapid s1(&m1), s2(&m2);
	ClTobiIc i1(ClTobiIc::SetOnly), i2(ClTobiIc::GetOnly);
	
	ICClass k1("0x300", 0.60f);
	ICClass k2("0x301", 0.40f);
	ICClassifier c("i1_classifier", "Test iC classifier", 
			ICClassifier::ValueProb, 
			ICClassifier::LabelBiosig);
	c.classes.Add(&k1);
	c.classes.Add(&k2);
	m1.classifiers.Add(&c);
	
	int block = 0;
	ClLoop::Configure();
	if(ClLoop::Connect() == false) {
		CcLogFatal("Cannot connect to loop");
		goto shutdown;
	}
	

	if(i2.Attach("12000", "/i2") == false) {
		CcLogFatal("Cannot attach i2");
		goto shutdown;
	}
	
	if(i1.Attach("/i2") == false) {
		CcLogFatal("Cannot attach i1 to i2");
		goto shutdown;
	}

	while(true) {
		m1.SetBlockIdx(block++);
		if(i1.SetMessage(&s1) == ClTobiIc::Detached) {
			CcLogFatal("i2 detached");
			goto shutdown;
		}

		if(i2.WaitMessage(&s2) == ClTobiIc::Detached) {
			CcLogFatal("i2 detached");
			goto shutdown;
		}
		
		m2.Dump();
		CcTime::Sleep(1000.00f);

		if(CcCore::receivedSIGAny.Get()) 
			goto shutdown;

	}

	CcTime::Sleep(60000.00f);
	
	
shutdown:
	i2.Detach();
	CcCore::Exit(0);
}
