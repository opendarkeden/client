// RarFile.cpp: implementation of the CRarFile class.
// Modified for cross-platform support without RAR dependency
//////////////////////////////////////////////////////////////////////

#include "RarFile.h"
#pragma warning(disable:4786)
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Error Reporting Macro (cross-platform)
//////////////////////////////////////////////////////////////////////
#ifdef PLATFORM_WINDOWS
	#define RARFILE_ERROR(msg) { \
		OutputDebugStringA("[RARFile ERROR] "); \
		OutputDebugStringA(msg); \
		OutputDebugStringA("\n"); \
	}
#else
	#define RARFILE_ERROR(msg) { \
		fprintf(stderr, "[RARFile ERROR] %s\n", msg); \
		fflush(stderr); \
	}
#endif

//////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////
CRarFile::CRarFile()
{
	m_file_pointer = NULL;
	m_data = NULL;
	m_size = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////
CRarFile::CRarFile(const char *rar_filename, const char *pass)
{
	m_file_pointer = NULL;
	m_data = NULL;
	m_size = 0;
	SetRAR(rar_filename, pass);
}

//////////////////////////////////////////////////////////////////////
// Destruction
//////////////////////////////////////////////////////////////////////
CRarFile::~CRarFile()
{
	Release();
}

//////////////////////////////////////////////////////////////////////
// Release
//////////////////////////////////////////////////////////////////////
void CRarFile::Release()
{
	if(m_data != NULL){
		free(m_data);
		m_data = NULL;
		m_file_pointer = NULL;
	}
	m_size = 0;
}

//////////////////////////////////////////////////////////////////////
// SetRAR
// Convert RAR file path to directory path
//////////////////////////////////////////////////////////////////////
void CRarFile::SetRAR(const char *rar_filename, const char *pass)
{
	if (rar_filename == NULL || rar_filename[0] == '\0')
	{
		m_rar_filename = "";
		m_base_dir = "";
		m_password = "";
		return;
	}

	m_rar_filename = rar_filename;
	m_password = pass;  // Store password but don't use it (not needed for extracted files)

	// Convert RAR file path to directory path
	// Example: "Data/Ui/txt/Item.rpk" → "Data/Ui/txt/Item/"
	m_base_dir = rar_filename;

	// Remove .rpk or .rar extension (case-insensitive)
	size_t len = m_base_dir.length();
	if (len > 4) {
		std::string ext = m_base_dir.substr(len - 4);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if (ext == ".rpk" || ext == ".rar") {
			m_base_dir = m_base_dir.substr(0, len - 4);
		}
	}

	// Append trailing slash if not present
	len = m_base_dir.length();
	if (len > 0) {
		char lastChar = m_base_dir[len - 1];
		if (lastChar != '/' && lastChar != '\\') {
			m_base_dir += "/";
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Open
// Open a file from the extracted directory
//////////////////////////////////////////////////////////////////////
bool CRarFile::Open(const char *in_filename)
{
	if (in_filename == NULL || in_filename[0] == '\0')
	{
		RARFILE_ERROR("Open called with NULL or empty filename");
		return false;
	}

	Release();

	// Build full path by combining base directory with filename
	std::string fullPath = m_base_dir + in_filename;

	// Open the file
	FILE* file = fopen(fullPath.c_str(), "rb");
	if (file == NULL)
	{
		// Log detailed error information
		char errorMsg[512];
		snprintf(errorMsg, sizeof(errorMsg),
				"Failed to open file: %s (base_dir=%s, filename=%s)",
				fullPath.c_str(), m_base_dir.c_str(), in_filename);
		RARFILE_ERROR(errorMsg);
		return false;
	}

	// Get file size
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (fileSize <= 0)
	{
		char errorMsg[256];
		snprintf(errorMsg, sizeof(errorMsg),
				"File has invalid size: %s (size=%ld)", fullPath.c_str(), fileSize);
		RARFILE_ERROR(errorMsg);
		fclose(file);
		return false;
	}

	// Allocate buffer and read entire file
	m_data = (char*)malloc(fileSize + 1);
	if (m_data == NULL)
	{
		RARFILE_ERROR("Memory allocation failed for file data");
		fclose(file);
		return false;
	}

	size_t bytesRead = fread(m_data, 1, fileSize, file);
	fclose(file);

	if (bytesRead != (size_t)fileSize) {
		char errorMsg[256];
		snprintf(errorMsg, sizeof(errorMsg),
				"Read size mismatch: %s (expected=%ld, actual=%zu)",
				fullPath.c_str(), fileSize, bytesRead);
		RARFILE_ERROR(errorMsg);
		free(m_data);
		m_data = NULL;
		return false;
	}

	m_size = (int)bytesRead;
	m_data[m_size] = '\0';  // Null-terminate for string operations
	m_file_pointer = m_data;

	return true;
}

//////////////////////////////////////////////////////////////////////
// Read
// Copy data to buffer
//////////////////////////////////////////////////////////////////////
char* CRarFile::Read(char *buf, int size)
{
	if(m_file_pointer == NULL || IsEOF())
		return NULL;

	memcpy(buf, m_file_pointer, size);
	char* re = m_data;
	m_file_pointer += size;
	return re;
}

//////////////////////////////////////////////////////////////////////
// Read
// Advance file pointer by size
//////////////////////////////////////////////////////////////////////
char* CRarFile::Read(int size)
{
	if(m_file_pointer == NULL || IsEOF())
		return NULL;

	char* re = (char*)m_file_pointer;
	m_file_pointer += size;
	return re;
}

//////////////////////////////////////////////////////////////////////
// GetString
// Read one line from the current file pointer position
//////////////////////////////////////////////////////////////////////
bool CRarFile::GetString(char* buf, int size)
{
	if (buf == NULL || size <= 0)
		return false;

	if (m_file_pointer == NULL || IsEOF())
	{
		buf[0] = '\0';
		return false;
	}

	// Find current position in data
	long currentPos = m_file_pointer - m_data;
	if (currentPos >= m_size)
	{
		buf[0] = '\0';
		return false;
	}

	// Find newline character
	char* lineStart = m_file_pointer;
	char* newline = (char*)memchr(lineStart, '\n', m_size - currentPos);

	int lineLength;
	if (newline != NULL)
	{
		// Found newline - calculate line length
		lineLength = (int)(newline - lineStart);

		// Skip the newline for next call
		m_file_pointer = newline + 1;
	}
	else
	{
		// No newline found - read to end
		lineLength = (int)(m_size - currentPos);
		m_file_pointer = m_data + m_size;
	}

	// Copy line to buffer (respect buffer size)
	int copyLength = lineLength;
	if (copyLength >= size)
		copyLength = size - 1;

	memcpy(buf, lineStart, copyLength);
	buf[copyLength] = '\0';

	// Remove trailing \r if present (Windows CRLF files)
	if (copyLength > 0 && buf[copyLength - 1] == '\r')
	{
		buf[copyLength - 1] = '\0';
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// IsEOF
//////////////////////////////////////////////////////////////////////
bool CRarFile::IsEOF(int plus)
{
	if (m_file_pointer == NULL)
		return true;

	long currentPos = m_file_pointer - m_data;
	return (currentPos + plus >= m_size);
}

//////////////////////////////////////////////////////////////////////
// GetList
// Stub implementation for compatibility
//////////////////////////////////////////////////////////////////////
std::vector<std::string> *CRarFile::GetList(char *filter)
{
	// Stub: Return empty list
	static std::vector<std::string> emptyList;
	return &emptyList;
}
