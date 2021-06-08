/*-----------------------------------------------------------------------------

	Typedef.h

	General type definition.
	`BOOL, TRUE, FALSE 사용안함. C++ bool, true, false 사용.

	19971226. KJTINC
	19991106.
	2000.8.5.
	
-----------------------------------------------------------------------------*/

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#define NULL						      0

#define NOT_SELECTED						-1

//
// type is all upper case and
// defined unsigned type
//
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned int	UINT;
typedef unsigned long   DWORD;

//
// id_t
//
// a identifier.
//
// Purpose:
//				General use.
//
typedef DWORD				id_t;

//
// Character type definition for DBCS(Double-Byte Character Set).
//
// (2000.10.4)
// Wide Chacter용 wchar_t는 아직 사용하지 않는다. 또한 header 충돌을 피하기 위해서 다른 이름을
// 사용한다. 이것은 Unicode가 정착되면 완전히 wchar_t로 대체되던가 typedef wchar_t char_t로
// 바뀔 것이다.
//
typedef WORD char_t;

#endif
