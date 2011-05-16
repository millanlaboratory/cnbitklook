#ifndef CCCLIENTEXT_CPP 
#define CCCLIENTEXT_CPP 

#include "CcClientExt.hpp" 

CcClientExt::CcClientExt(size_t bsize) : CcClient(bsize) {
}

CcClientExt::~CcClientExt(void) {
}
		
bool SendRecv(const char* query, std::string *reply, 
		std::string hdr, std::string trl, float waitms = -1) {

	return false;
}

bool SendRecv(const std::string& query, std::string *reply, 
		std::string hdr, std::string trl, float waitms = -1) {
	
	return false;
}

#endif
