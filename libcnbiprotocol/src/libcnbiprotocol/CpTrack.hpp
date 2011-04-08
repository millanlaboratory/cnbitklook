/*
 *  Copyright (C) 2010 Michele Tavella <michele.tavella@epfl.ch>
 *
 *  This file is part of libcnbiprotocol
 */

#ifndef CPTRACK_HPP 
#define CPTRACK_HPP 
#include <string>
#include <vector>

class CpTrack {
	public:
		CpTrack(void);
		CpTrack(const std::string& track, char separator = ' ');
		virtual ~CpTrack(void);
		virtual void Set(const std::string& track, char separator = ' ');
		virtual std::string Get(unsigned int i);
		virtual unsigned int Size(void);
	private:
		void Split(const std::string& strack, char separator);
	protected:

	public:
	private:
		std::vector<std::string> _track;
	protected:
};

#endif
