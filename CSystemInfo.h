/********************************************************************
	created:	2003/10/17
	created:	17:10:2003   13:39
	filename: 	E:\designed\project\CSystemInfo.h
	file path:	E:\designed\project
	file base:	CSystemInfo
	file ext:	h
	author:		sonee
	
	purpose:
*********************************************************************/


#pragma once

class CSystemInfo
{
public :

	long			GetCpuClock();

private :	
	unsigned __int64			m_start;
	unsigned __int64			m_overhead;
};
