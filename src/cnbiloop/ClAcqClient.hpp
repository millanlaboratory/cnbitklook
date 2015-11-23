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

#ifndef CLACQCLIENT_HPP 
#define CLACQCLIENT_HPP 

#include "ClClient.hpp"
#include "ClAcqLang.hpp"

class ClAcqClient : public ClClient {
	public:
		bool Connect(const CcAddress address = "127.0.0.1:9000");
		int OpenXDF(const std::string& filegdf, 
				const std::string& filelog, const std::string& linelog);
		int CloseXDF(void);
		int UpdateLog(const std::string& linelog);

	private:
		ClAcqLang _language;
};

#endif
