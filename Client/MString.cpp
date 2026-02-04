//--------------------------------------------------------------------------
// MString.cpp
//--------------------------------------------------------------------------

#include "Client_PCH.h"
#include <stdarg.h>
#include "MString.h"
#include "DebugLog.h"

#ifdef PLATFORM_MACOS
#include <iconv.h>
#endif

// Emscripten has iconv in its sysroot
#ifdef __EMSCRIPTEN__
#include <iconv.h>
#endif

// Forward declaration
static char* ConvertGBKToUTF8(const char* gbkStr, size_t gbkLen, size_t& outLen);

//#include "DebugInfo.h"
//#define	new			DEBUG_NEW
//#define	delete		DEBUG_DELETE

//--------------------------------------------------------------------------
// static
//--------------------------------------------------------------------------
char MString::s_pBuffer[MAX_BUFFER_LENGTH];

//--------------------------------------------------------------------------
//
// constructor / destructor
//
//--------------------------------------------------------------------------
MString::MString()
{
	m_Length = 0;
	m_pString = NULL;
}

MString::MString(const MString& str)
{
	m_Length = 0;
	m_pString = NULL;
	*this = str;
}

MString::MString(const char* str)
{
	m_Length = 0;
	m_pString = NULL;
	*this = str;
}

MString::~MString()
{
	if (m_pString!=NULL)
	{
		delete [] m_pString;		
	}		
}

//--------------------------------------------------------------------------
//
// member functions
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Init( len )
//--------------------------------------------------------------------------
// size만큼 memory확보
//--------------------------------------------------------------------------
void	
MString::Init(int len)
{
	Release();

	m_Length = 0;
	m_pString = new char [len + 1];
	m_pString[0] = NULL;
}

//--------------------------------------------------------------------------
// Relase
//--------------------------------------------------------------------------
// memory에서 제거
//--------------------------------------------------------------------------
void	
MString::Release()
{
	if (m_pString!=NULL)
	{
		delete [] m_pString;
		m_pString = NULL;
		m_Length = 0;
	}
}

//--------------------------------------------------------------------------
// Assign operator =
//--------------------------------------------------------------------------
void	
MString::operator = (const char* str)
{
	if (m_pString!=NULL)
	{
		delete [] m_pString;
		m_pString = NULL;
	}

	if (str==NULL)
	{
		m_Length = 0;
	}
	else
	{
		m_Length = strlen(str);

		if (m_Length!=0)
		{
			m_pString = new char [m_Length + 1];
			strcpy( m_pString, str );
		}
	}
}

//--------------------------------------------------------------------------
// Assign operator =
//--------------------------------------------------------------------------
void
MString::operator = (const MString& str)
{
	//--------------------------------
	// 길이가 0인 경우..
	//--------------------------------
	if (str.m_Length==0)
	{
		if (m_pString!=NULL)
		{
			delete [] m_pString;			
			m_pString	= NULL;
			m_Length	= 0;			
		}		
	}
	//--------------------------------
	// 길이가 0 이상인 경우...
	//--------------------------------
	else
	{
		if (m_pString!=NULL)
		{
			delete [] m_pString;
		}			
		
		m_Length = str.m_Length;
		m_pString = new char [m_Length + 1];
		
		strcpy(m_pString, str.m_pString);
	}
}

//--------------------------------------------------------------------------
// Format
//--------------------------------------------------------------------------
// 적절한 형식으로 string을 만든다.
//--------------------------------------------------------------------------
void
MString::Format(const char* format, ...)
{
	va_list		vl;

    va_start(vl, format);
    vsprintf(s_pBuffer, format, vl);
    va_end(vl);

	*this = s_pBuffer;
}

//--------------------------------------------------------------------------
// Save To File
//--------------------------------------------------------------------------
void		
MString::SaveToFile(std::ofstream& file)
{
	file.write((const char*)&m_Length, 4);

	// length가 0이 아닌 경우에만..
	if (m_Length!=0)
	{
		file.write((const char*)m_pString, static_cast<int>(m_Length));
	}
}

