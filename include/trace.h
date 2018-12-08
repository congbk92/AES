#ifndef ___TRACE_H___
#define ___TRACE_H___
#include <stdarg.h>
#include <iostream>
#include <fstream>

//define NO_TRACE
#ifndef NO_TRACE

#define TRACE(...)		Tracer::getInstance()->writeLogRespectedLV(__FILE__, __LINE__, __func__,__VA_ARGS__)

// print log of current state in hexa format to debug
#define TRACE_STATE(state)	{														\
								char buffer[100] = ""; 								\
								for( int i =0; i<16; i++)							\
								{													\
									sprintf(buffer,"%s %2x", buffer, state[i]);		\
								}													\
								TRACE("%s", buffer);								\
							}


class Tracer
{
	public:
		static Tracer *getInstance();
		void writeLogRespectedLV(const char* file, int line, const char* func,const char *fmt, ...);
	private:
		static Tracer *p_instance;
		std::string _logFileName;
		Tracer();
		Tracer(std::string logfile);
		void initLogFile();
		~Tracer();
};

#else
#define	TRACE(...)			//do nothing
#endif /* NO_TRACE */
#endif /* ___TRACE_H___ */
