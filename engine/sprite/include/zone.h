/**
 * @file zone.h
 * @brief Zone (map) file parsing for Dark Eden
 * 
 * Parses .map zone files containing:
 * - Zone header (version, ID, name, type, etc.)
 * - Sector grid (tile sprite IDs, properties, light)
 * - ImageObjects (buildings, decorations)
 * 
 * Based on original MZone.cpp implementation.
 */

#ifndef ZONE_H
#define ZONE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Constants
 * ============================================================================ */

#define ZONE_VERSION_STRING "=MAP_2000_05_10="
#define ZONE_VERSION_LEN    16

#define ZONE_MAX_NAME_LEN   256
#define ZONE_MAX_DESC_LEN   1024

/* Sector property flags (from MSector.h) */
#define SECTOR_BLOCK_UNDERGROUND    0x01
#define SECTOR_BLOCK_GROUND         0x02
#define SECTOR_BLOCK_FLYING         0x04
#define SECTOR_BLOCK_ALL            0x07
#define SECTOR_ITEM                 0x08
#define SECTOR_UNDERGROUND_CREATURE 0x10
#define SECTOR_GROUND_CREATURE      0x20
#define SECTOR_FLYING_CREATURE      0x40
#define SECTOR_PORTAL               0x80

/* Zone types */
typedef enum ZoneType {
    ZONE_TYPE_NORMAL = 0,
    ZONE_TYPE_SLAYER_GUILD,
    ZONE_TYPE_RESERVED_SLAYER_GUILD,
    ZONE_TYPE_NPC_VAMPIRE_LAIR,
    ZONE_TYPE_PC_VAMPIRE_LAIR,
    ZONE_TYPE_NPC_HOME,
    ZONE_TYPE_NPC_SHOP,
    ZONE_TYPE_RANDOMAP
} ZoneType;

/* ImageObject types (match MObject::OBJECT_TYPE numeric values) */
typedef enum ImageObjectType {
    OBJECT_TYPE_OBJECT = 0,              /* Base */
    OBJECT_TYPE_CREATURE = 1,
    OBJECT_TYPE_ITEM = 2,
    OBJECT_TYPE_IMAGEOBJECT = 3,
    OBJECT_TYPE_SHADOWOBJECT = 4,
    OBJECT_TYPE_ANIMATIONOBJECT = 5,
    OBJECT_TYPE_SHADOWANIMATIONOBJECT = 6,
    OBJECT_TYPE_INTERACTIONOBJECT = 7,
    OBJECT_TYPE_PORTAL = 8,
    OBJECT_TYPE_EFFECT = 9
} ImageObjectType;

/* Error codes */
typedef enum ZoneError {
    ZONE_OK = 0,
    ZONE_ERR_IO = -1,
    ZONE_ERR_FORMAT = -2,
    ZONE_ERR_VERSION = -3,
    ZONE_ERR_NOMEM = -4,
    ZONE_ERR_PARAM = -5
} ZoneError;

/* ============================================================================
 * Data Structures
 * ============================================================================ */

/**
 * Zone file header information
 */
typedef struct ZoneHeader {
    char version[ZONE_VERSION_LEN + 1];  /* Version string */
    uint16_t zone_id;                     /* Zone ID */
    uint16_t zone_group_id;               /* Zone group ID */
    char zone_name[ZONE_MAX_NAME_LEN];    /* Zone name */
    uint8_t zone_type;                    /* Zone type (ZoneType enum) */
    uint8_t zone_level;                   /* Zone level (1-10) */
    char description[ZONE_MAX_DESC_LEN];  /* Zone description */
} ZoneHeader;

/**
 * Single sector (tile) data
 */
typedef struct Sector {
    uint16_t sprite_id;   /* Tile sprite ID (0xFFFF = empty) */
    uint8_t property;     /* Property flags */
    uint8_t light;        /* Light level */
} Sector;

/**
 * ImageObject position in sector grid
 */
typedef struct ImageObjectPosition {
    uint16_t x;
    uint16_t y;
} ImageObjectPosition;

/**
 * ImageObject data
 */
