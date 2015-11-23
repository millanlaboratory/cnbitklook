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

#include <cnbicore/CcBasic.hpp>
#include <cnbicore/CcThread.hpp>
#include <cnbicore/CcDoubleBuffer.hpp>
#include <string.h>
#include <iostream>

	using namespace std;

	class Reader : public CcThread {
		public:
			Reader(CcDoubleBuffer* buffer) {
				this->_buffer = buffer;
		}
		virtual void Main(void) {
			int data;
			while(CcThread::IsRunning()) {
				this->_buffer->Read(&data);
				cout << "Reading: " << data << endl;
				CcTime::Sleep(this->sleep);

				if(data >= 10) 
					CcThread::Stop();
			}
		}
	public:
		double sleep;
	private:
		CcDoubleBuffer* _buffer;
};

class Writer : public CcThread {
	public:
		Writer(CcDoubleBuffer* buffer) {
			this->_buffer = buffer;
		}
		virtual void Main(void) {
			int data = 0;
			while(CcThread::IsRunning()) {
				++data;
				//cout << "Writing: " << data << endl;
				this->_buffer->Write(&data);
				CcTime::Sleep(this->sleep);
			}
		}
	public:
		double sleep;
	private:
		CcDoubleBuffer* _buffer;
};


int main(void) {
	CcDoubleBuffer buffer(sizeof(int));

	Writer w(&buffer);
	Reader r(&buffer);
	
	cout << "Writer slower then Reader" << endl;
	r.sleep = 000.01f;
	w.sleep = 000.05f;
	buffer.Prepare();
	w.Start();
	r.Start();
	r.Join();
	w.Stop();
	w.Join();
	
	cout << "Reader as fast as Writer" << endl;
	r.sleep = 000.01f;
	w.sleep = 000.01f;
	buffer.Prepare();
	w.Start();
	r.Start();
	r.Join();
	w.Stop();
	w.Join();
	
	cout << "Reader slower then Writer" << endl;
	r.sleep = 000.05f;
	w.sleep = 000.01f;
	buffer.Prepare();
	w.Start();
	r.Start();
	r.Join();
	w.Stop();
	w.Join();
	

	return 0;
}
