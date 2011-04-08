/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */
#ifndef CPPROBABILITY_HPP 
#define CPPROBABILITY_HPP 

#include <libcnbicore/CcObject.hpp>
#include <libtobiic/ICMessage.hpp>
#include <libtobiic/ICClassifier.hpp>
#include <map>

class CpProbability : public CcObject {
	public:
		CpProbability(const unsigned int size);
		CpProbability(ICMessage* message, const std::string& cname);
		CpProbability(ICClassifier* classifier);
		virtual ~CpProbability(void);

		CpProbability* MapICClass(const std::string& kname, unsigned int index);
		CpProbability* Update(ICMessage* message);
		CpProbability* Update(ICClassifier* classifier);
		
		unsigned int Size(void);
		CpProbability* Set(unsigned int index, float value);
		CpProbability* Get(unsigned int index, float* value);
		float Get(unsigned int index);
		
		CpProbability* Zero(void);
		CpProbability* Priors(void);
		CpProbability* Hard(unsigned int index);

		float Min(unsigned int* index = 0);
		float Max(unsigned int* index = 0);
		void Dump(void);

	private:
	protected:

	public:
		float* data;
	private:
		unsigned int _size;
		bool _tobiic;
		std::string _icclassifier;
		std::map<unsigned int, std::string> _icmap;
	protected:
};

#endif
