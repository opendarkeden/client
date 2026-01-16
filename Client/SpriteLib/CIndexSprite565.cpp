//----------------------------------------------------------------------
// CIndexSprite565.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CSpriteSurface.h"
#include "CIndexSprite.h"
#include "CIndexSprite565.h"

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// fstream에 save 한다.    ( file에는 5:6:5로 저장한다. )
//----------------------------------------------------------------------
bool	
CIndexSprite565::SaveToFile(ofstream& file)
{
	// width와 height를 저장한다.
	file.write((const char*)&m_Width , 2);
	file.write((const char*)&m_Height, 2);

	// NULL이면 저장하지 않는다. 길이만 저장되는 것이다.
	if (m_Pixels==NULL || m_Width==0 || m_Height==0)
		return false;
	
	// 압축 된 것 저장
	WORD index;	

	int colorCount, transPair, indexCount;

	register int i;
	register int j;

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// 반복 회수의 2 byte
		transPair = m_Pixels[i][0];
				
		index	= 1;

		// 각 line마다 byte수를 세어서 저장해야한다.
		for (j=0; j<transPair; j++)
		{			
			// transCount = m_Pixels[i][index];
			index++;	// 투명 수
			indexCount = m_Pixels[i][index++];	// indexPair 수

			index += indexCount;		// index색에 대한 정보 수 만큼

			// Normal색 수
			colorCount = m_Pixels[i][index++];

			// Normal색 수만큼 증가
			index	+= colorCount;
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
CIndexSprite565::LoadFromFile(ifstream& file)
{
	// 이미 잡혀있는 memory를 release한다.
	Release();

	// width와 height를 Load한다.
	file.read((char*)&m_Width , 2);
	file.read((char*)&m_Height, 2);	

	// 길이가 0이면 더 Load할게 없겠지..
	if (m_Width==0 || m_Height==0) 
	{	
		m_bInit = true;

		return true;
	}
	
	m_Pixels = NULL;
	m_Pixels = new WORD* [m_Height];	
	WORD len;

	register int i;

	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// byte수와 실제 data를 Load한다.
		file.read((char*)&len, 2);
		
		m_Pixels[i] = NULL;
//		if(len>0)		//add by viva
			m_Pixels[i] = new WORD [len];		

		file.read((char*)m_Pixels[i], len<<1);
	}	

	m_bInit = true;

	return true;
}