/**
 * @file mzone_tileprovider.h
 * @brief MZone to ITileDataProvider adapter
 *
 * 使用原始游戏代码 MZone 来实现 ITileDataProvider 接口
 * 避免 ZoneLoader 自定义实现的潜在问题
 */

#ifndef __MZONE_TILEPROVIDER_H__
#define __MZONE_TILEPROVIDER_H__

#include "Client/MZone.h"
#include "Client/TileRenderer.h"  // For ITileDataProvider interface

/**
 * MZoneTileProvider - 让 MZone 实现 ITileDataProvider 接口
 *
 * 使用原始游戏代码，确保所有地图都能正确加载
 */
class MZoneTileProvider : public ITileDataProvider {
public:
    MZoneTileProvider();
    ~MZoneTileProvider();

    // 加载 zone 文件（使用原始 MZone::LoadFromFile）
    bool LoadFromFile(const char* filename);

    // 释放资源
    void Release();

    // ITileDataProvider interface implementation
    virtual int GetSpriteID(int sectorX, int sectorY) override
    {
        if (!IsInBounds(sectorX, sectorY))
            return -1;  // Out of bounds = empty

        const MSector& sector = m_pZone->GetSector(sectorX, sectorY);
        TYPE_SPRITEID spriteID = sector.GetSpriteID();

        // spriteID=0 or SPRITEID_NULL means no tile
        if (spriteID == 0 || spriteID == SPRITEID_NULL)
            return -1;

        return spriteID;
    }

    virtual int GetWidth() const override
    {
        if (m_pZone == nullptr)
            return 0;
        return m_pZone->GetWidth();
    }

    virtual int GetHeight() const override
    {
        if (m_pZone == nullptr)
            return 0;
        return m_pZone->GetHeight();
    }

    // 获取原始 MZone 指针（用于访问其他信息）
    MZone* GetZone() { return m_pZone; }
    const MZone* GetZone() const { return m_pZone; }

private:
    bool IsInBounds(int x, int y) const
    {
        if (m_pZone == nullptr)
            return false;
        return (x >= 0 && y >= 0 && x < m_pZone->GetWidth() && y < m_pZone->GetHeight());
    }

private:
    MZone* m_pZone;
    // 需要保持文件句柄打开，因为 MZone::LoadFromFile 需要引用
    std::ifstream* m_pFile;
};

#endif // __MZONE_TILEPROVIDER_H__
