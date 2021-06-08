// stdafx.cpp : source file that includes just the standard includes
//	infLoad.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

bool 
FileOpenBinary(const char* filename, class ifstream& file)
{
	if (file.is_open())
	{
		file.close();
	}

	file.open(filename, ios::binary);
	
	if (!file.is_open())
	{
		/*
		DEBUG_ADD_FORMAT("[Error] File Open Error : [%s]", filename);
		*/
		char msg[300];
		sprintf(msg,"[Error] File Open Error : [%s]", filename);
		MessageBox(0,msg,"Error",MB_OK);
		return false;
	}

	/*
	DEBUG_ADD_FORMAT("File Open OK : [%s]", filename);
	*/
	return true;
}


