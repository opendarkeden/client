//----------------------------------------------------------------------
// KeyAccelerator.cpp
//----------------------------------------------------------------------

#include "client_PCH.h"
#include <fstream.h>
#include "KeyAccelerator.h"
#include "CDirectInput.h"
#include <cstdio>
//----------------------------------------------------------------------
// define functions
//----------------------------------------------------------------------
#define READ_CHECK_EOF( value, temp, bytes )		\
		{											\
			file.read((char*)&temp, bytes);			\
			if (!file.eof())						\
			{										\
				value = temp;						\
				return false;						\
			}										\
			else									\
			{										\
				return true;						\
			}										\
		}
//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
KeyAccelerator*		g_pKeyAccelerator = NULL;

//----------------------------------------------------------------------
//
// constructor / destructor
//
//----------------------------------------------------------------------
KeyAccelerator::KeyAccelerator()
{
}

KeyAccelerator::~KeyAccelerator()
{
}

//----------------------------------------------------------------------
// Init ( max )
//----------------------------------------------------------------------
void				
KeyAccelerator::Init( int max )
{
	m_Keys.clear();
	m_Accelerators.clear();

	m_Accelerators.reserve( max );
}

//----------------------------------------------------------------------
// Set AcceleratorKey ( accel, key )
//----------------------------------------------------------------------
// key를 누르면 accel이 실행된다..는 의미.
// accel은 key와 연결되어있다..는 의미도 되고..
//----------------------------------------------------------------------
void				
KeyAccelerator::SetAcceleratorKey(BYTE accel, WORD key)
{
	if (accel >= m_Accelerators.capacity())
	{
		return;
	}

	KEY_MAP::iterator iKey = m_Keys.find( m_Accelerators[accel] );

	if (iKey != m_Keys.end())
	{
		// 이미 설정된 key가 있었다면 제거한다.
		m_Keys.erase( iKey );
	}

	m_Keys[key] = accel;
	
	m_Accelerators[accel] = key;
}

//----------------------------------------------------------------------
// Get Accelerator ( key )
//----------------------------------------------------------------------
// 확실히 그 key만 입력된 경우
//----------------------------------------------------------------------
BYTE
KeyAccelerator::GetAccelerator(WORD key) const
{
	KEY_MAP::const_iterator iKey = m_Keys.find( key );

	if (iKey==m_Keys.end())
	{
		return 0;
	}

	return iKey->second;
}

//----------------------------------------------------------------------
// Get AcceleratorSimilar ( key )
//----------------------------------------------------------------------
// ctrl+I만 눌러서 실행되는게
// ctrl+shift+I를 누르면 실행이 안된다.
// 이런 경우에도 실행되게 할려면.. 조합키를 고려해서 체크해야 한다.
//----------------------------------------------------------------------
BYTE				
KeyAccelerator::GetAcceleratorSimilar(WORD key) const
{
	// 입력된 key
	KEY_MAP::const_iterator iKey = m_Keys.find( key );

	if (iKey!=m_Keys.end())
	{
		return iKey->second;
	}

	WORD checkKey;	

	int bHasControl	= ACCEL_HAS_CONTROL(key);
	int bHasAlt		= ACCEL_HAS_ALT(key);
	int bHasShift	= ACCEL_HAS_SHIFT(key);

	//------------------------------------------------------------
	// 눌린 key가 없는 경우
	//------------------------------------------------------------
//	if (!bHasControl && !bHasAlt && !bHasShift)
//	{
//		return 0;
//	}

	//------------------------------------------------------------
	// ctrl제거
	//------------------------------------------------------------
	if (bHasControl)
	{
		checkKey = ACCEL_REMOVE_CONTROL( key );
		iKey = m_Keys.find( checkKey );
		if (iKey!=m_Keys.end())	return iKey->second;
	}

	//------------------------------------------------------------
	// alt제거
	//------------------------------------------------------------
	if (bHasAlt)
	{
		checkKey = ACCEL_REMOVE_ALT( key );
		iKey = m_Keys.find( checkKey );
		if (iKey!=m_Keys.end())	return iKey->second;
	}

	//------------------------------------------------------------
	// shift제거
	//------------------------------------------------------------
	if (bHasShift)
	{
		checkKey = ACCEL_REMOVE_SHIFT( key );
		iKey = m_Keys.find( checkKey );
		if (iKey!=m_Keys.end())	return iKey->second;
	}

	WORD keyOnly = ACCEL_GET_KEY( key );

	//------------------------------------------------------------
	// ctrl + alt + shift 인 경우
	//------------------------------------------------------------
	if (bHasControl && bHasAlt && bHasShift)
	{
		//------------------------------------------------------------
		// ctrl만 남긴다.
		//------------------------------------------------------------
		checkKey = ACCEL_ADD_CONTROL( keyOnly );
		iKey = m_Keys.find( checkKey );
		if (iKey!=m_Keys.end())	return iKey->second;	

		//------------------------------------------------------------
		// alt만 남긴다
		//------------------------------------------------------------
		checkKey = ACCEL_ADD_ALT( keyOnly );
		iKey = m_Keys.find( checkKey );
		if (iKey!=m_Keys.end())	return iKey->second;		

		//------------------------------------------------------------
		// shift만 남긴다
		//------------------------------------------------------------
		checkKey = ACCEL_ADD_SHIFT( keyOnly );
		iKey = m_Keys.find( checkKey );
		if (iKey!=m_Keys.end())	return iKey->second;
	}

	//------------------------------------------------------------
	// 다 제거한 경우
	//------------------------------------------------------------
	iKey = m_Keys.find( keyOnly );
	if (iKey!=m_Keys.end())	return iKey->second;

	return 0;
}

//----------------------------------------------------------------------
// Get Key ( accel )
//----------------------------------------------------------------------
WORD
KeyAccelerator::GetKey(BYTE accel) const
{
	if (accel < m_Accelerators.capacity())
	{	
		return m_Accelerators[accel];
	}

	return 0;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void				
KeyAccelerator::SaveToFile(FILE *file)
{
	int num = m_Accelerators.capacity();

	fwrite((const void*)&num, 1, 4, file);

	// Only the key-accelerator relationship needs to be saved.
	// Because accel is in order, so there is no need to save.
	// So we only need to store the key.
	for (int accel=1; accel<num; accel++)
	{
		WORD key = m_Accelerators[accel];
		fwrite((const void*)&key, 1, 2, file);
	}
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void				
KeyAccelerator::LoadFromFile(FILE *file)
{
	int num;
	fread((void*)&num, 1, 4, file);

	Init( num );

	WORD key;

	for (int accel=1; accel<num; accel++)
	{
		int sz;
		sz = fread((void*)&key, 1, 2, file);
		if (sz == 2) {
			SetAcceleratorKey(accel, key);
		}
	}
}
