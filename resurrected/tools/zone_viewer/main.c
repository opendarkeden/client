/**
 * @file main.c
 * @brief Zone file viewer/dumper tool
 * 
 * Usage: zone_viewer <zone_file.map> [options]
 * 
 * Options:
 *   -h, --header    Show header info only
 *   -s, --sectors   Dump sector grid info
 *   -o, --objects   Dump image objects
 *   -a, --all       Show all info (default)
 */

#include "zone.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(const char* prog) {
    printf("Usage: %s <zone_file.map> [options]\n", prog);
    printf("\nOptions:\n");
    printf("  -h, --header    Show header info only\n");
    printf("  -s, --sectors   Dump sector grid statistics\n");
    printf("  -o, --objects   Dump image objects\n");
    printf("  -a, --all       Show all info (default)\n");
    printf("  -d, --dump      Dump raw sector data (first 100)\n");
    printf("  -v, --verbose   Enable verbose parser debug logs\n");
}

static void print_header(const ZoneHeader* h) {
    printf("\n=== Zone Header ===\n");
    printf("Version:      %s\n", h->version);
    printf("Zone ID:      %u\n", h->zone_id);
    printf("Group ID:     %u\n", h->zone_group_id);
    printf("Name:         %s\n", h->zone_name);
    printf("Type:         %u", h->zone_type);
    switch (h->zone_type) {
        case ZONE_TYPE_NORMAL: printf(" (Normal)"); break;
        case ZONE_TYPE_SLAYER_GUILD: printf(" (Slayer Guild)"); break;
        case ZONE_TYPE_NPC_VAMPIRE_LAIR: printf(" (NPC Vampire Lair)"); break;
        case ZONE_TYPE_PC_VAMPIRE_LAIR: printf(" (PC Vampire Lair)"); break;
        case ZONE_TYPE_NPC_HOME: printf(" (NPC Home)"); break;
        case ZONE_TYPE_NPC_SHOP: printf(" (NPC Shop)"); break;
        case ZONE_TYPE_RANDOMAP: printf(" (Random Map)"); break;
        default: printf(" (Unknown)"); break;
    }
    printf("\n");
    printf("Level:        %u\n", h->zone_level);
    if (h->description[0]) {
        printf("Description:  %s\n", h->description);
    }
}

static void print_sector_stats(const Zone* z) {
    printf("\n=== Sector Statistics ===\n");
    printf("Dimensions:   %u x %u (%u total sectors)\n", 
           z->width, z->height, (uint32_t)z->width * z->height);
    printf("Tile FP:      %ld\n", z->fp_tile);
    printf("Object FP:    %ld\n", z->fp_image_object);
    
    /* Count statistics */
    uint32_t total = (uint32_t)z->width * z->height;
    uint32_t empty = 0;
    uint32_t blocked_ground = 0;
    uint32_t blocked_flying = 0;
    uint32_t blocked_underground = 0;
    uint32_t portals = 0;
    
    for (uint32_t i = 0; i < total; i++) {
        const Sector* s = &z->sectors[i];
        if (s->sprite_id == 0xFFFF) empty++;
        if (s->property & SECTOR_BLOCK_GROUND) blocked_ground++;
        if (s->property & SECTOR_BLOCK_FLYING) blocked_flying++;
        if (s->property & SECTOR_BLOCK_UNDERGROUND) blocked_underground++;
        if (s->property & SECTOR_PORTAL) portals++;
    }
    
    printf("\nSector counts:\n");
    printf("  Empty (0xFFFF):     %u (%.1f%%)\n", empty, 100.0 * empty / total);
    printf("  Block Ground:       %u (%.1f%%)\n", blocked_ground, 100.0 * blocked_ground / total);
    printf("  Block Flying:       %u (%.1f%%)\n", blocked_flying, 100.0 * blocked_flying / total);
    printf("  Block Underground:  %u (%.1f%%)\n", blocked_underground, 100.0 * blocked_underground / total);
    printf("  Portals:            %u\n", portals);
}

static void print_sector_dump(const Zone* z, uint32_t count) {
    printf("\n=== Sector Dump (first %u) ===\n", count);
    
    uint32_t total = (uint32_t)z->width * z->height;
    if (count > total) count = total;
    
    for (uint32_t i = 0; i < count; i++) {
        const Sector* s = &z->sectors[i];
        uint16_t x = i % z->width;
        uint16_t y = i / z->width;
        
        printf("[%3u,%3u] sprite=0x%04X prop=0x%02X light=%u", 
               x, y, s->sprite_id, s->property, s->light);
        
        /* Print flags */
        if (s->property) {
            printf(" (");
            int first = 1;
            if (s->property & SECTOR_BLOCK_GROUND) {
                printf("%sGND", first ? "" : "|"); first = 0;
            }
            if (s->property & SECTOR_BLOCK_FLYING) {
                printf("%sFLY", first ? "" : "|"); first = 0;
            }
            if (s->property & SECTOR_BLOCK_UNDERGROUND) {
                printf("%sUND", first ? "" : "|"); first = 0;
            }
            if (s->property & SECTOR_PORTAL) {
                printf("%sPORTAL", first ? "" : "|"); first = 0;
            }
            printf(")");
        }
        printf("\n");
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
        printf("    positions=%u", obj->position_count);
        
        if (obj->position_count > 0 && obj->position_count <= 5) {
            printf(" [");
            for (uint32_t j = 0; j < obj->position_count; j++) {
                printf("(%u,%u)", obj->positions[j].x, obj->positions[j].y);
                if (j < obj->position_count - 1) printf(",");
            }
            printf("]");
        }
        printf("\n");
        
        /* Type-specific info */
        switch (obj->type) {
            case OBJECT_TYPE_SHADOWOBJECT:
                /* No extra shadow fields in original format */
                break;
            case OBJECT_TYPE_ANIMATIONOBJECT:
            case OBJECT_TYPE_SHADOWANIMATIONOBJECT:
                printf("    frames=%u current=%u\n",
                       obj->extra.animation.max_frame,
                       0u);
                break;
            case OBJECT_TYPE_INTERACTIONOBJECT:
                printf("    interaction_type=%u\n", obj->extra.interaction.interaction_type);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    int show_header = 0;
    int show_sectors = 0;
    int show_objects = 0;
    int show_dump = 0;
    int verbose = 0;
    
    /* Parse options */
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--header") == 0) {
            show_header = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sectors") == 0) {
            show_sectors = 1;
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--objects") == 0) {
            show_objects = 1;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all") == 0) {
            show_header = show_sectors = show_objects = 1;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--dump") == 0) {
            show_dump = 1;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }
    
    /* Default: show all */
    if (!show_header && !show_sectors && !show_objects && !show_dump) {
        show_header = show_sectors = show_objects = 1;
    }
    
    /* Load zone */
    printf("Loading zone: %s\n", filename);
    
    Zone zone;
    if (verbose) zone_set_debug(1);
    Error* err = zone_load(filename, &zone);
    if (err != NULL) {
        fprintf(stderr, "Error loading zone: %s\n", zone_strerror(err->code));
        err_print(err, stderr);
        return 1;
    }
    
    /* Print requested info */
    if (show_header) {
        print_header(&zone.header);
    }
    
    if (show_sectors) {
        print_sector_stats(&zone);
    }
    
    if (show_dump) {
        print_sector_dump(&zone, 100);
    }
    
    if (show_objects) {
        print_image_objects(&zone);
    }
    
    /* Cleanup */
    zone_free(&zone);
    
    printf("\nDone.\n");
    return 0;
}
