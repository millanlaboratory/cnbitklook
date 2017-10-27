#include <cnbiloop/ClLoop.hpp>
#include <cnbiloop/ClTobiIc.hpp>
#include <getopt.h>
#include <cmath>
#include <string>

#define CL_TICSENDER_CLASS_LABEL 	"0x0580"

void usage(void) { 
	printf("Usage: cl_ticsender [OPTION]...\n\n");
	printf(" Sends a sinewave via TobiIc\n\n");
	printf("  -a       address of nameserver in ip:port format\n");
	printf("  -p       TCP port (9500 default)\n");
	printf("  -n       TCP client name (/ctrl1 default)\n");
	printf("  -r       Sending rate (16 Hz default)\n");
	printf("  -f       Sine frequency rate (0.2 Hz default)\n");
	printf("  -m       Sine maximum amplitude value (0.7 default)\n");
	printf("  -d       Dump message sent\n");
	printf("  -h       display this help and exit\n");
}

float sinewave(float a, float dt, float f) {
	return a*sin((2.0f*M_PI*dt*f));
}

int main(int argc, char* argv[]) {
	int opt;
	std::string optname("/ctrl1");
	std::string optmax("0.7");
	std::string optrate("16.0");
	std::string optfreq("0.2");
	CcPort optport("");
	CcAddress nameserver;
	bool dump = false;
	
	while((opt = getopt(argc, argv, "a:p:c:l:n:m:f:hd")) != -1) {
		if(opt == 'p')
			optport.assign(optarg);
		else if(opt == 'n')
			optname.assign(optarg);
		else if(opt == 'm')
			optmax.assign(optarg);
		else if(opt == 'r')
			optrate.assign(optarg);
		else if(opt == 'f')
			optfreq.assign(optarg);
		else if(opt == 'd')
			dump = true;
		else if(opt == 'a')
			nameserver.assign(optarg);
		else {
			usage();
			CcCore::Exit(opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE);
		}
	}

	CcCore::OpenLogger("cl_ticsender");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	ClLoop::Configure(nameserver);

	// Initialization IC Message
	ICMessage* 			icm;
	ICSerializerRapid* 	ics;
	ICClass* 			icclass;
	ICClassifier* 		icclassifier;
	std::string 		classlbl(CL_TICSENDER_CLASS_LABEL);

	// Initialization ClTobiIC interface
	ClTobiIc* ic;

	// Initialization support variables
	std::string absolute, relative;
	bool 	attached;
	int 	status, block;
	float 	value;
	float 	dt;
	float 	rate;
	float   freq;
	float 	ampl;
	CcTimeValue start;

	// Creation IC Message	
	icm = new ICMessage;
	ics = new ICSerializerRapid(icm);

	icclass = 	   new ICClass(classlbl, 0.0f);
	icclassifier = new ICClassifier("cl_ticsender", "cl_ticsender",
					ICClassifier::ValueProb, ICClassifier::LabelCustom);

	icclassifier->classes.Add(icclass);
	icm->classifiers.Add(icclassifier);

	ic = new ClTobiIc(ClTobiIc::SetOnly);

	// Connection to CNBI Loop
	if(ClLoop::Connect() == false) {
		CcLogFatal("Cannot connect to loop");
		CcCore::Exit(EXIT_FAILURE);
	}

	// Connection to ClTobiIc
	attached = false;
	CcLogInfoS("Waiting for connection at " << optname <<":"<<optport);
	while(attached == false) {
		attached = optport.empty() ? ic->Attach(optname) : ic->Attach(optport, optname);
		
		if(CcCore::receivedSIGAny.Get()) 
			CcCore::Exit(EXIT_FAILURE);
		
		CcTime::Sleep(1000.0f);
	}
	CcLogInfoS("Connected at " << optname <<":"<<optport);


	dt = 0;
	block = 0;
	CcTime::Tic(&start);
	rate = std::stof(optrate);
	freq = std::stof(optfreq);
	ampl = std::stof(optmax);

	CcLogInfoS("Sending sinewave to " << optname << 
			   " (A=" << ampl << ", freq=" << freq <<"Hz)" <<
			   " at " << rate << "Hz");
	while(true) {
		
		value = sinewave(ampl, CcTime::Toc(&start)/1000.0f, 0.2f);

		icm->SetValue("cl_ticsender", classlbl, value);

		if(ic->SetMessage(ics, block++) == ClTobiIc::Detached) {
			CcLogFatal("Tobi Ic detached");
			break;
		}
		
		if(dump == true)
			icm->Dump();
		
		CcTime::Sleep(1000.0f/rate);

		if(CcCore::receivedSIGAny.Get()) 
			break;
	}


	ic->Detach();

	delete ic;
	delete icclass;
	delete icclassifier;
	delete icm;
	delete ics;

	CcCore::Exit(0);
}
