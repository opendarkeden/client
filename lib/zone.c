/**
 * @file zone.c
 * @brief Zone (map) file parsing implementation
 * 
 * Based on original MZone.cpp, ZoneFileHeader.cpp, MSector.cpp
 */

#include "zone.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
/* Debug logging */
static int g_zone_debug = 0;
static void zdbg(FILE* f, const char* fmt, ...) {
    if (!g_zone_debug) return;
    long pos = ftell(f);
    fprintf(stderr, "[zone dbg] @0x%08lX ", pos);
    va_list ap; va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
}

void zone_set_debug(int enable) {
    g_zone_debug = enable ? 1 : 0;
}

/* ============================================================================
 * Internal helpers - Little-endian reads
 * ============================================================================ */

static int read_u8(FILE* f, uint8_t* out) {
    int c = fgetc(f);
    if (c == EOF) return -1;
    *out = (uint8_t)c;
    return 0;
}

static int read_u16_le(FILE* f, uint16_t* out) {
    uint8_t buf[2];
    if (fread(buf, 1, 2, f) != 2) return -1;
    *out = (uint16_t)(buf[0] | (buf[1] << 8));
    return 0;
}

static int read_u32_le(FILE* f, uint32_t* out) {
    uint8_t buf[4];
    if (fread(buf, 1, 4, f) != 4) return -1;
    *out = (uint32_t)(buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));
    return 0;
}

static int read_i32_le(FILE* f, int32_t* out) {
    uint32_t u;
    if (read_u32_le(f, &u) != 0) return -1;
    *out = (int32_t)u;
    return 0;
}

/**
 * Read MString format: 4-byte length + string data (no null terminator in file)
 */
static int read_mstring(FILE* f, char* out, size_t max_len) {
    uint32_t len;
    if (read_u32_le(f, &len) != 0) return -1;
    
    if (len == 0) {
        out[0] = '\0';
        return 0;
    }
    
    if (len >= max_len) {
        /* String too long, skip it */
        fseek(f, (long)len, SEEK_CUR);
        out[0] = '\0';
        return 0;
    }
    
    if (fread(out, 1, len, f) != len) return -1;
    out[len] = '\0';
    return 0;
}

/* ============================================================================
 * Zone Header Loading
 * ============================================================================ */

static Error* load_zone_header(FILE* f, ZoneHeader* header) {
    memset(header, 0, sizeof(ZoneHeader));
    
    /* Read version string (MString format) */
    if (read_mstring(f, header->version, ZONE_VERSION_LEN + 1) != 0) {
        return_err_code(ZONE_ERR_IO);
    }
    
    /* Validate version */
    if (strcmp(header->version, ZONE_VERSION_STRING) != 0) {
        return_err_code(ZONE_ERR_VERSION);
    }
    
    /* Read zone ID */
    if (read_u16_le(f, &header->zone_id) != 0) return_err_code(ZONE_ERR_IO);
    
    /* Read zone group ID */
    if (read_u16_le(f, &header->zone_group_id) != 0) return_err_code(ZONE_ERR_IO);
    
    /* Read zone name */
    if (read_mstring(f, header->zone_name, ZONE_MAX_NAME_LEN) != 0) {
        return_err_code(ZONE_ERR_IO);
    }
    
    /* Read zone type */
    if (read_u8(f, &header->zone_type) != 0) return_err_code(ZONE_ERR_IO);
    
    /* Read zone level */
    if (read_u8(f, &header->zone_level) != 0) return_err_code(ZONE_ERR_IO);
    
    /* Read description */
    if (read_mstring(f, header->description, ZONE_MAX_DESC_LEN) != 0) {
        return_err_code(ZONE_ERR_IO);
    }
    
    return NULL;
}

/* ============================================================================
 * Sector Loading
 * ============================================================================ */

static Error*
load_sectors(FILE* f, Zone* zone) {
    uint32_t total = (uint32_t)zone->width * (uint32_t)zone->height;
    
    zone->sectors = (Sector*)calloc(total, sizeof(Sector));
    if (!zone->sectors) return_err_code(ZONE_ERR_NOMEM);
    
    for (uint32_t i = 0; i < total; i++) {
        Sector* s = &zone->sectors[i];
        
        if (read_u16_le(f, &s->sprite_id) != 0) return_err_code(ZONE_ERR_IO);
        if (read_u8(f, &s->property) != 0) return_err_code(ZONE_ERR_IO);
        if (read_u8(f, &s->light) != 0) return_err_code(ZONE_ERR_IO);
    }
    
    return NULL;
}

/* ============================================================================
 * ImageObject Loading
 * ============================================================================ */

