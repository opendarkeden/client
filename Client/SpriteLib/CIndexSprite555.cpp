//----------------------------------------------------------------------
// CIndexSprite555.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "CSpriteSurface.h"
#include "CIndexSprite.h"
#include "CIndexSprite555.h"

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// fstream에 save 한다.    ( file에는 5:6:5로 저장한다. )
//----------------------------------------------------------------------
// Normal색에 대해서만 5:6:5로 바꿔주면 된다.
//----------------------------------------------------------------------
bool	
CIndexSprite555::SaveToFile(ofstream& file)
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
	register int k;

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
			indexCount = m_Pixels[i][index++];	// index색 수

			index += indexCount;	// index색에 대한 정보 수 만큼

			// Normal색 수
			colorCount = m_Pixels[i][index++];

			// m_Pixels[i][index] ~ m_Pixels[i][index+colorCount-1]
			// 5:5:5를 5:6:5로 바꿔서 저장하고 다시 5:5:5로 바꿔준다.
			for (k=0; k<colorCount; k++)								
			{
				m_Pixels[i][index] = CDirectDraw::Convert555to565(m_Pixels[i][index]);
				index++;
			}
		}

		// byte수와 실제 data를 저장한다.
		file.write((const char*)&index, 2);
		file.write((const char*)m_Pixels[i], index<<1);

		//-------------------------------------------------------
		// m_Pixels[i]에 한 줄을 Save했다.
		// 현재 줄에서.. Normal color에 해당하는 부분을
		// 다시 5:6:5에서 5:5:5로 바꿔줘야 한다.
		//-------------------------------------------------------

		// 반복 회수의 2 byte
		transPair = m_Pixels[i][0];
				
		WORD	index	= 1;

		for (j=0; j<transPair; j++)
		{			
			// transCount = m_Pixels[i][index];
			index++;	// 투명 수
			indexCount = m_Pixels[i][index++];	// index색수

			index += indexCount;	// index색에 대한 정보 수 만큼

			// Normal색 수
			colorCount = m_Pixels[i][index++];

			// m_Pixels[i][index] ~ m_Pixels[i][index+colorCount-1]
			// 5:6:5를 5:5:5로 바꾼다.
			for (k=0; k<colorCount; k++)								
			{
				m_Pixels[i][index] = CDirectDraw::Convert565to555(m_Pixels[i][index]);
				index++;
			}
		}
	}


	return true;
}

//----------------------------------------------------------------------
// fstream에서 load한다.
//----------------------------------------------------------------------
bool	
CIndexSprite555::LoadFromFile(ifstream& file)
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
	register int j;
	register int k;
	//--------------------------------
	// 5:6:5
	//--------------------------------
	for (int i=0; i<m_Height; i++)
	{
		// byte수와 실제 data를 Load한다.
		file.read((char*)&len, 2);
		
		m_Pixels[i] = NULL;
		m_Pixels[i] = new WORD [len];		

		file.read((char*)m_Pixels[i], len<<1);

		
		//-------------------------------------------------------
		// m_Pixels[i]에 한 줄을 Load했다.
		// 현재 줄에서.. Normal color에 해당하는 부분을
		// 5:6:5에서 5:5:5로 바꿔줘야 한다.
		//-------------------------------------------------------

		int transPair, colorCount, indexCount;

		// 반복 회수의 2 byte
		transPair = m_Pixels[i][0];
				
		WORD	index	= 1;

		for (j=0; j<transPair; j++)
		{			
			// transCount = m_Pixels[i][index];
			index++;	// 투명 수
			indexCount = m_Pixels[i][index++];	// indexPair 수

			index += indexCount;	// index색에 대한 정보 수 만큼

			// Normal색 수
			colorCount = m_Pixels[i][index++];

			// m_Pixels[i][index] ~ m_Pixels[i][index+colorCount-1]
			// 5:6:5를 5:5:5로 바꾼다.
			for (k=0; k<colorCount; k++)								
			{
				m_Pixels[i][index] = CDirectDraw::Convert565to555(m_Pixels[i][index]);
				index++;
			}
		}
	}	
	
	m_bInit = true;

	return true;
}