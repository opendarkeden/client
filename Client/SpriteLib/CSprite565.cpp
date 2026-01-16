//----------------------------------------------------------------------
// CSprite565.cpp
//----------------------------------------------------------------------
#include "client_PCH.h"
#include "CSprite.h"
#include "CSprite565.h"

//extern BYTE	LOADING_STATUS_NONE;
//extern BYTE	LOADING_STATUS_NOW;
//extern BYTE	LOADING_STATUS_LOADING;

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// fstream에 save 한다.    ( file에는 5:6:5로 저장한다. )
//----------------------------------------------------------------------
bool	
CSprite565::SaveToFile(ofstream& file)
{
	// width와 height를 저장한다.
	file.write((const char*)&m_Width , 2);
	file.write((const char*)&m_Height, 2);

	// NULL이면 저장하지 않는다. 길이만 저장되는 것이다.
	if (m_Pixels==NULL || m_Width==0 || m_Height==0)
		return false;
	
	// 압축 된 것 저장
	WORD index;	

	register int i;
	register int j;

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// 반복 회수의 2 byte
		int	count = m_Pixels[i][0], 
				colorCount;
		index	= 1;

		// 각 line마다 byte수를 세어서 저장해야한다.
		for (j=0; j<count; j++)
		{
			//transCount = m_Pixels[i][index];
			colorCount = m_Pixels[i][index+1];

			index+=2;	// 두 count 만큼

			index += colorCount;	// 투명색 아닌것만큼 +				
		}

		// byte수와 실제 data를 저장한다.
		file.write((const char*)&index, 2);
		file.write((const char*)m_Pixels[i], index<<1);
	}


	return true;
}

//----------------------------------------------------------------------
// fstream에서 load한다.
//----------------------------------------------------------------------
bool	
CSprite565::LoadFromFile(ifstream& file)
{
	// 이거를 하나로 묶어야 하는데..
	if (m_bLoading) 
	{	
		return false;
	}
	m_bLoading = true;

	// F --> T
	/*
	bool*	pCheck = &m_bLoading;

	bool	T = true;
	bool	F = false;

	// Loading하고 있지 않은 상태인 경우
	// Loading할려는 상태로 만든다.
	void* pOld = InterlockedCompareExchange( 
						(PVOID *)&pCheck,  // pointer to the destination pointer
						&T,      // the exchange value
						&F		// the value to compare
					);
 
	bool old = *(bool*)pOld;

	// 지금 loading할것이 아니면 return
	*/

	// loadind중이라고 표시
	m_bLoading = true;
	

	// 이미 잡혀있는 memory를 release한다.
	Release();

	//long start = file.tellg();

	// width와 height를 Load한다.
	file.read((char*)&m_Width , 2);
	file.read((char*)&m_Height, 2);	


	// 길이가 0이면 더 Load할게 없겠지..
	if (m_Width==0 || m_Height==0) 
	{	
		m_bInit = true;

		m_bLoading = false;

		return true;
	}

	//---------------------------------
	// for OLD version of CSprite
	//---------------------------------
	//BOOL dummy;
	//file.read((char*)&dummy, 1);	
	//---------------------------------
	
	m_Pixels = NULL;
	m_Pixels = new WORD* [m_Height];	
	//m_Pixels = (WORD**)malloc(sizeof(WORD*)*m_Height);

	WORD len;

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// byte수와 실제 data를 Load한다.
		file.read((char*)&len, 2);
		
		m_Pixels[i] = NULL;
		m_Pixels[i] = new WORD [len];		
		//m_Pixels[i] = (WORD*)malloc(sizeof(WORD)*len);		

		file.read((char*)m_Pixels[i], len<<1);		
	}	
	
	m_bInit = true;

	m_bLoading = false;

	return true;
}


//----------------------------------------------------------------------
// fstream에서 Buffer로 load한다.
//----------------------------------------------------------------------
/*
bool	
CSprite565::LoadFromFileToBuffer(ifstream& file)
{
	// width와 height를 Load한다.
	file.read((char*)&s_Width , 2);
	file.read((char*)&s_Height, 2);	

	// 길이가 0이면 더 Load할게 없겠지..
	if (s_Width==0 || s_Height==0) 
		return false;

	//---------------------------------
	// for OLD version of CSprite
	//---------------------------------
	//BOOL dummy;
	//file.read((char*)&dummy, 1);	
	//---------------------------------

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<s_Height; i++)
	{
		// byte수와 실제 data를 Load한다.
		file.read((char*)&s_BufferLen[i], 2);		
		
		file.read((char*)s_Buffer[i], s_BufferLen[i]<<1);
	}	
	
	return true;
}
*/