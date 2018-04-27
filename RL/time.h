#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>
class CTime
{
	double m_dClockRate;
public:
	CTime::CTime();
	float Time();
	double TimeDouble();
};

#endif