static void free_image_object(ImageObject* obj) {
    if (obj->positions) {
        free(obj->positions);
        obj->positions = NULL;
    }
}

static Error* load_image_object_base(FILE* f, ImageObject* obj, uint8_t type_first_byte) {
    /* In original format, object type byte appears twice:
       - Once before object record (used to choose class)
       - Again at the start of MObject::SaveToFile
       Consume and validate the second byte here. */
    uint8_t inner_type;
    if (read_u8(f, &inner_type) != 0) return_err_code(ZONE_ERR_IO);
    /* Accept mismatch but continue to avoid hard failure on variant files */
    (void)type_first_byte;
    if (g_zone_debug && inner_type != type_first_byte) {
        zdbg(f, "warn: inner_type(%u) != outer_type(%u)", inner_type, type_first_byte);
    }

    /* Read base MObject fields: ID (4), sector X (2), sector Y (2) */
    if (read_u32_le(f, &obj->id) != 0) return_err_code(ZONE_ERR_IO);
    if (read_u16_le(f, &obj->sector_x) != 0) return_err_code(ZONE_ERR_IO);
    if (read_u16_le(f, &obj->sector_y) != 0) return_err_code(ZONE_ERR_IO);
    zdbg(f, "MObject: id=%u sx=%u sy=%u", obj->id, obj->sector_x, obj->sector_y);

    /* Read MImageObject fields */
    if (read_u32_le(f, &obj->image_object_id) != 0) return_err_code(ZONE_ERR_IO);
    if (read_u16_le(f, &obj->sprite_id) != 0) return_err_code(ZONE_ERR_IO);
    if (read_i32_le(f, &obj->pixel_x) != 0) return_err_code(ZONE_ERR_IO);
    if (read_i32_le(f, &obj->pixel_y) != 0) return_err_code(ZONE_ERR_IO);
    if (read_u16_le(f, &obj->viewpoint) != 0) return_err_code(ZONE_ERR_IO);
    if (read_u8(f, &obj->is_animation) != 0) return_err_code(ZONE_ERR_IO);
    if (read_u8(f, &obj->trans_flags) != 0) return_err_code(ZONE_ERR_IO);
    zdbg(f, "MImageObject: imgID=%u sprite=%u px=%d py=%d vp=%u anim=%u trans=0x%02X",
         obj->image_object_id, obj->sprite_id, obj->pixel_x, obj->pixel_y,
         obj->viewpoint, obj->is_animation, obj->trans_flags);

    return NULL;
}

static Error* load_image_object_positions(FILE* f, ImageObject* obj) {
    /* Original IMAGEOBJECT_POSITION_LIST uses 2-byte size (WORD) */
    uint16_t count16;
    if (read_u16_le(f, &count16) != 0) return_err_code(ZONE_ERR_IO);
    obj->position_count = count16;
    zdbg(f, "ImageObject positions count=%u", obj->position_count);

    if (obj->position_count == 0) {
        obj->positions = NULL;
        return NULL;
    }

    obj->positions = (ImageObjectPosition*)calloc(obj->position_count,
                                                  sizeof(ImageObjectPosition));
    if (!obj->positions) return_err_code(ZONE_ERR_NOMEM);

        uint32_t log_head = 16;
        uint32_t log_tail = 8;
        if (!g_zone_debug) { log_head = 0; log_tail = 0; }
        if (log_head > obj->position_count) log_head = obj->position_count;
    for (uint32_t i = 0; i < obj->position_count; i++) {
        if (read_u16_le(f, &obj->positions[i].x) != 0) return_err_code(ZONE_ERR_IO);
        if (read_u16_le(f, &obj->positions[i].y) != 0) return_err_code(ZONE_ERR_IO);
            if (g_zone_debug) {
                if (i < log_head) {
                    zdbg(f, "  pos[%u]=(%u,%u)", i, obj->positions[i].x, obj->positions[i].y);
                } else if (i >= obj->position_count - log_tail) {
                    zdbg(f, "  pos[%u]=(%u,%u)", i, obj->positions[i].x, obj->positions[i].y);
                }
            }
    }

    return NULL;
}

