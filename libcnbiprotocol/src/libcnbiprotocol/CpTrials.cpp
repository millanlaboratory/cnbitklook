/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPTRIALS_CPP
#define CPTRIALS_CPP

#include "CpRandInt.hpp"
#include "CpTrials.hpp"
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

CpTrials::CpTrials(void) {
	this->_trials.clear();
	this->_koccurr.clear();
}

CpTrials::~CpTrials(void) {
}

bool CpTrials::AddClassLabel(CpClassLabel klabel, unsigned int occurrences) {
	/* The number of the devil is reserved */
	if(klabel == CpTrials::None)
		return false;

	/* Already added klabel */
	std::map<CpClassLabel, unsigned int>::iterator it;
	it = this->_koccurr.find(klabel);
	if(it != this->_koccurr.end()) 
		return false;

	/* Add klabel/occurrences */
	this->_koccurr.insert(std::make_pair(klabel, occurrences));
	return true;
}

bool CpTrials::IncreaseClassLabel(CpClassLabel klabel, unsigned int occurrences) {
	/* The number of the devil is reserved */
	if(klabel == CpTrials::None)
		return false;

	/* Already added klabel */
	std::map<CpClassLabel, unsigned int>::iterator it;
	it = this->_koccurr.find(klabel);
	if(it == this->_koccurr.end()) 
		return false;

	/* Add klabel/occurrences */
	it->second += occurrences;
	return true;
}

bool CpTrials::Create(void) {
	this->_trials.clear();

	std::map<CpClassLabel, unsigned int>::iterator itm;
	std::vector<CpClassLabel>::iterator itv;
	itm = this->_koccurr.begin();
	
	if(itm == this->_koccurr.end())
		return false;

	do {
		itv = this->_trials.end();
		this->_trials.insert(itv, itm->second, itm->first);
	} while(++itm != this->_koccurr.end());

	return true;
}

bool CpTrials::Shuffle(void) {
	if(this->_trials.empty())
		return false;

	CpRandInt seed;
	std::random_shuffle(this->_trials.begin(), this->_trials.end(), seed);

	return true;
}

void CpTrials::Dump(void) {
	printf("[CpTrials::Dump] Trial vector:\n");
	std::ostream_iterator<CpClassLabel> output(std::cout, " " );
	std::copy(this->_trials.begin(), this->_trials.end(), output);
	std::cout << std::endl;
}

CpClassLabel CpTrials::At(unsigned int pos) {
	if(pos >= this->_trials.size())
		return CpTrials::None; // TODO excpetion

	return this->_trials[pos];
}

unsigned int CpTrials::Total(void) { 
	return this->_trials.size();
}
#endif
