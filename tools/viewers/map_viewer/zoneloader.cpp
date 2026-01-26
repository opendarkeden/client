/**
 * @file zoneloader.cpp
 * @brief Zone 文件加载器实现 - 基于 engine/sprite/src/zone.c 的 C++ 封装
 */

// 必须首先包含 zone.h，在包含任何客户端头文件之前
#include "engine/sprite/include/zone.h"

#include "zoneloader.h"
#include <iostream>
#include <cstring>
#include <map>
#include <algorithm>

// SectorData 的转换方法
void SectorData::InitFrom(const Sector* s) {
    if (s) {
        spriteID = s->sprite_id;
        property = s->property;
        light = s->light;
    }
}

// ImageObjectData 的转换方法
void ImageObjectData::InitFrom(const ImageObject* obj) {
    if (obj) {
        type = obj->type;
        id = obj->id;
        sectorX = obj->sector_x;
        sectorY = obj->sector_y;
        imageObjectID = obj->image_object_id;
        spriteID = obj->sprite_id;
        pixelX = obj->pixel_x;
        pixelY = obj->pixel_y;
        viewpoint = obj->viewpoint;
        isAnimation = obj->is_animation;
        transFlags = obj->trans_flags;

        // 转换位置列表
        positions.clear();
        for (uint32_t i = 0; i < obj->position_count; i++) {
            positions.push_back({obj->positions[i].x, obj->positions[i].y});
        }
    }
}

// PIMPL 实现类
class ZoneLoader::Impl {
public:
    Impl() {
        memset(&m_zone, 0, sizeof(m_zone));
    }

    ~Impl() {
        Release();
    }

    void Release() {
        zone_free(&m_zone);
        memset(&m_zone, 0, sizeof(m_zone));
    }

    Zone* GetZone() { return &m_zone; }
    const Zone* GetZone() const { return &m_zone; }

private:
    ::Zone m_zone;
};

// ZoneLoader 实现
ZoneLoader::ZoneLoader()
    : m_impl(new Impl())
{
}

ZoneLoader::~ZoneLoader() {
    Release();
    delete m_impl;
}

void ZoneLoader::Release() {
    m_impl->Release();
    m_sectors.clear();
    m_imageObjects.clear();
    m_info = ZoneInfo();
}

void ZoneLoader::ConvertZoneInfo(const ZoneHeader* header) {
    if (header) {
        m_info.version = header->version;
        m_info.zoneID = header->zone_id;
        m_info.zoneGroupID = header->zone_group_id;
        m_info.zoneName = header->zone_name;
        m_info.zoneType = header->zone_type;
        m_info.zoneLevel = header->zone_level;
        m_info.description = header->description;
    }
}