static Error* load_image_object(FILE* f, ImageObject* obj) {
    /* Read leading object type byte */
    uint8_t type_first;
    if (read_u8(f, &type_first) != 0) return_err_code(ZONE_ERR_IO);
    obj->type = type_first;
    zdbg(f, "Object type=%u (outer)", obj->type);

    /* Load base data (consumes inner type + base fields) */
    Error* err = load_image_object_base(f, obj, type_first);
    if (err != NULL) return_err_wrap(err);

    /* Load type-specific data to match original binary */
    switch (obj->type) {
        case OBJECT_TYPE_SHADOWOBJECT:
            /* Shadow object does not have extra serialized fields in original */
            obj->extra.shadow.reserved = 0;
            zdbg(f, "ShadowObject: no extra fields");
            break;

        case OBJECT_TYPE_ANIMATIONOBJECT:
        case OBJECT_TYPE_SHADOWANIMATIONOBJECT: {
            /* CAnimationFrame::LoadFromFile: frame_id (2), max_frame (1) */
            if (read_u16_le(f, &obj->extra.animation.frame_id) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.max_frame) != 0)
                return_err_code(ZONE_ERR_IO);
            zdbg(f, "Animation: frame_id=%u max_frame=%u", obj->extra.animation.frame_id, obj->extra.animation.max_frame);

            /* MAnimationObject additional fields */
            if (read_u8(f, &obj->extra.animation.blt_type) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.direction) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.sound_frame) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u16_le(f, &obj->extra.animation.sound_id) != 0)
                return_err_code(ZONE_ERR_IO);
            zdbg(f, "Animation: blt=%u dir=%u sound_fr=%u sound_id=%u",
                 obj->extra.animation.blt_type, obj->extra.animation.direction,
                 obj->extra.animation.sound_frame, obj->extra.animation.sound_id);

            /* ShowTimeChecker::LoadFromFile */
            if (read_u8(f, &obj->extra.animation.loop) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u32_le(f, &obj->extra.animation.min_delay) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u32_le(f, &obj->extra.animation.max_delay) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.start_hour) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.end_hour) != 0)
                return_err_code(ZONE_ERR_IO);
            zdbg(f, "ShowTime: loop=%u min=%u max=%u start=%u end=%u",
                 obj->extra.animation.loop, obj->extra.animation.min_delay,
                 obj->extra.animation.max_delay, obj->extra.animation.start_hour,
                 obj->extra.animation.end_hour);
            break;
        }

        case OBJECT_TYPE_INTERACTIONOBJECT:
        {
            /* Interaction object base is animation + one extra type field */
            if (read_u16_le(f, &obj->extra.animation.frame_id) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.max_frame) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.blt_type) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.direction) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.sound_frame) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u16_le(f, &obj->extra.animation.sound_id) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.loop) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u32_le(f, &obj->extra.animation.min_delay) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u32_le(f, &obj->extra.animation.max_delay) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.start_hour) != 0)
                return_err_code(ZONE_ERR_IO);
            if (read_u8(f, &obj->extra.animation.end_hour) != 0)
                return_err_code(ZONE_ERR_IO);

            if (read_u16_le(f, &obj->extra.interaction.interaction_type) != 0)
                return_err_code(ZONE_ERR_IO);
            zdbg(f, "Interaction: type=%u", obj->extra.interaction.interaction_type);
            break;
        }

        case OBJECT_TYPE_IMAGEOBJECT:
        default:
            /* No extra data */
            break;
    }

    return NULL;
}

static Error*
load_image_objects(FILE* f, Zone* zone) {
    int32_t count;
    if (read_i32_le(f, &count) != 0) return_err_code(ZONE_ERR_IO);
    
    if (count <= 0) {
        zone->image_object_count = 0;
        zone->image_objects = NULL;
        return NULL;
    }
    
    zone->image_object_count = (uint32_t)count;
    zone->image_objects = (ImageObject*)calloc(count, sizeof(ImageObject));
    if (!zone->image_objects) return_err_code(ZONE_ERR_NOMEM);
    zdbg(f, "ImageObjects: count=%d", count);
    
    for (int32_t i = 0; i < count; i++) {
        ImageObject* obj = &zone->image_objects[i];
        zdbg(f, "-- Object[%d] begin --", i);
        /* Load object data */
        Error *err = load_image_object(f, obj);
        if (err != NULL) return_err_wrap(err);
        zdbg(f, "Object[%d]: type=%u id=%u imgID=%u sprite=%u px=%d py=%d vp=%u anim=%u trans=0x%02X",
             i, obj->type, obj->id, obj->image_object_id, obj->sprite_id,
             obj->pixel_x, obj->pixel_y, obj->viewpoint, obj->is_animation, obj->trans_flags);
        /* Load position list */
        err = load_image_object_positions(f, obj);
        if (err != NULL) return_err_wrap(err);
        zdbg(f, "Object[%d]: positions_loaded=%u", i, obj->position_count);
        zdbg(f, "-- Object[%d] end --", i);
    }
    
    return NULL;
}

