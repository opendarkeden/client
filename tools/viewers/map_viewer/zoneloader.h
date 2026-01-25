/**
 * @file zoneloader.h
 * @brief 简化的 Zone 文件加载器
 *
 * 从原始 MZone 代码提取，只包含必要的文件解析功能
 * 不依赖 MZone、MSector 等复杂类
 * 实现 ITileDataProvider 接口以支持 TileRenderer
 */

#ifndef __ZONELOADER_H__
#define __ZONELOADER_H__

#include <string>
#include <cstdint>
#include <fstream>
#include "Client/TileRenderer.h"  // For ITileDataProvider interface

// Zone 文件头信息
struct ZoneInfo {
    std::string version;
    uint16_t zoneID;
    uint16_t zoneGroupID;
    std::string zoneName;
    uint8_t zoneType;
    uint8_t zoneLevel;
    std::string description;
};

// Sector 数据结构（与 MSector_st 一致）
struct Sector {
    uint16_t spriteID;
    uint8_t  property;
    uint8_t  light;
};

// ZoneLoader implements ITileDataProvider to work with TileRenderer
class ZoneLoader : public ITileDataProvider {
public:
    ZoneLoader();
    ~ZoneLoader();

    // 加载 zone 文件
    bool LoadFromFile(const char* filename);

    // 获取 zone 信息
    const ZoneInfo& GetInfo() const { return m_info; }

    // 获取地图尺寸
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    // 获取 sector 数据（只读）
    const Sector* GetSector(int x, int y) const;
    const Sector* GetSectors() const { return m_sectors; }

    // ITileDataProvider interface implementation
    virtual int GetSpriteID(int sectorX, int sectorY) override
    {
        const Sector* sector = GetSector(sectorX, sectorY);
        if (!sector) return -1;  // SPRITEID_NULL
        // spriteID=0 or 0xFFFF means no tile
        if (sector->spriteID == 0 || sector->spriteID == 0xFFFF)
            return -1;  // SPRITEID_NULL
        // Return spriteID directly (no conversion needed - matches game code)
        return sector->spriteID;
    }

    // 释放资源
    void Release();

private:
    // 读取 MString 格式
    std::string ReadMString(std::ifstream& file);

private:
    ZoneInfo m_info;
    int m_width;
    int m_height;
    Sector* m_sectors;
};

#endif // __ZONELOADER_H__
