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
#include "XMLException.hpp"

XMLException::XMLException(std::string info, std::string caller) {
	this->_info = info;
	this->_caller = caller;
}

XMLException::~XMLException(void) {
}

std::string XMLException::Caller(void) const {
	return this->_caller;
}

std::string XMLException::Info(void) const {
	return this->_info;
}

std::ostream &operator<<(std::ostream& output, const XMLException& obj) {
	output << std::string("[") << obj.Caller() << std::string("] ") 
		<< obj.Info();
	return output;
}

std::ostream &operator<<(std::ostream& output, const XMLException* obj) {
	output << std::string("[") << obj->Caller() << std::string("] ") 
		<< obj->Info();
	return output;
}
	
bool XMLException::operator==(const XMLException& right) {
	return(this->_info.compare(right.Info()) == 0);
}

bool XMLException::operator!=(const XMLException& right) {
	return(this->_info.compare(right.Info()) != 0);
}
