#ifndef CCCLIENTEXT_CPP 
#define CCCLIENTEXT_CPP 

#include "CcClientExt.hpp" 
#include <cnbicore/CcTime.hpp>

CcClientExt::CcClientExt(size_t bsize) : CcClient(bsize) {
}

CcClientExt::~CcClientExt(void) {
}
		
bool CcClientExt::SendRecv(const char* query, std::string *reply, 
		std::string hdr, std::string trl, float waitms) {

	int bytes = CcClient::Send(query);
	if(bytes == TR_BYTES_ERROR || bytes == TR_BYTES_NONE)
		return false;
	
	CcStreamer* stream;
	CcTimeValue tic;
	CcTime::Tic(&tic);
	while(true) {
		CcSocket::_semsocket.Wait();
		stream = CcSocket::GetStream(CcSocket::_socket->fd);
		if(stream->Extract(reply, hdr, trl, CcStreamer::Reverse)) {
			CcSocket::_semsocket.Post();
			return true;
		}
		CcSocket::_semsocket.Post();

		if(CcTime::Toc(&tic) > waitms)
			return false;
		CcTime::Sleep(CCASYNC_WAIT_RECV);
	}
	return false;
}

bool CcClientExt::SendRecv(const std::string& query, std::string *reply, 
		std::string hdr, std::string trl, float waitms) {
	return this->SendRecv(query.c_str(), reply, hdr, trl, waitms);
}

#endif
