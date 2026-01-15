//----------------------------------------------------------------------
// KeyAccelerator.h
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Set Default
//----------------------------------------------------------------------
// default file�� �����صΰ� load�ϴ� �͵� �������� ����.
//----------------------------------------------------------------------
/*
void				
SetDefault()
{
	const WORD keys[MAX_ACCELERATOR] =  
	{
		0,											// ACCEL_NULL,
		DIK_I |	FLAG_ACCELERATOR_CONTROL,			// ACCEL_CHARINFO,
	};

	for (int accel=1; accel<MAX_ACCELERATOR; accel++)
	{
		g_pKeyAccelerator->SetAcceleratorKey( accel, keys[accel] );
	}
}
*/
//----------------------------------------------------------------------


#ifndef __KEYACCELERATOR_H__
#define __KEYACCELERATOR_H__

#ifdef PLATFORM_WINDOWS
	#pragma warning(disable:4786)
#endif

#include <map>
#include <vector>
#include "DIK_Defines.h"
#include "AcceleratorDef.h"

//----------------------------------------------------------------------
// Accelerator Key Defines
//----------------------------------------------------------------------
#define	FLAG_ACCELERATOR_CONTROL	0x0100
#define	FLAG_ACCELERATOR_ALT		0x0200
#define	FLAG_ACCELERATOR_SHIFT		0x0400

#define	ACCEL_MAKE_KEY(key, flag)	(key | flag)
#define	ACCEL_GET_KEY(key)			(key & 0x00FF)

#define	ACCEL_ADD_CONTROL(key)		(key | FLAG_ACCELERATOR_CONTROL)
#define	ACCEL_ADD_ALT(key)			(key | FLAG_ACCELERATOR_ALT)
#define	ACCEL_ADD_SHIFT(key)		(key | FLAG_ACCELERATOR_SHIFT)

#define	ACCEL_REMOVE_CONTROL(key)	(key & ~FLAG_ACCELERATOR_CONTROL)
#define	ACCEL_REMOVE_ALT(key)		(key & ~FLAG_ACCELERATOR_ALT)
#define	ACCEL_REMOVE_SHIFT(key)		(key & ~FLAG_ACCELERATOR_SHIFT)

#define	ACCEL_HAS_CONTROL(key)		(key & FLAG_ACCELERATOR_CONTROL)
#define	ACCEL_HAS_ALT(key)			(key & FLAG_ACCELERATOR_ALT)
#define	ACCEL_HAS_SHIFT(key)		(key & FLAG_ACCELERATOR_SHIFT)


//----------------------------------------------------------------------
// KeyAccelerator
//----------------------------------------------------------------------
// key�� WORD, accel�� BYTE.. ����.. 
//----------------------------------------------------------------------
class KeyAccelerator {
	public :
		typedef std::map<WORD, BYTE>			KEY_MAP;			// key = accel
		typedef std::vector<WORD>				ACCELERATOR_VECTOR;	// accel = key

	public :
		KeyAccelerator();
		~KeyAccelerator();

		//------------------------------------------------------------
		// Init
		//------------------------------------------------------------
		void				Init( int max );

		//------------------------------------------------------------
		// Set AcceleratorKey
		//------------------------------------------------------------
		void				SetAcceleratorKey(BYTE accel, WORD key);

		//------------------------------------------------------------
		// Get Accelerator / Key
		//------------------------------------------------------------
		BYTE				GetAccelerator(WORD key) const;			// key�� ����� accelerator�� ã�´�.
		BYTE				GetAcceleratorSimilar(WORD key) const;	// key�� ����� accelerator�� ã�´�. key������ ������ ��쵵 üũ�Ѵ�.
		WORD				GetKey(BYTE accel) const;				// accelerator�� ����� key�� ã�´�.
		const char *		GetName(BYTE accel) const
		{
			if(accel >= MAX_ACCELERATOR)
				return NULL;
			return ACCELERATOR_NAME[accel];
		}

		//------------------------------------------------------------
		// File I/O
		//------------------------------------------------------------
		void				SaveToFile(FILE* file);
		void				LoadFromFile(FILE* file);


	protected :
		KEY_MAP				m_Keys;					// key�� accelerator�� ����
		ACCELERATOR_VECTOR	m_Accelerators;			// accelerator�� key�� ����			
};

extern KeyAccelerator*	g_pKeyAccelerator;

#endif

