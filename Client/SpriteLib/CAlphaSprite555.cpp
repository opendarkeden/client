//----------------------------------------------------------------------
// CAlphaSprite555.cpp
//----------------------------------------------------------------------
#include "CSpriteSurface.h"
#include "CAlphaSprite.h"
#include "CAlphaSprite555.h"

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// fstream에 save 한다.    ( file에는 5:6:5로 저장한다. )
//----------------------------------------------------------------------
bool
CAlphaSprite555::SaveToFile(ofstream& file)
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
	register int k;

	//--------------------------------
	// 5:5:5
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

			// m_Pixels[i][index] ~ m_Pixels[i][index+colorCount-1]
			// 5:5:5를 5:6:5로 바꿔서 저장하고 다시 5:5:5로 바꿔준다.

			// Alpha값 skip
			index++;
			for (k=0; k<colorCount; k++)								
			{
				m_Pixels[i][index] = ColorDraw::Convert555to565(m_Pixels[i][index]);

				// Alpha,Color, Alpha,Color, .... 순이기 때문에
				// Color다음에 Color를 바꾸기 위해서..
				index+=2;
			}
			index--;

			//index += colorCount;	// 투명색 아닌것만큼 +				
		}

		// byte수와 실제 data를 저장한다.
		file.write((const char*)&index, 2);			
		file.write((const char*)m_Pixels[i], index<<1);


		// 다시 5:5:5로 바꿔준다.						
		index	= 1;
			
		for (j=0; j<count; j++)
		{
			//transCount = m_Pixels[i][index];
			colorCount = m_Pixels[i][index+1];				

			index+=2;	// 두 count 만큼

			// m_Pixels[i][index] ~ m_Pixels[i][index+colorCount-1]
			// 5:5:5를 5:6:5로 바꿔서 저장하고 다시 5:5:5로 바꿔준다.
			index++;
			for (k=0; k<colorCount; k++)								
			{					
				m_Pixels[i][index] = ColorDraw::Convert565to555(m_Pixels[i][index]);
				index+=2;
			}
			index--;

			//index += colorCount;	// 투명색 아닌것만큼 +				
		}
	}

	return true;
}

//----------------------------------------------------------------------
// fstream에서 load한다.
//----------------------------------------------------------------------
bool	
CAlphaSprite555::LoadFromFile(ifstream& file)
{
	// 이미 잡혀있는 memory를 release한다.
	Release();

	// width와 height를 저장한다.
	file.read((char*)&m_Width , 2);
	file.read((char*)&m_Height, 2);	

	// 길이가 0이면 더 Load할게 없겠지..
	if (m_Width==0 || m_Height==0) 
	{	
		m_bInit = true;

		return true;
	}

	//---------------------------------
	// for OLD version of CAlphaSprite
	//---------------------------------
	//BOOL dummy;
	//file.read((char*)&dummy, 1);	
	//---------------------------------
	
	m_Pixels = new WORD* [m_Height];

	WORD len;

	//--------------------------------
	// 5:5:5
	//--------------------------------
	// 5:6:5로 저장된걸 읽었기 때문에 5:6:5를 5:5:5로 바꿔줘야 한다.	
	int	count, index, colorCount;

	register int i;
	register int j;

	for (int i=0; i<m_Height; i++)
	{			
		// byte수와 실제 data를 Load한다.
		file.read((char*)&len, 2);
		m_Pixels[i] = new WORD [len];
		file.read((char*)m_Pixels[i], len<<1);

		count = m_Pixels[i][0];			
		index = 1;

		for (j=0; j<count; j++)
		{
			//transCount = m_Pixels[i][index];
			colorCount = m_Pixels[i][index+1];				

			index+=2;	// 두 count 만큼

			// m_Pixels[i][index] ~ m_Pixels[i][index+colorCount-1]
			// 5:5:5를 5:6:5로 바꿔서 저장하고 다시 5:5:5로 바꿔준다.
			index++;	// Alpha값 skip
			for (int j=0; j<colorCount; j++)								
			{					
				m_Pixels[i][index] = ColorDraw::Convert565to555(m_Pixels[i][index]);
				index+=2;
			}
			index--;

			//index += colorCount;	// 투명색 아닌것만큼 +				
		}
	}

	m_bInit = true;

	return true;
}


