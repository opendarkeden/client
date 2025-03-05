//----------------------------------------------------------------------
// MObject.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MObject.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------
MObject::MObject()	
{ 
	m_ObjectType	= TYPE_OBJECT; 
	m_ID			= OBJECTID_NULL;
	m_X	 			= SECTORPOSITION_NULL;
	m_Y				= SECTORPOSITION_NULL;
	ClearScreenRect();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Add ScreenRect
//----------------------------------------------------------------------
// 화면에서의 Sprite충돌 사각 영역을 추가한다.
//----------------------------------------------------------------------
void				
MObject::AddScreenRect(RECT* pRect)
{
	if (pRect->left < m_ScreenRect.left)		m_ScreenRect.left	= pRect->left;
	if (pRect->top < m_ScreenRect.top)			m_ScreenRect.top	= pRect->top;
	if (pRect->right > m_ScreenRect.right)		m_ScreenRect.right	= pRect->right;
	if (pRect->bottom > m_ScreenRect.bottom)	m_ScreenRect.bottom	= pRect->bottom;
}

//----------------------------------------------------------------------
// Save to File
//----------------------------------------------------------------------
void	
MObject::SaveToFile(ofstream& file)
{
	file.write((const char*)&m_ObjectType, 1);
	file.write((const char*)&m_ID, SIZE_OBJECTID);
	file.write((const char*)&m_X, SIZE_SECTORPOSITION);
	file.write((const char*)&m_Y, SIZE_SECTORPOSITION);
}

//----------------------------------------------------------------------
// Load from File
//----------------------------------------------------------------------
void	
MObject::LoadFromFile(ifstream& file)
{
	file.read((char*)&m_ObjectType, 1);
	file.read((char*)&m_ID, SIZE_OBJECTID);	
	file.read((char*)&m_X, SIZE_SECTORPOSITION);
	file.read((char*)&m_Y, SIZE_SECTORPOSITION);
}

