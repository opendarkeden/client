// RarFile.h: interface for the CRarFile class.
// Modified for cross-platform support without RAR dependency
//////////////////////////////////////////////////////////////////////

#ifndef _RAR_FILE_HEADER_
#define _RAR_FILE_HEADER_

#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
#include "../../basic/Platform.h"
#endif
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * CRarFile - Cross-platform file reader that works with extracted RAR content
 *
 * RAR files are mapped to directories with the same name:
 * - Data/Info/infodata.rpk  → Data/Info/infodata/
 * - Data/Ui/txt/Item.rpk    → Data/Ui/txt/Item/
 *
 * This avoids dependency on unrar library and improves cross-platform compatibility
 */

class CRarFile
{
private:
	std::string m_rar_filename;
	std::string m_password;
	std::string m_base_dir;      // Extracted directory path

	char *m_data;
	char *m_file_pointer;
	int m_size;

public:
	// Constructor
	CRarFile();
	CRarFile(const char *rar_filename, const char *pass);

	// Destructor
	~CRarFile();

	// Release resources
	void Release();

	// Set RAR file path (converted to directory path)
	void SetRAR(const char *rar_filename, const char *pass);

	// Open a file from the extracted directory
	bool Open(const char *in_filename);

	// Read data
	char*	Read(char *buf, int size);
	char*	Read(int size);
	bool	GetString(char* buf, int size);

	// Check if file is ready
	bool	IsSet()	{ return (m_data != NULL); }

	// Check if EOF
	bool	IsEOF(int plus = 0);

	// Get file list (stub for compatibility)
	std::vector<std::string> *GetList(char *filter = NULL);

	char* GetFilePointer(){return m_file_pointer;};
};

#endif