typedef struct ImageObject {
    uint8_t type;                  /* ImageObjectType */
    uint32_t id;                   /* Instance Object ID (MObject::m_ID) */
    uint16_t sector_x;             /* Sector X (MObject::m_X) */
    uint16_t sector_y;             /* Sector Y (MObject::m_Y) */
    uint32_t image_object_id;      /* Image class ID (MImageObject::m_ImageObjectID) */
    uint16_t sprite_id;            /* Sprite ID */
    int32_t pixel_x;               /* Pixel X position */
    int32_t pixel_y;               /* Pixel Y position */
    uint16_t viewpoint;            /* Viewpoint (SIZE_SECTORPOSITION) */
    uint8_t is_animation;          /* MImageObject::m_bAnimation */
    uint8_t trans_flags;           /* MImageObject::m_bTrans (bit flags) */
    
    /* Position list - sectors this object occupies */
    uint32_t position_count;
    ImageObjectPosition* positions;
    
    /* Type-specific data */
    union {
        struct {
            /* Original shadow object has no extra binary fields */
            uint16_t reserved;
        } shadow;
        struct {
            /* Map stores frame_id (TYPE_FRAMEID, 2 bytes) and max_frame (1 byte) */
            uint16_t frame_id;
            uint8_t  max_frame;
            /* Optional runtime fields from MAnimationObject */
            uint8_t  blt_type;
            uint8_t  direction;
            uint8_t  sound_frame;
            uint16_t sound_id;
            uint8_t  loop;
            uint32_t min_delay;
            uint32_t max_delay;
            uint8_t  start_hour;
            uint8_t  end_hour;
        } animation;
        struct {
            uint16_t interaction_type;
        } interaction;
    } extra;
} ImageObject;

/**
 * Complete zone data
 */
typedef struct Zone {
    ZoneHeader header;
    
    uint16_t width;               /* Zone width in sectors */
    uint16_t height;              /* Zone height in sectors */
    
    Sector* sectors;              /* Sector grid (height * width) */
    
    uint32_t image_object_count;
    ImageObject* image_objects;
    
    /* File positions for lazy loading */
    long fp_tile;                 /* File position of tile data */
    long fp_image_object;         /* File position of image objects */
} Zone;

/* ============================================================================
 * API Functions
 * ============================================================================ */

/**
 * Load zone from file path
 * @param path Path to .map file
 * @param zone Pointer to Zone structure to fill
 * @return ZONE_OK on success, error code on failure
 */
Error* zone_load(const char* path, Zone* zone);

/**
 * Load zone from file stream
 * @param file Open file stream
 * @param zone Pointer to Zone structure to fill
 * @return ZONE_OK on success, error code on failure
 */
Error* zone_load_from_file(FILE* file, Zone* zone);

/**
 * Load only zone header (fast, no sector data)
 * @param path Path to .map file
 * @param header Pointer to ZoneHeader to fill
 * @return ZONE_OK on success, error code on failure
 */
Error* zone_load_header(const char* path, ZoneHeader* header);

/**
 * Free zone resources
 * @param zone Pointer to Zone structure
 */
void zone_free(Zone* zone);

/**
 * Get sector at position
 * @param zone Zone pointer
 * @param x X coordinate
 * @param y Y coordinate
 * @return Pointer to sector, or NULL if out of bounds
 */
const Sector* zone_get_sector(const Zone* zone, uint16_t x, uint16_t y);

/**
 * Check if position is walkable for ground creatures
 * @param zone Zone pointer
 * @param x X coordinate
 * @param y Y coordinate
 * @return true if walkable
 */
bool zone_can_walk_ground(const Zone* zone, uint16_t x, uint16_t y);

/**
 * Check if position is walkable for flying creatures
 * @param zone Zone pointer
 * @param x X coordinate
 * @param y Y coordinate
 * @return true if walkable
 */
bool zone_can_walk_flying(const Zone* zone, uint16_t x, uint16_t y);

/**
 * Check if position is walkable for underground creatures
 * @param zone Zone pointer
 * @param x X coordinate
 * @param y Y coordinate
 * @return true if walkable
 */
bool zone_can_walk_underground(const Zone* zone, uint16_t x, uint16_t y);

/**
 * Get error string
 * @param err Error code
 * @return Human-readable error string
 */
const char* zone_strerror(ZoneError err);

/** Enable or disable verbose debug logging inside parser */
void zone_set_debug(int enable);

#ifdef __cplusplus
}
#endif

#endif /* ZONE_H */
