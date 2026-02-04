//----------------------------------------------------------------------
// ZoneFileHeader.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "ZoneFileHeader.h"
#include <cstdint>

//----------------------------------------------------------------------
//
// FILEINFO_ZONE_HEADER
//
//----------------------------------------------------------------------
void		
FILEINFO_ZONE_HEADER::SaveToFile(std::ofstream& file)
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
FILEINFO_ZONE_HEADER::LoadFromFile(std::ifstream& file)//뗍혤뒈暠
{
	ZoneVersion.LoadFromFile(file);
	file.read((char*)&ZoneID, 2);
	file.read((char*)&ZoneGroupID, 2);
	ZoneName.LoadFromFile(file);

	// Some legacy map files store ZoneType/ZoneLevel as WORDs instead of BYTEs.
	// Detect layout by peeking the description length with both interpretations.
	const std::streampos afterName = file.tellg();

	auto peekDescLen = [&](int typeSizeBytes, uint32_t& outLen) {
		file.clear();
		file.seekg(afterName);
		file.ignore(typeSizeBytes * 2);
		file.read((char*)&outLen, 4);
	};

	uint32_t descLenByteLayout = 0xFFFFFFFFu;
	uint32_t descLenWordLayout = 0xFFFFFFFFu;
	peekDescLen(1, descLenByteLayout);
	peekDescLen(2, descLenWordLayout);

	const uint32_t kMaxDescLen = 65536;
	const bool byteLayoutOk = descLenByteLayout <= kMaxDescLen;
	const bool wordLayoutOk = descLenWordLayout <= kMaxDescLen;

	file.clear();
	file.seekg(afterName);

	if (byteLayoutOk || !wordLayoutOk)
	{
		file.read((char*)&ZoneType, 1);
		file.read((char*)&ZoneLevel, 1);
		Description.LoadFromFile(file);
	}
	else
	{
		uint16_t typeWord = 0;
		uint16_t levelWord = 0;
		file.read((char*)&typeWord, 2);
		file.read((char*)&levelWord, 2);
		ZoneType = static_cast<BYTE>(typeWord & 0xFF);
		ZoneLevel = static_cast<BYTE>(levelWord & 0xFF);
		Description.LoadFromFile(file);
	}
}
