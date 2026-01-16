#include "../Client_PCH.h"
#include "CRarFile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CRarFile::CRarFile()
{
    m_pFile = NULL;
    m_pString = NULL;
    m_StringLen = 0;
    m_Filename[0] = '\0';
}

CRarFile::CRarFile(const char* pRarFileName, const char* pPassword)
{
    m_pFile = NULL;
    m_pString = NULL;
    m_StringLen = 0;
    m_Filename[0] = '\0';
    SetRAR(pRarFileName, pPassword);
}

CRarFile::~CRarFile()
{
    Release();
}

void CRarFile::Release()
{
    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }

    if (m_pString != NULL)
    {
        delete[] m_pString;
        m_pString = NULL;
    }

    m_StringLen = 0;
    m_Filename[0] = '\0';
}

void CRarFile::SetRAR(const char* pRarFileName, const char* pPassword)
{
    // Stub: Just store the filename
    if (pRarFileName != NULL)
    {
        strncpy(m_Filename, pRarFileName, sizeof(m_Filename) - 1);
        m_Filename[sizeof(m_Filename) - 1] = '\0';
    }
}

bool CRarFile::Open(const char* pFilename)
{
    // Try to open the file directly
    if (m_pFile != NULL)
    {
        fclose(m_pFile);
    }

    m_pFile = fopen(pFilename, "rb");
    return (m_pFile != NULL);
}

int CRarFile::Read(int nBytesToRead)
{
    if (m_pFile == NULL || m_pString == NULL)
    {
        return 0;
    }

    int nBytesToReadActual = nBytesToRead;
    if (nBytesToReadActual > m_StringLen)
    {
        nBytesToReadActual = m_StringLen;
    }

    size_t nRead = fread(m_pString, 1, nBytesToReadActual, m_pFile);
    return (int)nRead;
}

void CRarFile::GetString(char* pBuffer, int nBufferSize)
{
    if (pBuffer == NULL || nBufferSize <= 0)
    {
        return;
    }

    if (m_pString != NULL && m_StringLen > 0)
    {
        int nCopy = m_StringLen;
        if (nCopy >= nBufferSize)
        {
            nCopy = nBufferSize - 1;
        }
        memcpy(pBuffer, m_pString, nCopy);
        pBuffer[nCopy] = '\0';
    }
    else
    {
        pBuffer[0] = '\0';
    }
}

char* CRarFile::GetList(char* pBuffer)
{
    // Stub: Return empty list
    if (pBuffer != NULL)
    {
        pBuffer[0] = '\0';
    }
    return pBuffer;
}
