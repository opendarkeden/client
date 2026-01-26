/**
 * @file zoneloader.h
 * @brief Zone 文件加载器 - 基于 engine/sprite/src/zone.c 的 C++ 封装
 *
 * 使用验证过的 C API (zone.h/zone.c) 来解析 zone 文件
 *
 * 重要：为了避免 Packet/Exception.h 中的 Error 类与 engine/sprite/include/error.h
 * 中的 Error 结构体冲突，zone.h 的包含只在 .cpp 文件中进行，头文件中只使用
 * 前置声明和 C++ 封装类型。
 */

#ifndef __ZONELOADER_H__
#define __ZONELOADER_H__

#include <string>
#include <cstdint>
#include <vector>
#include <map>

// 前置声明 C API 类型，避免包含 zone.h
typedef struct Zone Zone;
typedef struct ZoneHeader ZoneHeader;
typedef struct Sector Sector;
typedef struct ImageObject ImageObject;

#ifdef __cplusplus
extern "C" {
#endif

// 前置声明 C API 函数（不需要声明 Error，因为我们在 .cpp 中处理）
void zone_free(Zone* zone);

#ifdef __cplusplus
}
#endif

// Zone 文件头信息 (C++ 封装)
struct ZoneInfo {
    std::string version;
    uint16_t zoneID;
    uint16_t zoneGroupID;
    std::string zoneName;
    uint8_t zoneType;
    uint8_t zoneLevel;
    std::string description;
};

// Sector 数据结构 (C++ 封装)
struct SectorData {
    uint16_t spriteID;
    uint8_t  property;
    uint8_t  light;

    SectorData() : spriteID(0xFFFF), property(0), light(0) {}

    // 从 C API 的 Sector 转换 (在 .cpp 中实现)
    void InitFrom(const Sector* s);
};

// ImageObject 数据结构 (C++ 封装)
struct ImageObjectData {
    uint8_t type;                  // ImageObjectType
    uint32_t id;                   // Instance Object ID
    uint16_t sectorX;              // Sector X
    uint16_t sectorY;              // Sector Y
    uint32_t imageObjectID;        // Image class ID
    uint16_t spriteID;             // Sprite ID
    int32_t pixelX;                // Pixel X position
    int32_t pixelY;                // Pixel Y position
    uint16_t viewpoint;            // Viewpoint
    uint8_t isAnimation;           // Animation flag
    uint8_t transFlags;            // Transparency flags

    // Position list - sectors this object occupies
    std::vector<std::pair<uint16_t, uint16_t>> positions;

    ImageObjectData() : type(0), id(0), sectorX(0), sectorY(0),
                       imageObjectID(0), spriteID(0), pixelX(0), pixelY(0),
                       viewpoint(0), isAnimation(0), transFlags(0) {}

    // 从 C API 的 ImageObject 转换 (在 .cpp 中实现)
    void InitFrom(const ImageObject* obj);
};

/**
 * ZoneLoader - Zone 文件加载器
 *
 * 使用 engine/sprite/src/zone.c 的 C API 实现文件解析
 * 完全封装了 C API，避免 Error 类型冲突
 */
class ZoneLoader {
public:
    ZoneLoader();
    ~ZoneLoader();

    // 禁止拷贝
    ZoneLoader(const ZoneLoader&) = delete;
    ZoneLoader& operator=(const ZoneLoader&) = delete;

    /**
     * 加载 zone 文件
     * @param filename zone 文件路径
     * @return 成功返回 true，失败返回 false
     */
    bool LoadFromFile(const char* filename);

    /**
     * 获取 zone 信息
     */
    const ZoneInfo& GetInfo() const { return m_info; }

    /**
     * 获取地图尺寸
     */
    int GetWidth() const;
    int GetHeight() const;

    /**
     * 获取 sector 数据
     * @param x sector X 坐标
     * @param y sector Y 坐标
     * @return sector 数据指针，越界返回 nullptr
     */
    const SectorData* GetSector(int x, int y) const;

    /**
     * 获取所有 sector 数据
     */
    const std::vector<SectorData>& GetAllSectors() const { return m_sectors; }

    /**
     * 获取 ImageObject 数量
     */
    int GetImageObjectCount() const { return m_imageObjects.size(); }

    /**
     * 获取 ImageObject 数据
     * @param index 索引
     * @return ImageObject 数据指针，越界返回 nullptr
     */
    const ImageObjectData* GetImageObject(int index) const;

    /**
     * 获取所有 ImageObject 数据
     */
    const std::vector<ImageObjectData>& GetAllImageObjects() const { return m_imageObjects; }

    /**
     * 获取指定位置的 sprite ID
     * @param sectorX sector X 坐标
     * @param sectorY sector Y 坐标
     * @return sprite ID，无效返回 -1
     */
    int GetSpriteID(int sectorX, int sectorY) const;

    /**
     * 释放资源
     */
    void Release();

private:
    /**
     * 从 C API 的 ZoneHeader 转换到 C++ 的 ZoneInfo
     */
    void ConvertZoneInfo(const ZoneHeader* header);

private:
    class Impl;  // PIMPL 模式，隐藏 C API 依赖
    Impl* m_impl;

    ZoneInfo m_info;
    std::vector<SectorData> m_sectors;
    std::vector<ImageObjectData> m_imageObjects;
};

/**
 * ZoneLoaderDataProvider - 将 ZoneLoader 适配为 ITileDataProvider 接口
 *
 * 这个适配器类避免了在 ZoneLoader 头文件中包含 TileRenderer.h
 * 从而防止 Error 类型冲突
 */
class ZoneLoaderDataProvider {
public:
    explicit ZoneLoaderDataProvider(ZoneLoader* loader) : m_loader(loader) {}

    int GetSpriteID(int sectorX, int sectorY) const {
        return m_loader ? m_loader->GetSpriteID(sectorX, sectorY) : -1;
    }

    int GetWidth() const { return m_loader ? m_loader->GetWidth() : 0; }
    int GetHeight() const { return m_loader ? m_loader->GetHeight() : 0; }

private:
    ZoneLoader* m_loader;
};

#endif // __ZONELOADER_H__
