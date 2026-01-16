//----------------------------------------------------------------------
// SectorSoundInfo.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "SectorSoundInfo.h"

//----------------------------------------------------------------------
//
// SECTORSOUND_INFO
//
//----------------------------------------------------------------------
void
SECTORSOUND_INFO::SaveToFile(std::ofstream& file) const
{
	file.write((const char*)&ZoneSoundID, 2);
	file.write((const char*)&X, 1);
	file.write((const char*)&Y, 1);
}

void
SECTORSOUND_INFO::LoadFromFile(std::ifstream& file)
{
	file.read((char*)&ZoneSoundID, 2);
	file.read((char*)&X, 1);
	file.read((char*)&Y, 1);
}
