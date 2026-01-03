//----------------------------------------------------------------------
// CFrame.cpp
//----------------------------------------------------------------------
#include "Frame_PCH.h"
#include "CFrame.h"

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Set
//----------------------------------------------------------------------
void
CFrame::Set(TYPE_SPRITEID spriteID, short cx, short cy)
{
	m_SpriteID	= spriteID;
	m_cX		= cx;
	m_cY		= cy;
	//m_fEffect	= 0;		// effect flag
}

//----------------------------------------------------------------------
// Save to File
//----------------------------------------------------------------------
void
CFrame::SaveToFile(ofstream& file)
{
	file.write((const char*)&m_SpriteID, SIZE_SPRITEID);
	file.write((const char*)&m_cX, 2);
	file.write((const char*)&m_cY, 2);
	//file.write((const char*)&m_fEffect, 1);
}

//----------------------------------------------------------------------
// Load from File
//----------------------------------------------------------------------
void
CFrame::LoadFromFile(ifstream& file)
{
	file.read((char*)&m_SpriteID, SIZE_SPRITEID);
	file.read((char*)&m_cX, 2);
	file.read((char*)&m_cY, 2);
	//file.read((char*)&m_fEffect, 1);
}

//----------------------------------------------------------------------
// Assign Operator =
//----------------------------------------------------------------------
void
CFrame::operator = (const CFrame& frame)
{
	m_SpriteID	= frame.m_SpriteID;
	m_cX		= frame.m_cX;
	m_cY		= frame.m_cY;
	//m_fEffect	= frame.m_fEffect;
}


//----------------------------------------------------------------------
// Save to File
//----------------------------------------------------------------------
void
CEffectFrame::SaveToFile(ofstream& file)
{
	CFrame::SaveToFile(file);	
	
	if (m_bBackground)
	{
		char temp = m_Light | 0x80;
		
		file.write((const char*)&temp, 1);
	}
	else
	{
		file.write((const char*)&m_Light, 1);
	}

	
	//file.write((const char*)&m_fEffect, 1);

	
}

//----------------------------------------------------------------------
// Load from File
//----------------------------------------------------------------------
void
CEffectFrame::LoadFromFile(ifstream& file)
{
	CFrame::LoadFromFile(file);
	file.read((char*)&m_Light, 1);

	if (m_Light & 0x80)
	{
		m_bBackground = true;
		m_Light &= 0x7F;
	}
	else
	{
		m_bBackground = false;
	}

	//file.read((char*)&m_fEffect, 1);
}

//----------------------------------------------------------------------
// Assign Operator =
//----------------------------------------------------------------------
void
CEffectFrame::operator = (const CEffectFrame& frame)
{	
	m_SpriteID		= frame.m_SpriteID;
	m_cX			= frame.m_cX;
	m_cY			= frame.m_cY;
	m_Light			= frame.m_Light;
	m_bBackground	= frame.m_bBackground;
}

