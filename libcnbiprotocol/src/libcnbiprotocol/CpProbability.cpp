/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */
#ifndef CPPROBABILITY_CPP 
#define CPPROBABILITY_CPP 

#include "CpProbability.hpp" 
#include <libcnbicore/CcException.hpp>
#include <stdio.h>
#include <string.h>

CpProbability::CpProbability(const unsigned int size) {
	CcObject::SetName("CpProbability[Standalone]");
	this->_tobiic = false;
	this->_size = size;
	this->data = new float[this->_size];
	this->Priors();
}

CpProbability::CpProbability(ICMessage* message, const std::string& cname) {
	CcObject::SetName("CpProbability[ICMessage]");
	this->_tobiic = true;
	ICClassifier* classifier = message->GetClassifier(cname);
	this->_size = classifier->classes.Size();
	this->data = new float[this->_size];
	this->Zero();
	this->_icclassifier.assign(cname);
}

CpProbability::CpProbability(ICClassifier* classifier) {
	CcObject::SetName("CpProbability[ICClassifier]");
	this->_tobiic = true;
	this->_size = classifier->classes.Size();
	this->data = new float[this->_size];
	this->Zero();
	this->_icclassifier.assign(classifier->GetName());
}

CpProbability::~CpProbability(void) {
	if(this->data == 0)
		return;
	delete [] this->data;
	this->data = 0;
}
		
CpProbability* CpProbability::MapICClass(const std::string& kname, 
		unsigned int index) {
	if(this->_tobiic == false)
		throw CcException("Not in libtobiic mode", __PRETTY_FUNCTION__);

	if(this->_icmap.size() == this->_size)
		throw CcException("All ICClass already mapped", __PRETTY_FUNCTION__);
	this->_icmap[index] = kname;

	return this;
}

CpProbability* CpProbability::Update(ICMessage* message) {
	if(this->_tobiic == false)
		throw CcException("Not in libtobiic mode", __PRETTY_FUNCTION__);
	if(this->_icmap.size() < this->_size)
		throw CcException("Not all ICClass are mapped", __PRETTY_FUNCTION__);

	this->Update(message->classifiers.Get(this->_icclassifier));
	return this;
}

CpProbability* CpProbability::Update(ICClassifier* classifier) {
	if(this->_tobiic == false)
		throw CcException("Not in libtobiic mode", __PRETTY_FUNCTION__);
	if(this->_icmap.size() < this->_size)
		throw CcException("Not all ICClass are mapped", __PRETTY_FUNCTION__);
	
	for(unsigned int i = 0; i < this->_size; i++)
		this->data[i] = classifier->classes.Get(this->_icmap[i])->GetValue();
	
	return this;
}
		
unsigned int CpProbability::Size(void) {
	return this->_size;
}

CpProbability* CpProbability::Set(unsigned int index, float value) {
	if(index > this->_size)
		throw CcException("Requested value of bounds", __PRETTY_FUNCTION__);
	this->data[index] = value;

	return this;
}

CpProbability* CpProbability::Get(unsigned int index, float* value) {
	if(index > this->_size)
		throw CcException("Requested value of bounds", __PRETTY_FUNCTION__);
	if(value == 0)
		throw CcException("value is NULL", __PRETTY_FUNCTION__);
	
	*value = this->data[index];
	return this;
}

float CpProbability::Get(unsigned int index) {
	if(index > this->_size)
		throw CcException("Requested value of bounds", __PRETTY_FUNCTION__);
	return this->data[index];
}

CpProbability* CpProbability::Zero(void) {
	memset(this->data, 0, sizeof(float) * this->_size);
}

CpProbability* CpProbability::Priors(void) {
	for(unsigned int i = 0; i < this->_size; i++) 
		this->data[i] = 1.00f/(float)this->_size;

	return this;
}
		
CpProbability* CpProbability::Hard(unsigned int index) {
	if(index > this->_size)
		throw CcException("Requested value of bounds", __PRETTY_FUNCTION__);

	this->Zero();
	this->data[index] = 1;
	
	return this;
}

float CpProbability::Min(unsigned int* index) {
	unsigned int idx = 0;
	float min = this->data[idx];

	for(unsigned int i = 1; i < this->_size; i++) {
		if(this->data[i] < min) {
			idx = i;
			min = this->data[i];
		}
	}
	if(index != 0)
		*index = idx;

	return min;
}

float CpProbability::Max(unsigned int* index) {
	unsigned int idx = 0;
	float max = this->data[idx];

	for(unsigned int i = 1; i < this->_size; i++) {
		if(this->data[i] > max) {
			idx = i;
			max = this->data[i];
		}
	}
	if(index != 0)
		*index = idx;

	return max;
}

void CpProbability::Dump(void) {
	printf("[CpProbability::Dump] ");
	if(this->_tobiic) {
		printf("ICClassifier=\"%s\": ", 
				this->_icclassifier.c_str());
	}
	for(unsigned int i = 0; i < this->_size; i++) {
		printf("%f", this->data[i]);
		printf("%c", i == this->_size -1 ? '\n' : ' ');
	}
}

#endif
