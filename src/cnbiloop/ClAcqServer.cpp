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

#ifndef CLACQSERVER_CPP 
#define CLACQSERVER_CPP 

#include "ClAcqServer.hpp" 
#include "ClAcqLang.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <errno.h>

ClAcqServer::ClAcqServer(CaWriter* writer) {
	this->_writer = writer;
}

ClAcqServer::~ClAcqServer(void) {
}

void ClAcqServer::HandleRecvPeer(CcSocket* caller, CcAddress addr, 
		CcStreamer* stream) { 
	while(this->CommunicationCl((CcServer*)caller, addr, stream));
}

bool ClAcqServer::CreateLog(const std::string& logfile, 
		const std::string& xdffile, const std::string& logline) {

	if(logfile.empty())
		return false;

	std::ofstream fid;
	fid.open(logfile.c_str(), std::ios::out | std::ios::app);

	if(fid.is_open() == false)
		return false;

	fid << xdffile << " ";
	fid << logline;
	fid.close();

	return true;
}

bool ClAcqServer::UpdateLog(const std::string& logfile, const std::string& logline, bool space) {
	if(logfile.empty())
		return false;
	std::ofstream fid;
	fid.open(logfile.c_str(), std::ios::out | std::ios::app);

	if(fid.is_open() == false)
		return false;

	if(space == true)
		fid << " ";
	fid << logline;
	fid.close();

	return true;
}
		
bool ClAcqServer::CommunicationCl(CcServer* server, CcAddress address,
		CcStreamer* stream) {
	std::string txtlabel;
	std::string message;
	std::string xdffile, logfile, logline, xdffilename;

	if(stream->Extract(&message, ClAcqLang::Hdr, ClAcqLang::Trl) == false)
		return false;

	if(language.IsOpenXDF(message.c_str(), &xdffile, &logfile, &logline)) {
		if(this->_writer->IsOpen() == true) {
			server->Send(language.Error(ClAcqLang::XDFAlreadyOpen), address);
			CcLogWarningS("Open XDF from " << address << ": " 
					<< xdffile << " XDFAlreadyOpen");
		} else {
			xdffilename = xdffile;
			std::string cnbitkdata = CcCore::GetEnvCnbiTkData();
			if(cnbitkdata.empty() == false) {
				CcLogConfigS("CNBITK_DATA points to: " << cnbitkdata);
				xdffile = cnbitkdata + "/" + xdffile;
				logfile = cnbitkdata + "/" + logfile;
				mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;
				if(mkdir(cnbitkdata.c_str(), mode) != 0) {
					if(errno != EEXIST) {
						CcLogFatalS("Cannot create directory: " << strerror(errno));
					}
				} else {
					CcLogConfigS("Data directory ready: " << cnbitkdata);
				}
			}
			this->_log = logfile;
			this->_xdf = xdffile;

			if(this->_writer->Open(this->_xdf) == false) {
				server->Send(language.Error(ClAcqLang::XDFOpenFailed), address);
				CcLogErrorS("Open XDF from " << address << ": " 
						<< this->_xdf << " XDFOpenFailed");
			} else {
				if(this->_writer->Setup(NULL) == false) {
					server->Send(language.Error(ClAcqLang::XDFSetupFailed), address);
					CcLogErrorS("Open XDF from " << address << ": " 
							<< this->_xdf << " XDFSetupFailed");
				} else if(this->CreateLog(this->_log, xdffilename, logline) == false) {
					server->Send(language.Error(ClAcqLang::LogOpenFailed), address);
					CcLogErrorS("Open Log from " << address << ": " 
							<< this->_log << " LogOpenFailed");
				} else {
					server->Send(language.Ok(), address);
					CcLogInfoS("Open XDF from " << address << ": " << this->_xdf);
				}
			}
		}
	} else if(language.IsCloseXDF(message.c_str())) {
		if(this->_writer->IsOpen() == false) {
				server->Send(language.Error(ClAcqLang::XDFNotOpen), address);
				CcLogWarningS("Close XDF from " << address << ": " 
						<< this->_xdf << " XDFNotOpen");
		} else {
			if(this->_writer->Close() == false) {
				server->Send(language.Error(ClAcqLang::XDFCloseFailed), address);
				CcLogWarningS("Close XDF from " << address << ": " 
						<< this->_xdf << " XDFCloseFailed");
			} else {
				this->UpdateLog(this->_log, "\n");
				this->_log.clear();
				this->_xdf.clear();
				server->Send(language.Ok(), address);
				CcLogInfoS("Close XDF from " << address);
			}
		}
	} else if(language.IsUpdateLog(message.c_str(), &logline)) {
		if(this->_log.empty() == true || this->_writer->IsOpen() == false) {
			server->Send(language.Error(ClAcqLang::LogUpdateFailed), address);
			CcLogWarningS("Update Log from " << address << ": " 
					<< this->_log << " LogUpdateFailed");
		} else {
			server->Send(language.Ok(), address);
			this->UpdateLog(this->_log, logline, true);
			CcLogInfoS("Update Log from " << address << ": " << logline);
		}
	} else {
		server->Send(language.Error(ClAcqLang::NotUndestood), address);
		CcLogWarningS("Message from " << address << 
				" not understood: " << message); 
	}
	return true;
}

void ClAcqServer::Register(CcServer* server) {
	CB_CcSocket(server->iOnRelease, this, HandleRelease);
	CB_CcSocket(server->iOnAcceptPeer, this, HandleAcceptPeer);
	CB_CcSocket(server->iOnDropPeer, this, HandleDropPeer);
	CB_CcSocket(server->iOnRecvPeer, this, HandleRecvPeer);
}
#endif