//--------------------------------------------------------------------------
// Load From File
//--------------------------------------------------------------------------
void
MString::LoadFromFile(std::ifstream& file)
{
	if (m_pString!=NULL)
	{
		delete [] m_pString;
		m_pString = NULL;
	}

	file.read((char*)&m_Length, 4);

	// Sanity check: reject obviously corrupted lengths
	// Maximum reasonable string length is 64KB
	const size_t MAX_STRING_LENGTH = 65536;
	if (m_Length > MAX_STRING_LENGTH)
	{
		// Corrupted length - treat as empty string
		DEBUG_ADD_FORMAT("MString: Corrupted length %zu, treating as empty string", m_Length);
		m_Length = 0;
		m_pString = new char[1];
		m_pString[0] = '\0';
		return;
	}

	// len이 0이 아닌 경우에만...
	if (m_Length!=0)
	{
		char* pTemp = new char [m_Length + 1];
		file.read((char*)pTemp, static_cast<int>(m_Length));
		pTemp[m_Length] = '\0';

		// Convert from GBK to UTF-8 at runtime
		// Resource files are in GBK encoding, convert to UTF-8
		size_t convertedLen = 0;
		char* pConverted = ConvertGBKToUTF8(pTemp, m_Length, convertedLen);
		delete [] pTemp;

		// Update m_Length to the converted length
		m_Length = convertedLen;
		m_pString = pConverted;
	}
	else
	{
		// Empty string - allocate a buffer for it
		m_pString = new char[1];
		m_pString[0] = '\0';
	}
}

//--------------------------------------------------------------------------
// Convert to UTF-8 if needed (internal helper)
// NOTE: Auto-conversion disabled - resource files should be pre-converted to UTF-8
//       The conversion functions are kept here for reference/future use if needed.
//--------------------------------------------------------------------------
namespace {
	// Check if string is valid UTF-8 (used for validation)
	bool IsValidUtf8(const char* str, size_t len)
	{
		for (size_t i = 0; i < len; )
		{
			unsigned char c = str[i];
			int seqLen;

			if (c < 0x80)
			{
				seqLen = 1;
			}
			else if ((c >> 5) == 0x6)
			{
				seqLen = 2;
			}
			else if ((c >> 4) == 0xE)
			{
				seqLen = 3;
			}
			else if ((c >> 3) == 0x1E)
			{
				seqLen = 4;
			}
			else
			{
				return false;
			}

			if (i + seqLen > len)
				return false;

			for (int j = 1; j < seqLen; j++)
			{
				if ((str[i + j] & 0xC0) != 0x80)
					return false;
			}

			i += seqLen;
		}
		return true;
	}
}

// Convert GBK to UTF-8 - used by LoadFromFile for runtime conversion
static char* ConvertGBKToUTF8(const char* gbkStr, size_t gbkLen, size_t& outLen)
{
#ifdef PLATFORM_MACOS
	// Always try GBK conversion - don't skip even if it looks like valid UTF-8
	// because GBK strings can sometimes pass UTF-8 validation (especially ASCII)

	iconv_t cd = iconv_open("UTF-8", "GBK");
	if (cd != (iconv_t)-1)
	{
		size_t inLen = gbkLen;
		size_t outBufLen = gbkLen * 4;  // Allocate enough space
		char* inBuf = const_cast<char*>(gbkStr);
		char* outBuf = new char[outBufLen];
		char* outStart = outBuf;
		memset(outBuf, 0, outBufLen);

		size_t result = iconv(cd, &inBuf, &inLen, &outBuf, &outBufLen);
		iconv_close(cd);

		if (result != (size_t)-1 && inLen == 0)
		{
			// Conversion successful
			size_t convertedLen = outBuf - outStart;
			outStart[convertedLen] = '\0';  // Ensure null termination
			outLen = convertedLen;  // Return the actual converted length
			return outStart;
		}
		delete[] outStart;
	}

	// Fallback: just copy the string (no conversion or failed conversion)
	outLen = gbkLen;
	char* result = new char[gbkLen + 1];
	memcpy(result, gbkStr, gbkLen);
	result[gbkLen] = '\0';
	return result;
#else
	// Non-Mac platform: just copy (assume UTF-8 or ASCII)
	outLen = gbkLen;
	char* result = new char[gbkLen + 1];
	memcpy(result, gbkStr, gbkLen);
	result[gbkLen] = '\0';
	return result;
#endif
}

