/**
 * @file mzone_tileprovider.cpp
 * @brief MZone to ITileDataProvider adapter implementation
 */

#include "mzone_tileprovider.h"
#include <fstream>

MZoneTileProvider::MZoneTileProvider()
    : m_pZone(nullptr)
    , m_pFile(nullptr)
{
}

MZoneTileProvider::~MZoneTileProvider()
{
    Release();
}

bool MZoneTileProvider::LoadFromFile(const char* filename)
{
    // Release any previous resources
    Release();

    // Create new zone
    m_pZone = new MZone();
    if (m_pZone == nullptr)
    {
        return false;
    }

    // Open file
    m_pFile = new std::ifstream(filename, std::ios::binary);
    if (!m_pFile->is_open())
    {
        delete m_pZone;
        m_pZone = nullptr;
        delete m_pFile;
        m_pFile = nullptr;
        return false;
    }

    // Load using original MZone code
    if (!m_pZone->LoadFromFile(*m_pFile))
    {
        delete m_pZone;
        m_pZone = nullptr;
        delete m_pFile;
        m_pFile = nullptr;
        return false;
    }

    // Keep file open for potential future operations
    // (MZone may need it for sound loading, etc.)

    return true;
}

void MZoneTileProvider::Release()
{
    if (m_pZone != nullptr)
    {
        m_pZone->Release();
        delete m_pZone;
        m_pZone = nullptr;
    }

    if (m_pFile != nullptr)
    {
        if (m_pFile->is_open())
        {
            m_pFile->close();
        }
        delete m_pFile;
        m_pFile = nullptr;
    }
}
