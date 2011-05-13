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

#ifndef CLACQASSERVER_CPP 
#define CLACQASSERVER_CPP 

#include "ClAcqAsServer.hpp" 
#include "ClAcqLang.hpp"
#include <iostream>
#include <fstream>

ClAcqAsServer::ClAcqAsServer(CaWriter* writer) {
	this->_writer = writer;
}

ClAcqAsServer::~ClAcqAsServer(void) {
}

void ClAcqAsServer::HandleBind(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS("Bound TCP socket: " << server->GetLocal());
}

void ClAcqAsServer::HandleRelease(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS("Released TCP socket: " << server->GetLocal());
}

void ClAcqAsServer::HandleListen(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogDebugS(
			"Listening on TCP socket: " << server->GetLocal());
}

void ClAcqAsServer::HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS("Accepted TCP endpoint: " << address);
}

void ClAcqAsServer::HandleDropEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogDebugS("Dropped TCP endpoint: " << address);
}

void ClAcqAsServer::HandleRecvEndpoint(CcSocket* caller, CcAddress address) { 
	CcServerMulti* server = (CcServerMulti*)caller;

	while(this->CommunicationCl(server, address));
}

bool ClAcqAsServer::LogXDF(const std::string& logfile, 
		const std::string& xdffile, const std::string& logline) {

	if(logfile.empty())
		return false;

	std::ofstream fid;
	fid.open(logfile.c_str(), std::ios::out | std::ios::app);

	if(fid.is_open() == false)
		return false;

	fid << xdffile << " ";
	fid << logline << std::endl;
	fid.close();

	return true;
}
		
bool ClAcqAsServer::CommunicationCl(CcServerMulti* server, CcAddress address) {
	std::string txtlabel;
	std::string message;
	std::string xdffile, logfile, logline;

	if(server->datastreams[address]->Extract(&message, 
				ClAcqLang::Hdr, ClAcqLang::Trl, CcStreamer::Forward) == false)
		return false;

	if(language.IsOpenXDF(message.c_str(), &xdffile, &logfile, &logline)) {
		if(this->_writer->IsOpen() == true) {
			server->Send(language.Error(ClAcqLang::XDFAlreadyOpen), address);
			CcLogWarningS("Open XDF from " << address << ": " 
					<< xdffile << " XDFAlreadyOpen");
		} else {
			if(this->_writer->Open(xdffile) == false) {
				server->Send(language.Error(ClAcqLang::XDFOpenFailed), address);
				CcLogErrorS("Open XDF from " << address << ": " 
						<< xdffile << " XDFOpenFailed");
			} else {
				if(this->_writer->Setup(NULL) == false) {
					server->Send(language.Error(ClAcqLang::XDFSetupFailed), address);
					CcLogErrorS("Open XDF from " << address << ": " 
							<< xdffile << " XDFSetupFailed");
				} else if(this->LogXDF(logfile, xdffile, logline) == false) {
					server->Send(language.Error(ClAcqLang::LogOpenFailed), address);
					CcLogErrorS("Open Log from " << address << ": " 
							<< xdffile << " LogOpenFailed");
				} else {
					server->Send(language.Ok(), address);
					CcLogInfoS("Open XDF from " << address << ": " << xdffile);
				}
			}
		}
	} else if(language.IsCloseXDF(message.c_str())) {
		if(this->_writer->IsOpen() == false) {
				server->Send(language.Error(ClAcqLang::XDFNotOpen), address);
				CcLogWarningS("Close XDF from " << address << ": " 
						<< xdffile << " XDFNotOpen");
		} else {
			if(this->_writer->Close() == false) {
				server->Send(language.Error(ClAcqLang::XDFCloseFailed), address);
				CcLogWarningS("Close XDF from " << address << ": " 
						<< xdffile << " XDFCloseFailed");
			} else {
				server->Send(language.Ok(), address);
				CcLogInfoS("Close XDF from " << address);
			}
		}
	} else {
		server->Send(language.Error(ClAcqLang::NotUndestood), address);
		CcLogWarningS("Message from " << address << 
				" not understood: " << message); 
	}
	return true;
}

void ClAcqAsServer::Register(CcServerMulti* server) {
	CB_CcSocket(server->iOnBind, this, HandleBind);
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnListen, this, HandleListen);
	CB_CcSocket(server->iOnAcceptEndpoint, this, HandleAcceptEndpoint);
	CB_CcSocket(server->iOnDropEndpoint, this, HandleDropEndpoint);
	CB_CcSocket(server->iOnRecvEndpoint, this, HandleRecvEndpoint);
}
#endif
