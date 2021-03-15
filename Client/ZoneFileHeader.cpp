//----------------------------------------------------------------------
// ZoneFileHeader.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "ZoneFileHeader.h"

//----------------------------------------------------------------------
//
// FILEINFO_ZONE_HEADER
//
//----------------------------------------------------------------------
void		
FILEINFO_ZONE_HEADER::SaveToFile(ofstream& file)
{
	ZoneVersion.SaveToFile(file);
	file.write((const char*)&ZoneID, 2);
	file.write((const char*)&ZoneGroupID, 2);
	ZoneName.SaveToFile(file);
	file.write((const char*)&ZoneType, 1);
	file.write((const char*)&ZoneLevel, 1);
	Description.SaveToFile(file);
}
void		
FILEINFO_ZONE_HEADER::LoadFromFile(ifstream& file)//∂¡»°µÿÕº
{
	ZoneVersion.LoadFromFile(file);
	file.read((char*)&ZoneID, 2);
	file.read((char*)&ZoneGroupID, 2);
	ZoneName.LoadFromFile(file);
	file.read((char*)&ZoneType, 1);
	file.read((char*)&ZoneLevel, 1);
	Description.LoadFromFile(file);
}