/* ============================================================================
 * Public API
 * ============================================================================ */

Error* zone_load(const char* path, Zone* zone) {
    if (!path || !zone) return_err_code(ZONE_ERR_PARAM);
    
    FILE* f = fopen(path, "rb");
    if (!f) return_err_code(ZONE_ERR_IO);
    
    Error* err = zone_load_from_file(f, zone);
    fclose(f);
    if (err != NULL) {
        return_err_wrap(err);
    }
    return NULL;
}

Error* zone_load_from_file(FILE* f, Zone* zone) {
    if (!f || !zone) return_err_code(ZONE_ERR_PARAM);
    
    memset(zone, 0, sizeof(Zone));
    
    /* Load header */
    Error *err = load_zone_header(f, &zone->header);
    if (err != NULL) {
        zone_free(zone);
        return_err_wrap(err);
    }
    
    /* Read file positions (for tile and image object data) */
    uint32_t fp_tile, fp_image_object;
    if (read_u32_le(f, &fp_tile) != 0) {
        zone_free(zone);
        return_err_code(ZONE_ERR_IO);
    }
    if (read_u32_le(f, &fp_image_object) != 0) {
        zone_free(zone);
        return_err_code(ZONE_ERR_IO);
    }
    zone->fp_tile = (long)fp_tile;
    zone->fp_image_object = (long)fp_image_object;
    
    /* Read zone dimensions */
    if (read_u16_le(f, &zone->width) != 0) {
        zone_free(zone);
        return_err_code(ZONE_ERR_IO);
    }
    if (read_u16_le(f, &zone->height) != 0) {
        zone_free(zone);
        return_err_code(ZONE_ERR_IO);
    }
    
    if (zone->width == 0 || zone->height == 0) {
        zone_free(zone);
        return_err_code(ZONE_ERR_FORMAT);
    }
    
    /* Load sectors */
    err = load_sectors(f, zone);
    if (err != NULL) {
        zone_free(zone);
        return_err_wrap(err);
    }
    
    /* Load image objects */
    err = load_image_objects(f, zone);
    if (err != NULL) {
        zone_free(zone);
        return_err_wrap(err);
    }
    
    return NULL;
}

Error* zone_load_header(const char* path, ZoneHeader* header) {
    if (!path || !header) return_err_code(ZONE_ERR_PARAM);
    
    FILE* f = fopen(path, "rb");
    if (!f) return_err_code(ZONE_ERR_IO);
    
    Error* err = load_zone_header(f, header);
    fclose(f);
    if (err != NULL) {
        return_err_wrap(err);
    }
    return NULL;
}

void zone_free(Zone* zone) {
    if (!zone) return;
    
    if (zone->sectors) {
        free(zone->sectors);
        zone->sectors = NULL;
    }
    
    if (zone->image_objects) {
        for (uint32_t i = 0; i < zone->image_object_count; i++) {
            free_image_object(&zone->image_objects[i]);
        }
        free(zone->image_objects);
        zone->image_objects = NULL;
    }
    
    zone->image_object_count = 0;
    zone->width = 0;
    zone->height = 0;
}

const Sector* zone_get_sector(const Zone* zone, uint16_t x, uint16_t y) {
    if (!zone || !zone->sectors) return NULL;
    if (x >= zone->width || y >= zone->height) return NULL;
    
    return &zone->sectors[(uint32_t)y * zone->width + x];
}

bool zone_can_walk_ground(const Zone* zone, uint16_t x, uint16_t y) {
    const Sector* s = zone_get_sector(zone, x, y);
    if (!s) return false;
    return (s->property & SECTOR_BLOCK_GROUND) == 0;
}

bool zone_can_walk_flying(const Zone* zone, uint16_t x, uint16_t y) {
    const Sector* s = zone_get_sector(zone, x, y);
    if (!s) return false;
    return (s->property & SECTOR_BLOCK_FLYING) == 0;
}

bool zone_can_walk_underground(const Zone* zone, uint16_t x, uint16_t y) {
    const Sector* s = zone_get_sector(zone, x, y);
    if (!s) return false;
    return (s->property & SECTOR_BLOCK_UNDERGROUND) == 0;
}

const char* zone_strerror(ZoneError err) {
    switch (err) {
        case ZONE_OK: return "OK";
        case ZONE_ERR_IO: return "I/O error";
        case ZONE_ERR_FORMAT: return "Invalid format";
        case ZONE_ERR_VERSION: return "Unsupported version";
        case ZONE_ERR_NOMEM: return "Out of memory";
        case ZONE_ERR_PARAM: return "Invalid parameter";
        default: return "Unknown error";
    }
}
