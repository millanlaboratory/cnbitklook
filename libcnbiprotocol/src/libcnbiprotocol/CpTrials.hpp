/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPTRIALS_HPP
#define CPTRIALS_HPP

#include <map>
#include <vector>

typedef unsigned int CpClassLabel;

class CpTrials {
	public:
		CpTrials(void);
		virtual ~CpTrials(void);

		bool AddClassLabel(CpClassLabel klabel, unsigned int occurrences);
		bool IncreaseClassLabel(CpClassLabel klabel, unsigned int occurrences);
		bool Create(void);
		bool Shuffle(void);
		void Dump(void);
		CpClassLabel At(unsigned int pos);
		unsigned int Total(void);

	public:
		static const unsigned int None = 666;
	private:
		std::vector<CpClassLabel> _trials;
		std::map<CpClassLabel, unsigned int> _koccurr;
};
#endif
