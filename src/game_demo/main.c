#include "top_view.h"
#include "sdl_framework.h"
#include <stdio.h>

int main(int argc, char** argv) {
    /* Args: <zone.map> <tile.spk> <CreatureBasePath> [cell] [speed] */
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <DarkEden/Data/Map/*.map> <DarkEden/Data/Image/tile.spk> <DarkEden/Data/Image/Creature> [cell=32] [speed=120]\n", argv[0]);
        return 1;
    }

    const char* map_path = argv[1];
    const char* tile_spk = argv[2];
    const char* creature_base = argv[3];
    int cell = (argc >= 5) ? atoi(argv[4]) : 32;
    float speed = (argc >= 6) ? (float)atof(argv[5]) : 120.0f;

    TopViewConfig cfg = {
        .map_path = map_path,
        .tile_spk_path = tile_spk,
        .creature_base_path = creature_base,
        .window_width = 960,
        .window_height = 540,
        .cell_pixels = cell,
        .move_speed = speed,
    };

    TopView* tv = top_view_create();
    int rc = top_view_init(tv, &cfg);
    if (rc != 0) {
        fprintf(stderr, "TopView init failed: %d\n", rc);
        top_view_destroy(tv);
        return 2;
    }

    top_view_run(tv);
    top_view_destroy(tv);
    return 0;
}
