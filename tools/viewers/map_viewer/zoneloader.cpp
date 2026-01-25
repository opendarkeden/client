/**
 * @file zoneloader.cpp
 * @brief 简化的 Zone 文件加载器实现
 *
 * 直接从原始 MZone 代码提取必要的文件解析逻辑
 */

#include "zoneloader.h"
#include <cstring>
#include <iostream>

ZoneLoader::ZoneLoader()
    : m_width(0)
    , m_height(0)
    , m_sectors(nullptr)
{
}

ZoneLoader::~ZoneLoader() {
    Release();
}

void ZoneLoader::Release() {
    if (m_sectors) {
        delete[] m_sectors;
        m_sectors = nullptr;
    }
    m_width = 0;
    m_height = 0;
}

std::string ZoneLoader::ReadMString(std::ifstream& file) {
    // MString 格式（从 MString.cpp 提取）：
    // 1. 读取 4 字节长度
    // 2. 读取长度字节的字符串内容
    uint32_t len;
    file.read(reinterpret_cast<char*>(&len), sizeof(uint32_t));

    if (len == 0 || len > 10240) { // 限制最大长度防止错误
        return "";
    }

    std::string str(len, '\0');
    file.read(&str[0], len);
    return str;
}

bool ZoneLoader::LoadFromFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open zone file: " << filename << std::endl;
        return false;
    }

    // 释放旧数据
    Release();

    // 读取 Zone 信息头（从 FILEINFO_ZONE_HEADER::LoadFromFile 提取）
    m_info.version = ReadMString(file);
    file.read(reinterpret_cast<char*>(&m_info.zoneID), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&m_info.zoneGroupID), sizeof(uint16_t));
    m_info.zoneName = ReadMString(file);
    file.read(reinterpret_cast<char*>(&m_info.zoneType), sizeof(uint8_t));
    file.read(reinterpret_cast<char*>(&m_info.zoneLevel), sizeof(uint8_t));
    m_info.description = ReadMString(file);

    std::cout << "Zone Version: " << m_info.version << std::endl;
    std::cout << "Zone Name: " << m_info.zoneName << std::endl;
    std::cout << "Zone ID: " << m_info.zoneID << ", Group ID: " << m_info.zoneGroupID << std::endl;
    std::cout << "Description: " << m_info.description << std::endl;

    // 检查版本（从 MZone::LoadFromFile 提取）
    // 实际版本字符串定义见 MZoneDef.h: #define MAP_VERSION_2000_05_10 "=MAP_2000_05_10="
    if (m_info.version != "=MAP_2000_05_10=" &&
        m_info.version != "=MAP_2000_05_10=H") { // H 变体用于某些地图
        std::cout << "Warning: Unknown zone version: " << m_info.version << std::endl;
        // 继续尝试加载，不要返回 false
    }

    // 读取 Tile FP 和 ImageObject FP（各 4 字节）
    uint32_t tileFP, imageObjectFP;
    file.read(reinterpret_cast<char*>(&tileFP), sizeof(uint32_t));
    file.read(reinterpret_cast<char*>(&imageObjectFP), sizeof(uint32_t));

    std::cout << "Tile FP: " << tileFP << ", ImageObject FP: " << imageObjectFP << std::endl;

    // 读取地图尺寸（从 MZone::LoadFromFile 第937-938行）
    file.read(reinterpret_cast<char*>(&m_width), sizeof(uint16_t));
    file.read(reinterpret_cast<char*>(&m_height), sizeof(uint16_t));

    std::cout << "Map size: " << m_width << "x" << m_height << " sectors" << std::endl;

    // 检查尺寸
    if (m_width == 0 || m_height == 0) {
        std::cerr << "Invalid map dimensions" << std::endl;
        return false;
    }

    if (m_width > 2000 || m_height > 2000) {
        std::cerr << "Warning: Map size seems too large: " << m_width << "x" << m_height << std::endl;
        return false;
    }

    // 分配 sector 数组
    m_sectors = new Sector[m_width * m_height];

    // 读取 sector 数据（从 MZone::LoadFromFile 第962-990行的注释代码提取）
    // Sector 格式：spriteID(2) + property(1) + light(1) = 4 bytes
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            Sector& sector = m_sectors[y * m_width + x];
            file.read(reinterpret_cast<char*>(&sector.spriteID), sizeof(uint16_t));
            file.read(reinterpret_cast<char*>(&sector.property), sizeof(uint8_t));
            file.read(reinterpret_cast<char*>(&sector.light), sizeof(uint8_t));
        }
    }

    std::cout << "Loaded " << (m_width * m_height) << " sectors" << std::endl;

    // 统计有效 sprite
    int validSprites = 0;
    uint16_t minSpriteID = 0xFFFF;
    uint16_t maxSpriteID = 0;
    for (int i = 0; i < m_width * m_height; i++) {
        if (m_sectors[i].spriteID > 0) {
            validSprites++;
            if (m_sectors[i].spriteID < minSpriteID) minSpriteID = m_sectors[i].spriteID;
            if (m_sectors[i].spriteID > maxSpriteID) maxSpriteID = m_sectors[i].spriteID;
        }
    }

    std::cout << "Valid sprites: " << validSprites << std::endl;
    std::cout << "SpriteID range: " << minSpriteID << " to " << maxSpriteID << std::endl;

    file.close();

    return true;
}

const Sector* ZoneLoader::GetSector(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return nullptr;
    }
    return &m_sectors[y * m_width + x];
}
