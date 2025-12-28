//////////////////////////////////////////////////////////////////////
// 
// Filename    : Encrypter.h 
// Written by  : sigi
// Description :
// 
//////////////////////////////////////////////////////////////////////

#ifndef __ENCRYPTER__
#define __ENCRYPTER__

// include files
#include "Types.h"
#include "Exception.h"
#include "EncryptUtility.h"

#define __USE_ENCRYPTER__

class Encrypter
{
public :
	Encrypter() { setCode(0); }

	void	setCode(uchar code);
	uchar	getCode() const	{ return m_ucharCode; }

	operator bool () const		{ return m_bCode; }
	operator char () const		{ return m_charCode; }
	operator uchar () const		{ return m_ucharCode; }
	operator short () const		{ return m_shortCode; }
	operator ushort () const	{ return m_ushortCode; }
	operator int () const		{ return m_intCode; }
	operator uint () const		{ return m_uintCode; }
	operator long () const		{ return m_longCode; }
	operator ulong () const		{ return m_ulongCode; }

	bool	convert(bool value)		{ return value^m_bCode; }
	uchar	convert(char value)		{ return value^m_charCode; }
	uchar	convert(uchar value)	{ return value^m_ucharCode; }
	ushort	convert(short value)	{ return value^m_shortCode; }
	ushort	convert(ushort value)	{ return value^m_ushortCode; }
	uint	convert(int value)		{ return value^m_intCode; }
	uint	convert(uint value)		{ return value^m_uintCode; }
	long	convert(long value)		{ return value^m_longCode; }
	ulong	convert(ulong value)	{ return value^m_ulongCode; }


private :
	bool		m_bCode;
	char		m_charCode;
	uchar		m_ucharCode;
	short		m_shortCode;
	ushort		m_ushortCode;
	int			m_intCode;
	uint		m_uintCode;	
	long		m_longCode;
	ulong		m_ulongCode;	
};


#endif
