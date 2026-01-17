// IVolume.h : IVolume interface definition.
// Developer : Alex Chmut
// Created : 8/11/98
#pragma once

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#else
#include "../../basic/Platform.h"
#endif

// User-defined callback for volume change notification
#ifndef CALLBACK
#define CALLBACK
#endif

typedef void (CALLBACK *PONMICVOULUMECHANGE)( DWORD dwCurrentVolume, DWORD dwUserValue );

////////////////////////////////////////////////////////////////////////
// IVolume interface
#ifdef PLATFORM_WINDOWS
class __declspec(novtable) IVolume
#else
class IVolume
#endif
{
public:
	virtual bool	IsAvailable() = 0;
	virtual void	Enable() = 0;
	virtual void	Disable() = 0;

	virtual DWORD	GetVolumeMetric() = 0;

	virtual DWORD	GetMinimalVolume() = 0;
	virtual DWORD	GetMaximalVolume() = 0;

	virtual DWORD	GetCurrentVolume() = 0;
	virtual void	SetCurrentVolume( DWORD dwValue ) = 0;

	virtual void	RegisterNotificationSink( PONMICVOULUMECHANGE, DWORD ) = 0;

	virtual void	Done() = 0;
};