#if 1  // Enabled - runtime conversion from GBK to UTF-8
namespace {
	// Convert encoding using iconv
	std::string ConvertEncoding(const char* str, size_t len, const char* fromEncoding)
	{
#ifdef PLATFORM_MACOS
		iconv_t cd = iconv_open("UTF-8", fromEncoding);
		if (cd == (iconv_t)-1)
			return std::string(str, len);

		size_t inLen = len;
		size_t outLen = len * 4;
		char* inBuf = const_cast<char*>(str);
		char* outBuf = new char[outLen];
		char* outStart = outBuf;
		memset(outBuf, 0, outLen);

		size_t result = iconv(cd, &inBuf, &inLen, &outBuf, &outLen);
		iconv_close(cd);

		if (result == (size_t)-1)
		{
			delete[] outStart;
			return std::string(str, len);
		}

		std::string converted(outStart, outBuf - outStart);
		delete[] outStart;
		return converted;
#else
		return std::string(str, len);
#endif
	}
}

void MString::ConvertToUTF8IfNeeded()
{
	if (m_pString == NULL || m_Length == 0)
		return;

	// Check if already UTF-8
	if (IsValidUtf8(m_pString, m_Length))
		return;

	// Try GBK first (Chinese encoding)
	std::string converted = ConvertEncoding(m_pString, m_Length, "GBK");
	if (converted != std::string(m_pString, m_Length))
	{
		// Successfully converted from GBK
		DEBUG_ADD_FORMAT("MString: Converted from GBK: '%.*s' -> '%s'",
		         (int)m_Length, m_pString, converted.c_str());
		delete[] m_pString;
		m_Length = converted.size();
		m_pString = new char[m_Length + 1];
		memcpy(m_pString, converted.c_str(), m_Length);
		m_pString[m_Length] = '\0';
		return;
	}

	// Try CP949 as fallback (Korean encoding)
	converted = ConvertEncoding(m_pString, m_Length, "CP949");
	if (converted != std::string(m_pString, m_Length))
	{
		DEBUG_ADD_FORMAT("MString: Converted from CP949: '%.*s' -> '%s'",
		         (int)m_Length, m_pString, converted.c_str());
		delete[] m_pString;
		m_Length = converted.size();
		m_pString = new char[m_Length + 1];
		memcpy(m_pString, converted.c_str(), m_Length);
		m_pString[m_Length] = '\0';
		return;
	}

	// Try EUC-KR as another fallback
	converted = ConvertEncoding(m_pString, m_Length, "EUC-KR");
	if (converted != std::string(m_pString, m_Length))
	{
		DEBUG_ADD_FORMAT("MString: Converted from EUC-KR: '%.*s' -> '%s'",
		         (int)m_Length, m_pString, converted.c_str());
		delete[] m_pString;
		m_Length = converted.size();
		m_pString = new char[m_Length + 1];
		memcpy(m_pString, converted.c_str(), m_Length);
		m_pString[m_Length] = '\0';
	}
	else
	{
		// Conversion failed - log the problematic string
		DEBUG_ADD_FORMAT("MString: Failed to convert encoding: '%.*s' (len=%zu)",
		         (int)m_Length, m_pString, m_Length);
	}
}
#endif
