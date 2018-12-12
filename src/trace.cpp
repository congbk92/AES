#include <stddef.h>  // defines NULL
#include "trace.h"

#ifdef DEBUG
Tracer* Tracer::p_instance = NULL; 
Tracer* Tracer::getInstance()
{
	if (!p_instance)
	{
		p_instance = new Tracer;
	}
	return p_instance;
}
void Tracer::writeLogRespectedLV(const char* file, int line, const char* func, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char outLog[2048];
	vsprintf(outLog,fmt, ap);

	std::ofstream plogfile;
  	plogfile.open (_logFileName.c_str(), std::ofstream::out | std::ofstream::app);
  	plogfile <<file<<":"<<line<<":"<<func<<"(): "<<outLog<<"\n";
  	plogfile.close();
}

Tracer::Tracer()
{
	_logFileName = "logfile.txt";
	initLogFile();
}
Tracer::Tracer(std::string logfile)
{
	_logFileName = logfile;
	initLogFile();
}
void Tracer::initLogFile()
{
	std::ofstream plogfile;
  	plogfile.open (_logFileName.c_str());
  	plogfile.close();
}
Tracer::~Tracer()
{
	delete p_instance;
}
#endif
