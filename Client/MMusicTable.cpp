//----------------------------------------------------------------------
// MMusicTable.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MMusicTable.h"

//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MUSIC_TABLE*	g_pMusicTable = NULL;

//----------------------------------------------------------------------
// Save
//----------------------------------------------------------------------
void			
MUSICTABLE_INFO::SaveToFile(std::ofstream& file)
{
	Filename.SaveToFile( file );
	FilenameWav.SaveToFile( file );
}

//----------------------------------------------------------------------
// Load
//----------------------------------------------------------------------
void			
MUSICTABLE_INFO::LoadFromFile(std::ifstream& file)
{
	Filename.LoadFromFile( file );
	FilenameWav.LoadFromFile( file );
}