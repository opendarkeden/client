/*-----------------------------------------------------------------------------

	Directory.cpp

	Cross-platform implementation of directory utilities.
	Uses platform abstraction layer.

	2025.01.14

-----------------------------------------------------------------------------*/

#include "Directory.h"
#include "Platform.h"
#include <string.h>
#include <stdlib.h>

/* Global instance */
C_DIRECTORY gC_directory;

/* ============================================================================
 * Constructor/Destructor
 * ============================================================================ */

C_DIRECTORY::C_DIRECTORY()
{
	m_sz_program_directory = NULL;
	m_sz_mixed_directory = NULL;

	/* Get executable directory */
	char dir[512];
	if (platform_get_executable_dir(dir, sizeof(dir)) == 0) {
		m_sz_program_directory = new char[strlen(dir) + 1];
		strcpy(m_sz_program_directory, dir);
	}
}

C_DIRECTORY::~C_DIRECTORY()
{
	if (m_sz_program_directory != NULL) {
		delete[] m_sz_program_directory;
		m_sz_program_directory = NULL;
	}

	if (m_sz_mixed_directory != NULL) {
		delete[] m_sz_mixed_directory;
		m_sz_mixed_directory = NULL;
	}
}

/* ============================================================================
 * Get Functions
 * ============================================================================ */

char* C_DIRECTORY::GetMixedPathWidthProgramDir(const char* name) const
{
	if (m_sz_program_directory == NULL) return NULL;

	/* Allocate buffer for full path */
	size_t len = strlen(m_sz_program_directory) + strlen(name) + 1;
	char* result = new char[len];
	strcpy(result, m_sz_program_directory);
	strcat(result, name);

	return result;
}

char* C_DIRECTORY::GetProgramDirectory() const
{
	return m_sz_program_directory;
}

char* C_DIRECTORY::GetMixedDirectory(const char* path, const char* filename) const
{
	/* Allocate buffer for combined path */
	size_t len = strlen(path) + strlen(filename) + 2;
	char* result = new char[len];
	strcpy(result, path);

	/* Add path separator if needed */
	size_t pathLen = strlen(path);
	if (pathLen > 0 && path[pathLen - 1] != platform_get_path_separator()) {
		strcat(result, platform_get_path_separator() == '\\' ? "\\" : "/");
	}

	strcat(result, filename);

	return result;
}

/* ============================================================================
 * Test Functions
 * ============================================================================ */

bool C_DIRECTORY::ExistFile(const char* fullpath) const
{
	return platform_file_exists(fullpath) ? true : false;
}
