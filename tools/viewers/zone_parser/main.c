/**
 * @file main.c
 * @brief Zone file parser/dumper tool
 *
 * Usage: zone_parser <zone_file.map> [options]
 *
 * Options:
 *   -h, --header    Show header info only
 *   -s, --sectors   Dump sector grid info
 *   -o, --objects   Dump image objects
 *   -a, --all       Show all info (default)
 */

#include "engine/sprite/include/zone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static void print_usage(const char* prog) {
    printf("Usage: %s <zone_file.map> [options]\n", prog);
    printf("\nOptions:\n");
    printf("  -h, --header    Show header info only\n");
    printf("  -s, --sectors   Dump sector grid statistics\n");
    printf("  -o, --objects   Dump image objects\n");
    printf("  -a, --all       Show all info (default)\n");
}

static const char* zone_type_name(uint8_t type) {
    switch (type) {
        case ZONE_TYPE_NORMAL: return "Normal";
        case ZONE_TYPE_SLAYER_GUILD: return "Slayer Guild";
        case ZONE_TYPE_NPC_VAMPIRE_LAIR: return "NPC Vampire Lair";
        case ZONE_TYPE_PC_VAMPIRE_LAIR: return "PC Vampire Lair";
        case ZONE_TYPE_NPC_HOME: return "NPC Home";
        case ZONE_TYPE_NPC_SHOP: return "NPC Shop";
        case ZONE_TYPE_RANDOMAP: return "Random Map";
        default: return "Unknown";
    }
}

static void print_header(const Zone* z) {
    printf("\n=== Zone Header ===\n");
    printf("Version:      %s\n", z->header.version);
    printf("Zone ID:      %u\n", z->header.zone_id);
    printf("Group ID:     %u\n", z->header.zone_group_id);
    printf("Name:         %s\n", z->header.zone_name);
    printf("Type:         %u (%s)\n", z->header.zone_type, zone_type_name(z->header.zone_type));
    printf("Level:        %u\n", z->header.zone_level);
    if (z->header.description[0]) {
        printf("Description:  %s\n", z->header.description);
    }
}

static void print_sector_stats(const Zone* z) {
    printf("\n=== Sector Statistics ===\n");
    printf("Dimensions:   %u x %u (%u total sectors)\n",
           z->width, z->height, (uint32_t)z->width * z->height);
    printf("Tile FP:      %lu\n", (unsigned long)z->fp_tile);
    printf("Object FP:    %lu\n", (unsigned long)z->fp_image_object);

    /* Count statistics */
    uint32_t total = (uint32_t)z->width * z->height;
    uint32_t empty = 0;
    uint32_t blocked = 0;
    uint32_t portals = 0;

    for (uint32_t i = 0; i < total; i++) {
        const Sector* s = &z->sectors[i];
        if (s->sprite_id == 0xFFFF) empty++;
        if (s->property & (SECTOR_BLOCK_GROUND | SECTOR_BLOCK_FLYING | SECTOR_BLOCK_UNDERGROUND)) blocked++;
        if (s->property & SECTOR_PORTAL) portals++;
    }

    printf("\nSector counts:\n");
    printf("  Empty (0xFFFF):     %u (%.1f%%)\n", empty, 100.0 * empty / total);
    printf("  Blocked:           %u (%.1f%%)\n", blocked, 100.0 * blocked / total);
    printf("  Portals:            %u\n", portals);
}

static void print_sector_sample(const Zone* z, uint32_t count) {
    printf("\n=== Sector Sample (first %u) ===\n", count);

    uint32_t total = (uint32_t)z->width * z->height;
    if (count > total) count = total;

    for (uint32_t i = 0; i < count; i++) {
        const Sector* s = &z->sectors[i];
        uint16_t x = i % z->width;
        uint16_t y = i / z->width;

        printf("[%3u,%3u] sprite=0x%04X prop=0x%02X light=%u\n",
               x, y, s->sprite_id, s->property, s->light);
    }
}

static const char* object_type_name(uint8_t type) {
    switch (type) {
        case OBJECT_TYPE_IMAGEOBJECT: return "ImageObject";
        case OBJECT_TYPE_SHADOWOBJECT: return "ShadowObject";
        case OBJECT_TYPE_ANIMATIONOBJECT: return "AnimationObject";
        case OBJECT_TYPE_SHADOWANIMATIONOBJECT: return "ShadowAnimationObject";
        case OBJECT_TYPE_INTERACTIONOBJECT: return "InteractionObject";
        default: return "Unknown";
    }
}

static void print_image_objects(const Zone* z) {
    printf("\n=== Image Objects (%u total) ===\n", z->image_object_count);

    for (uint32_t i = 0; i < z->image_object_count; i++) {
        const ImageObject* obj = &z->image_objects[i];

        printf("[%u] %s (id=%u)\n", i, object_type_name(obj->type), obj->id);
        printf("    sprite=%u pos=(%d,%d) view=%u\n",
               obj->sprite_id, obj->pixel_x, obj->pixel_y, obj->viewpoint);
        printf("    positions=%u\n", obj->position_count);

        if (obj->position_count > 0 && obj->position_count <= 10) {
            printf("      [");
            for (uint32_t j = 0; j < obj->position_count; j++) {
                printf("(%u,%u)", obj->positions[j].x, obj->positions[j].y);
                if (j < obj->position_count - 1) printf(", ");
            }
            printf("]\n");
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char* zone_file = argv[1];
    int show_header = 0;
    int show_sectors = 0;
    int show_objects = 0;
    int show_all = 1;

    static struct option long_options[] = {
        {"header", no_argument, 0, 'h'},
        {"sectors", no_argument, 0, 's'},
        {"objects", no_argument, 0, 'o'},
        {"all", no_argument, 0, 'a'},
        {0, 0, 0, 0}
    };

    int c;
    while ((c = getopt_long(argc, argv, "hsoa", long_options, NULL)) != -1) {
        switch (c) {
            case 'h': show_header = 1; show_all = 0; break;
            case 's': show_sectors = 1; show_all = 0; break;
            case 'o': show_objects = 1; show_all = 0; break;
            case 'a': show_all = 1; break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    printf("Zone Parser\n");
    printf("===========\n");
    printf("Loading zone file: %s\n", zone_file);

    Zone zone;
    Error* err = zone_load(zone_file, &zone);
    if (err != NULL) {
        fprintf(stderr, "Error: Failed to load zone file\n");
        if (err->msg) {
            fprintf(stderr, "  %s\n", err->msg);
        }
        free(err);
        return 1;
    }

    printf("Loaded successfully: %u x %u\n", zone.width, zone.height);

    if (show_all || show_header) {
        print_header(&zone);
    }

    if (show_all || show_sectors) {
        print_sector_stats(&zone);
        if (show_all) {
            print_sector_sample(&zone, 10);
        }
    }

    if (show_all || show_objects) {
        print_image_objects(&zone);
    }

    printf("\n=== Summary ===\n");
    printf("Zone: %s (%u)\n", zone.header.zone_name, zone.header.zone_id);
    printf("Size: %u x %u = %u sectors\n", zone.width, zone.height,
           (uint32_t)zone.width * zone.height);
    printf("Image Objects: %u\n", zone.image_object_count);

    zone_free(&zone);

    return 0;
}
