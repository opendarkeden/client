/**
 * @file zone_parser.cpp
 * @brief Zone Parser - 解析并打印 .map zone 文件信息
 *
 * 功能：
 * - 直接解析 .map 二进制文件
 * - 显示 zone 信息（header、sector 统计、objects）
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>

class ZoneParser {
public:
    ZoneParser()
        : m_zoneID(0)
        , m_zoneGroupID(0)
        , m_zoneType(0)
        , m_zoneLevel(0)
        , m_width(0)
        , m_height(0)
        , m_numImageObjects(0)
        , m_numObstacles(0)
    {
    }

    bool Parse(const char* mapFile) {
        std::ifstream file(mapFile, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << mapFile << std::endl;
            return false;
        }

        // Helper function to read MString format
        auto readMString = [&file]() -> std::string {
            uint32_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(uint32_t));
            if (len == 0) return "";

            std::string str(len, '\0');
            file.read(&str[0], len);
            return str;
        };

        // Read version string (MString format: 4 bytes length + string)
        m_zoneVersion = readMString();

        // Read zone header fields
        file.read(reinterpret_cast<char*>(&m_zoneID), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&m_zoneGroupID), sizeof(uint16_t));

        // Read zone name (MString format)
        m_zoneName = readMString();

        // Read zone type and level
        file.read(reinterpret_cast<char*>(&m_zoneType), sizeof(uint8_t));
        file.read(reinterpret_cast<char*>(&m_zoneLevel), sizeof(uint8_t));

        // Read description (MString format)
        m_description = readMString();

        // IMPORTANT: Skip Tile FP and ImageObject FP (4 bytes each)
        file.seekg(8, std::ios::cur);

        // Read dimensions
        file.read(reinterpret_cast<char*>(&m_width), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&m_height), sizeof(uint16_t));

        m_numSectors = m_width * m_height;

        // Skip sector data for now
        size_t sectorDataSize = m_numSectors * 4; // 4 bytes per sector
        file.seekg(sectorDataSize, std::ios::cur);

        // Read ImageObject count
        file.read(reinterpret_cast<char*>(&m_numImageObjects), sizeof(uint32_t));

        // Read Obstacle count
        file.read(reinterpret_cast<char*>(&m_numObstacles), sizeof(uint32_t));

        file.close();

        return !file.fail();
    }

    void PrintInfo() {
        std::cout << "========================================" << std::endl;
        std::cout << "Zone File Information" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;

        std::cout << "Header:" << std::endl;
        std::cout << "  Version:     " << m_zoneVersion << std::endl;
        std::cout << "  Zone ID:     " << m_zoneID << std::endl;
        std::cout << "  Group ID:    " << m_zoneGroupID << std::endl;
        std::cout << "  Name:        " << m_zoneName << std::endl;
        std::cout << "  Type:        " << GetZoneTypeName(m_zoneType) << " (" << (int)m_zoneType << ")" << std::endl;
        std::cout << "  Level:       " << (int)m_zoneLevel << std::endl;
        std::cout << "  Description: " << m_description << std::endl;
        std::cout << std::endl;

        std::cout << "Dimensions:" << std::endl;
        std::cout << "  Width:       " << m_width << " sectors" << std::endl;
        std::cout << "  Height:      " << m_height << " sectors" << std::endl;
        std::cout << "  Total:       " << m_numSectors << " sectors" << std::endl;
        std::cout << std::endl;

        std::cout << "Objects:" << std::endl;
        std::cout << "  ImageObjects: " << m_numImageObjects << std::endl;
        std::cout << "  Obstacles:    " << m_numObstacles << std::endl;
        std::cout << std::endl;

        std::cout << "Estimated file size breakdown:" << std::endl;
        std::cout << "  Header:       ~256 bytes" << std::endl;
        std::cout << "  Dimensions:   4 bytes" << std::endl;
        std::cout << "  Sectors:      " << (m_numSectors * 4) << " bytes (4 bytes per sector)" << std::endl;
        std::cout << "  Objects:      ~" << (m_numImageObjects * 16 + m_numObstacles * 12) << " bytes (estimated)" << std::endl;
        std::cout << std::endl;
    }

    void PrintSectorData(const char* mapFile, int maxSectors = 10) {
        std::ifstream file(mapFile, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << mapFile << std::endl;
            return;
        }

        // Helper function to read MString format
        auto readMString = [&file]() -> std::string {
            uint32_t len;
            file.read(reinterpret_cast<char*>(&len), sizeof(uint32_t));
            if (len == 0) return "";

            std::string str(len, '\0');
            file.read(&str[0], len);
            return str;
        };

        // Skip header
        readMString(); // version
        file.seekg(4, std::ios::cur); // zone ID + group ID
        readMString(); // name
        file.seekg(2, std::ios::cur); // type + level
        readMString(); // description
        file.seekg(8, std::ios::cur); // Tile FP + ImageObject FP

        // Read dimensions
        uint16_t width, height;
        file.read(reinterpret_cast<char*>(&width), sizeof(uint16_t));
        file.read(reinterpret_cast<char*>(&height), sizeof(uint16_t));

        std::cout << "========================================" << std::endl;
        std::cout << "Sector Data (first " << maxSectors << ")" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;

        // Read and display first few sectors
        int count = 0;
        for (int y = 0; y < height && count < maxSectors; y++) {
            for (int x = 0; x < width && count < maxSectors; x++) {
                uint16_t spriteID;
                uint8_t property;
                uint8_t light;

                file.read(reinterpret_cast<char*>(&spriteID), sizeof(uint16_t));
                file.read(reinterpret_cast<char*>(&property), sizeof(uint8_t));
                file.read(reinterpret_cast<char*>(&light), sizeof(uint8_t));

                std::cout << "Sector (" << x << ", " << y << "):" << std::endl;
                std::cout << "  SpriteID: " << spriteID << std::endl;
                std::cout << "  Property: 0x" << std::hex << (int)property << std::dec;
                std::cout << " (";
                if (property & 0x01) std::cout << "OBSTACLE ";
                if (property & 0x02) std::cout << "PORTAL ";
                if (property & 0x04) std::cout << "SPECIAL ";
                std::cout << ")" << std::endl;
                std::cout << "  Light:    " << (int)light << std::endl;
                std::cout << std::endl;

                count++;
            }
        }

        file.close();
    }

private:
    const char* GetZoneTypeName(uint8_t type) {
        switch (type) {
            case 0:  return "NORMAL";
            case 1:  return "SLAYER_GUILD";
            case 2:  return "RESERVED_SLAYER_GUILD";
            case 3:  return "NPC_VAMPIRE_LAIR";
            case 4:  return "PC_VAMPIRE_LAIR";
            case 5:  return "NPC_HOME";
            case 6:  return "NPC_SHOP";
            case 7:  return "RANDOMAP";
            default: return "UNKNOWN";
        }
    }

private:
    std::string m_zoneVersion;
    uint16_t m_zoneID;
    uint16_t m_zoneGroupID;
    std::string m_zoneName;
    uint8_t m_zoneType;
    uint8_t m_zoneLevel;
    std::string m_description;
    uint16_t m_width;
    uint16_t m_height;
    int m_numSectors;
    uint32_t m_numImageObjects;
    uint32_t m_numObstacles;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Zone Parser - C++ Edition\n";
        std::cout << "==========================\n\n";
        std::cout << "Usage: " << argv[0] << " <zone.map> [--sectors N]\n\n";
        std::cout << "Options:\n";
        std::cout << "  --sectors N    Show first N sectors (default: 10)\n\n";
        std::cout << "Example:\n";
        std::cout << "  " << argv[0] << " DarkEden/Data/Map/adam_c.map\n";
        std::cout << "  " << argv[0] << " DarkEden/Data/Map/adam_c.map --sectors 20\n";
        return 1;
    }

    const char* mapFile = argv[1];
    int showSectors = 10;

    // Parse optional arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--sectors") == 0 && (i + 1) < argc) {
            showSectors = atoi(argv[i + 1]);
            if (showSectors < 0) showSectors = 0;
            i++;
        }
    }

    std::cout << "Zone Parser - C++ Edition\n";
    std::cout << "==========================\n\n";
    std::cout << "Parsing: " << mapFile << std::endl;
    std::cout << std::endl;

    ZoneParser parser;
    if (!parser.Parse(mapFile)) {
        std::cerr << "Failed to parse zone file" << std::endl;
        return 1;
    }

    parser.PrintInfo();

    if (showSectors > 0) {
        parser.PrintSectorData(mapFile, showSectors);
    }

    return 0;
}
