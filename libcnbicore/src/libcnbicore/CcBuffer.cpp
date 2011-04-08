#ifndef CCBUFFER_CPP 
#define CCBUFFER_CPP 

#include "CcBuffer.hpp" 
#include "CcBasic.hpp" 
#include <stdlib.h>
#include <string.h>

template <class T>
CcBuffer<T>::CcBuffer(size_t size) : CcObject("CcBuffer") {
	this->bsize = size;
	this->buffer = (T*)malloc(this->bsize);
	this->Clear();
}

template <class T>
CcBuffer<T>::~CcBuffer(void) {
	free(this->buffer);
}
		
template <class T>
void CcBuffer<T>::Clear(void) {
	memset(this->buffer, 0, this->bsize);
}
		
template <class T>
void CcBuffer<T>::Write(const T* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Write would overflow");
	
	if(size == 0)
		memcpy(this->buffer, buffer, this->bsize);
	else
		memcpy(this->buffer, buffer, size);
}

template <class T>
void CcBuffer<T>::Read(T* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Read would overflow");

	if(size == 0)
		memcpy(buffer, this->buffer, this->bsize);
	else
		memcpy(buffer, this->buffer, size);
}

template <class T>
void CcBuffer<T>::Write(const CcBuffer* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Write would overflow");
	
	if(size == 0)
		memcpy(this->buffer, buffer->buffer, this->bsize);
	else
		memcpy(this->buffer, buffer->buffer, size);
}

template <class T>
void CcBuffer<T>::Read(CcBuffer* buffer, size_t size) {
	if(size > this->bsize)
		CcThrow("Read would overflow");

	if(size == 0)
		memcpy(buffer->buffer, this->buffer, this->bsize);
	else
		memcpy(buffer->buffer, this->buffer, size);
}

#endif