bool ZoneLoader::LoadFromFile(const char* filename) {
    // 释放旧数据
    Release();

    // 使用 C API 加载 zone 文件
    Error* err = zone_load(filename, m_impl->GetZone());
    if (err != NULL) {
        std::cerr << "Failed to load zone file: " << filename << std::endl;
        std::cerr << "  Error code: " << err->code << std::endl;
        free(err);
        Release();
        return false;
    }

    const Zone* zone = m_impl->GetZone();

    // 转换 zone 信息
    ConvertZoneInfo(&zone->header);

    std::cout << "=== Zone loaded successfully ===" << std::endl;
    std::cout << "Version: " << m_info.version << std::endl;
    std::cout << "Name: " << m_info.zoneName << std::endl;
    std::cout << "ID: " << m_info.zoneID << ", Group ID: " << m_info.zoneGroupID << std::endl;
    std::cout << "Type: " << (int)m_info.zoneType << ", Level: " << (int)m_info.zoneLevel << std::endl;
    if (!m_info.description.empty()) {
        std::cout << "Description: " << m_info.description << std::endl;
    }
    std::cout << "Size: " << zone->width << "x" << zone->height << std::endl;
    std::cout << "Image Objects: " << zone->image_object_count << std::endl;

    // 转换 sector 数据到 C++ 封装
    if (zone->sectors) {
        uint32_t total = (uint32_t)zone->width * zone->height;
        m_sectors.reserve(total);
        for (uint32_t i = 0; i < total; i++) {
            SectorData data;
            data.InitFrom(&zone->sectors[i]);
            m_sectors.push_back(data);
        }
        std::cout << "Loaded " << m_sectors.size() << " sectors" << std::endl;
    }

    // 转换 ImageObject 数据到 C++ 封装
    if (zone->image_objects) {
        m_imageObjects.reserve(zone->image_object_count);
        for (uint32_t i = 0; i < zone->image_object_count; i++) {
            ImageObjectData data;
            data.InitFrom(&zone->image_objects[i]);
            m_imageObjects.push_back(data);
        }
        std::cout << "Loaded " << m_imageObjects.size() << " image objects" << std::endl;

        // 统计每种类型的对象数量
        std::map<int, int> typeCounts;
        std::map<int, int> validSpritesPerType;
        for (const auto& obj : m_imageObjects) {
            typeCounts[obj.type]++;
            if (obj.spriteID > 0) {
                validSpritesPerType[obj.type]++;
            }
        }

        std::cout << "\nObject type distribution:" << std::endl;
        for (const auto& entry : typeCounts) {
            int type = entry.first;
            int count = entry.second;
            int valid = validSpritesPerType[type];
            std::cout << "  Type " << type << ": " << count
                      << " objects (" << valid << " with spriteID>0)" << std::endl;
        }

        // 打印前 10 个对象的详细信息
        std::cout << "\n=== First 10 ImageObjects ===" << std::endl;
        size_t showCount = std::min(size_t(10), m_imageObjects.size());
        for (size_t i = 0; i < showCount; i++) {
            const auto& obj = m_imageObjects[i];
            // 计算从 sector 坐标应该得到的像素坐标
            int expectedPixelX = obj.sectorX * 48;
            int expectedPixelY = obj.sectorY * 24;
            int diffX = obj.pixelX - expectedPixelX;
            int diffY = obj.pixelY - expectedPixelY;

            std::cout << "  [" << i << "] Type " << (int)obj.type
                      << " | spriteID=" << obj.spriteID
                      << "\n     sector=(" << obj.sectorX << "," << obj.sectorY << ")"
                      << " -> expected pixel=(" << expectedPixelX << "," << expectedPixelY << ")"
                      << "\n     pixel=(" << obj.pixelX << "," << obj.pixelY << ")"
                      << " diff=(" << diffX << "," << diffY << ")"
                      << "\n     viewpoint=" << obj.viewpoint
                      << " | positions=" << obj.positions.size()
                      << std::endl;
        }
    }

    return true;
}

int ZoneLoader::GetWidth() const {
    return m_impl->GetZone()->width;
}

int ZoneLoader::GetHeight() const {
    return m_impl->GetZone()->height;
}

const SectorData* ZoneLoader::GetSector(int x, int y) const {
    const Zone* zone = m_impl->GetZone();
    if (x < 0 || x >= (int)zone->width || y < 0 || y >= (int)zone->height) {
        return nullptr;
    }
    return &m_sectors[y * zone->width + x];
}

const ImageObjectData* ZoneLoader::GetImageObject(int index) const {
    if (index < 0 || index >= (int)m_imageObjects.size()) {
        return nullptr;
    }
    return &m_imageObjects[index];
}

int ZoneLoader::GetSpriteID(int sectorX, int sectorY) const {
    const SectorData* sector = GetSector(sectorX, sectorY);
    if (!sector) return -1;
    // spriteID=0 或 0xFFFF 表示没有 tile
    if (sector->spriteID == 0 || sector->spriteID == 0xFFFF)
        return -1;
    return sector->spriteID;
}
