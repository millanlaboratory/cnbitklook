/*
    Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>

    This file is part of the libcnbiloop library

    The libcnbiloop library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

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

ClAcqAsServer::ClAcqAsServer(ClWriter* writer, ndf_frame* frame,
		CcSemaphore* semframe) {
	this->_writer = writer;
	this->_frame = frame;
	this->_semframe = semframe;
	this->_serializerD = new IDSerializerRapid(&this->_messageD);
}

ClAcqAsServer::~ClAcqAsServer(void) {
	if(this->_serializerD == NULL)
		return;
	delete this->_serializerD;
	this->_serializerD = NULL;
}

void ClAcqAsServer::HandleBind(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogConfigS(this->_stream, "Bound TCP socket: " << server->GetLocal());
}

void ClAcqAsServer::HandleRelease(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogConfigS(this->_stream, "Released TCP socket: " << server->GetLocal());
}

void ClAcqAsServer::HandleListen(CcSocket* caller) { 
	CcServerMulti *server = (CcServerMulti*)caller;
	CcLogConfigS(this->_stream, 
			"Listening on TCP socket: " << server->GetLocal());
}

void ClAcqAsServer::HandleAcceptEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogConfigS(this->_stream, "Accepted TCP endpoint: " << address);
}

void ClAcqAsServer::HandleDropEndpoint(CcSocket* caller, CcAddress address) { 
	CcLogConfigS(this->_stream, "Dropped TCP endpoint: " << address);
}

void ClAcqAsServer::HandleRecvEndpoint(CcSocket* caller,
		CcAddress address) { 
	CcServerMulti* server = (CcServerMulti*)caller;

	while(this->CommunicationTiD(server, address));
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
	GDFEvent gdflabel;
	HWTrigger hwtlabel;
	std::string txtlabel;
	std::string message;
	std::string xdffile, logfile, logline;

	if(server->datastreams[address]->Extract(&message, 
				ClAcqLang::Hdr, ClAcqLang::Trl, CcStreamer::Forward) == false)
		return false;

	if(language.IsAddLabelGDF(message.c_str(), &gdflabel)) {
		this->_semframe->Wait();
		if(ndf_add_label(this->_frame, &gdflabel) == NULL) {
			CcLogWarningS(this->_stream, "Add label GDF from " << address << 
					": " << gdflabel << " NDFLimitReached"); 
			server->Send(language.Error(ClAcqLang::NDFLimitReached), address);
		} else {
			CcLogInfoS(this->_stream, "GDF label received from " << address << 
					": " << gdflabel <<
					" (" << this->_writer->TocOpen()/1000 << "s)");
			server->Send(language.Ok(), address);
		}
		this->_semframe->Post();
	} else if(language.IsAddLabelTXT(message.c_str(), &txtlabel)) {
		server->Send(language.Error(ClAcqLang::NotSupported), address);
	} else if(language.IsAddLabelLPT(message.c_str(), &hwtlabel)) {
		server->Send(language.Error(ClAcqLang::NotSupported), address);
	} else if(language.IsOpenXDF(message.c_str(), &xdffile, &logfile, &logline)) {
		if(this->_writer->IsOpen() == true) {
			server->Send(language.Error(ClAcqLang::XDFAlreadyOpen), address);
			CcLogWarningS(this->_stream, "Open XDF from " << address << ": " 
					<< xdffile << " XDFAlreadyOpen");
		} else {
			if(this->_writer->Open(xdffile) == false) {
				server->Send(language.Error(ClAcqLang::XDFOpenFailed), address);
				CcLogErrorS(this->_stream, "Open XDF from " << address << ": " 
						<< xdffile << " XDFOpenFailed");
			} else {
				if(this->_writer->Setup(NULL) == false) {
					server->Send(language.Error(ClAcqLang::XDFSetupFailed), address);
					CcLogErrorS(this->_stream, "Open XDF from " << address << ": " 
							<< xdffile << " XDFSetupFailed");
				} else if(this->LogXDF(logfile, xdffile, logline) == false) {
					server->Send(language.Error(ClAcqLang::LogOpenFailed), address);
					CcLogErrorS(this->_stream, "Open Log from " << address << ": " 
							<< xdffile << " LogOpenFailed");
				} else {
					server->Send(language.Ok(), address);
					CcLogInfoS(this->_stream, "Open XDF from " << address << ": " 
							<< xdffile);
				}
			}
		}
	} else if(language.IsCloseXDF(message.c_str())) {
		if(this->_writer->IsOpen() == false) {
				server->Send(language.Error(ClAcqLang::XDFNotOpen), address);
				CcLogWarningS(this->_stream, "Close XDF from " << address << ": " 
						<< xdffile << " XDFNotOpen");
		} else {
			if(this->_writer->Close() == false) {
				server->Send(language.Error(ClAcqLang::XDFCloseFailed), address);
				CcLogWarningS(this->_stream, "Close XDF from " << address << ": " 
						<< xdffile << " XDFCloseFailed");
			} else {
				server->Send(language.Ok(), address);
				CcLogInfoS(this->_stream, "Close XDF from " << address);
			}
		}
	} else {
		server->Send(language.Error(ClAcqLang::NotUndestood), address);
		CcLogWarningS(this->_stream, "Message from " << address << 
				" not understood: " << message); 
	}
	return true;
}

bool ClAcqAsServer::CommunicationTiD(CcServerMulti* server, CcAddress address) {
	IDevent idevent;
	IDFtype idfamily;
	std::string message;

	if(server->datastreams[address]->Extract(&message, 
			"<tobiid", "/>", CcStreamer::Forward) == false)
		return false;

	this->_serializerD->Deserialize(&message);
	idevent = this->_messageD.GetEvent();
	idfamily = this->_messageD.GetFamilyType();

	// Verify family type for compatibility
	if(idfamily != IDMessage::FamilyBiosig) {
		CcLogWarning("TiD family type not supported");
		return true;
	}
	
	if(this->_messageD.absolute.IsSet()) {
		// Add label to ClWriter
		this->_semframe->Wait();
		if(ndf_add_label(this->_frame, &idevent) == NULL)
			CcLogError("TiD event is valid but cannot be added to NDF frame");
		// TODO add event to file
		this->_semframe->Post();

		/* Perform all the necessary operations on the timestamps
		 * and distribute to all connected clients but the sender
		 */
		this->_writer->Tic(&this->_messageD);
		this->_serializerD->Serialize(&message);
		server->SendNot(message.c_str(), address);
		CcLogInfoS(this->_stream, "TiD event received from " << address << 
					" and distributed: " << idevent <<
					" (" << this->_writer->TocOpen()/1000 << "s)");
	} else {
		CcLogError("TiD event is not valid and will not be distributed");
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
