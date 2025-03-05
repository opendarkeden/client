//----------------------------------------------------------------------
// MPortal.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MPortal.h"

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

MPortal::MPortal()
{
	m_ZoneID.clear();
	SetRect(0,0,0,0);
	m_Type = 0;

}

MPortal::MPortal(std::vector<WORD> ZoneID, P_RECT rect, BYTE Type)
{
	m_ZoneID = ZoneID;
	SetRect(rect);
	m_Type = Type;
}

MPortal::MPortal(std::vector<WORD> ZoneID, BYTE left, BYTE top, BYTE right, BYTE bottom, BYTE Type)
{
	m_ZoneID = ZoneID;
	SetRect(left, top, right, bottom);
	m_Type = Type;
}

MPortal::~MPortal()
{
}


//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void	
MPortal::SaveToFile(ofstream& file)
{
	file.write((const char*)&m_Type, 1);

	BYTE size = m_ZoneID.size();

	if(m_Type == TYPE_MULTI_PORTAL)
	{
		file.write((const char*)&size, 1);	
	}

	for(int i = 0; i < size; i++)
		file.write((const char*)&m_ZoneID[i], 2);	

	file.write((const char*)&m_Rect, SIZE_P_RECT);	
}
		
//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void	
MPortal::LoadFromFile(ifstream& file)
{
	file.read((char*)&m_Type, 1);
	WORD z;

	BYTE size;

	if(m_Type == TYPE_MULTI_PORTAL)
		file.read((char*)&size, 1);	
	else size = 1;

	m_ZoneID.clear();
	for(int i = 0; i < size; i++)
	{
		file.read((char*)&z, 2);	
		m_ZoneID.push_back(z);
	}

	file.read((char*)&m_Rect, SIZE_P_RECT);	
}