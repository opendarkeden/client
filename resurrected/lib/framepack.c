/**
 * @file framepack.c
 * @brief FramePack structures implementation (Pure C)
 * 
 * Requirements: 3.1, 3.2, 3.3
 */

#include "framepack.h"
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * CreatureFramePack Implementation
 * ============================================================================ */

void creature_framepack_init(CreatureFramePack* pack)
{
    if (pack == NULL) return;
    pack->creatures.v.data = NULL;
    pack->creatures.v.len = 0;
    pack->creatures.v.cap = 0;
}

void creature_framepack_free(CreatureFramePack* pack)
{
    if (pack == NULL) return;
    
    /* Free each nested ActionArray */
    for (int i = 0; i < vecLen(&pack->creatures); i++) {
        ActionArray* aa = vecRef(&pack->creatures, i);
        action_array_free(aa);
    }
    
    vecFree(&pack->creatures);
}

int creature_framepack_size(const CreatureFramePack* pack)
{
    if (pack == NULL) return 0;
    return vecLen(&pack->creatures);
}

ActionArray* creature_framepack_get(CreatureFramePack* pack, int creature_type)
{
    if (pack == NULL || creature_type < 0 || creature_type >= vecLen(&pack->creatures)) {
        return NULL;
    }
    return vecRef(&pack->creatures, creature_type);
}

Frame* creature_framepack_get_frame(CreatureFramePack* pack, 
                                    int creature_type, int action, 
                                    int direction, int frame_idx)
{
    ActionArray* aa = creature_framepack_get(pack, creature_type);
    if (aa == NULL) return NULL;
    
    DirectionArray* da = action_array_get(aa, action);
    if (da == NULL) return NULL;
    
    FrameArray* fa = direction_array_get(da, direction);
    if (fa == NULL) return NULL;
    
    return frame_array_get(fa, frame_idx);
}

int creature_framepack_load(CreatureFramePack* pack, const char* filename)
{
    if (pack == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    int result = creature_framepack_load_from_file(pack, file);
    fclose(file);
    
    return result;
}

int creature_framepack_load_from_file(CreatureFramePack* pack, FILE* file)
{
    if (pack == NULL || file == NULL) return 0;
    
    /* Initialize pack */
    creature_framepack_init(pack);
    
    /* Read size as uint16_t (TYPE_FRAMEID) */
    uint16_t size;
    if (fread(&size, sizeof(uint16_t), 1, file) != 1) {
        return 0;
    }
    
    if (size == 0) {
        return 1;  /* Empty pack is valid */
    }
    
    /* Pre-allocate */
    vecInit(&pack->creatures, size);
    
    /* Read each ActionArray */
    for (uint16_t i = 0; i < size; i++) {
        ActionArray aa;
        if (!action_array_load(&aa, file)) {
            creature_framepack_free(pack);
            return 0;
        }
        vecAppend(&pack->creatures, aa);
    }
    
    return 1;
}

int creature_framepack_save(const CreatureFramePack* pack, const char* filename)
{
    if (pack == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    int result = creature_framepack_save_to_file(pack, file);
    fclose(file);
    
    return result;
}

int creature_framepack_save_to_file(const CreatureFramePack* pack, FILE* file)
{
    if (pack == NULL || file == NULL) return 0;
    
    /* Write size as uint16_t */
    uint16_t size = (uint16_t)vecLen(&pack->creatures);
    if (fwrite(&size, sizeof(uint16_t), 1, file) != 1) {
        return 0;
    }
    
    /* Write each ActionArray */
    for (int i = 0; i < vecLen(&pack->creatures); i++) {
        ActionArray* aa = vecRef((vector(ActionArray)*)&pack->creatures, i);
        if (!action_array_save(aa, file)) {
            return 0;
        }
    }
    
    return 1;
}

void creature_framepack_info_to_file(const CreatureFramePack* pack, const char* filename)
{
    if (pack == NULL || filename == NULL) return;
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) return;
    
    fprintf(file, "=== CreatureFramePack Info ===\n");
    fprintf(file, "Total CreatureTypes: %d\n\n", creature_framepack_size(pack));
    
    /* Iterate through all creature types */
    for (int type = 0; type < vecLen(&pack->creatures); type++) {
        ActionArray* aa = vecRef((vector(ActionArray)*)&pack->creatures, type);
        
        if (action_array_size(aa) == 0) {
            fprintf(file, "[%d] (empty)\n", type);
            continue;
        }
        
        /* Iterate through all actions */
        for (int action = 0; action < action_array_size(aa); action++) {
            DirectionArray* da = action_array_get(aa, action);
            
            if (direction_array_size(da) == 0) {
                continue;
            }
            
            /* Iterate through all directions */
            for (int dir = 0; dir < direction_array_size(da); dir++) {
                FrameArray* fa = direction_array_get(da, dir);
                
                if (frame_array_size(fa) == 0) {
                    continue;
                }
                
                /* Iterate through all frames */
                for (int frame_idx = 0; frame_idx < frame_array_size(fa); frame_idx++) {
                    Frame* f = frame_array_get(fa, frame_idx);
                    
                    fprintf(file, "[%d][%d][%d][%d] = ( %d, %d, %d )\n",
                            type, action, dir, frame_idx,
                            f->sprite_id, f->cx, f->cy);
                }
            }
        }
    }
    
    fclose(file);
}

/* ============================================================================
 * DirectionFramePack Implementation
 * ============================================================================ */

void direction_framepack_init(DirectionFramePack* pack)
{
    if (pack == NULL) return;
    pack->types.v.data = NULL;
    pack->types.v.len = 0;
    pack->types.v.cap = 0;
}

void direction_framepack_free(DirectionFramePack* pack)
{
    if (pack == NULL) return;
    
    /* Free each nested DirectionArray */
    for (int i = 0; i < vecLen(&pack->types); i++) {
        DirectionArray* da = vecRef(&pack->types, i);
        direction_array_free(da);
    }
    
    vecFree(&pack->types);
}

int direction_framepack_size(const DirectionFramePack* pack)
{
    if (pack == NULL) return 0;
    return vecLen(&pack->types);
}

DirectionArray* direction_framepack_get(DirectionFramePack* pack, int type_idx)
{
    if (pack == NULL || type_idx < 0 || type_idx >= vecLen(&pack->types)) {
        return NULL;
    }
    return vecRef(&pack->types, type_idx);
}

int direction_framepack_load(DirectionFramePack* pack, const char* filename)
{
    if (pack == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    int result = direction_framepack_load_from_file(pack, file);
    fclose(file);
    
    return result;
}

int direction_framepack_load_from_file(DirectionFramePack* pack, FILE* file)
{
    if (pack == NULL || file == NULL) return 0;
    
    /* Initialize pack */
    direction_framepack_init(pack);
    
    /* Read size as uint16_t */
    uint16_t size;
    if (fread(&size, sizeof(uint16_t), 1, file) != 1) {
        return 0;
    }
    
    if (size == 0) {
        return 1;
    }
    
    /* Pre-allocate */
    vecInit(&pack->types, size);
    
    /* Read each DirectionArray */
    for (uint16_t i = 0; i < size; i++) {
        DirectionArray da;
        if (!direction_array_load(&da, file)) {
            direction_framepack_free(pack);
            return 0;
        }
        vecAppend(&pack->types, da);
    }
    
    return 1;
}

/* ============================================================================
 * AnimationFramePack Implementation
 * ============================================================================ */

void animation_framepack_init(AnimationFramePack* pack)
{
    if (pack == NULL) return;
    pack->types.v.data = NULL;
    pack->types.v.len = 0;
    pack->types.v.cap = 0;
}

void animation_framepack_free(AnimationFramePack* pack)
{
    if (pack == NULL) return;
    
    /* Free each nested FrameArray */
    for (int i = 0; i < vecLen(&pack->types); i++) {
        FrameArray* fa = vecRef(&pack->types, i);
        frame_array_free(fa);
    }
    
    vecFree(&pack->types);
}

int animation_framepack_size(const AnimationFramePack* pack)
{
    if (pack == NULL) return 0;
    return vecLen(&pack->types);
}

FrameArray* animation_framepack_get(AnimationFramePack* pack, int type_idx)
{
    if (pack == NULL || type_idx < 0 || type_idx >= vecLen(&pack->types)) {
        return NULL;
    }
    return vecRef(&pack->types, type_idx);
}

int animation_framepack_load(AnimationFramePack* pack, const char* filename)
{
    if (pack == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    int result = animation_framepack_load_from_file(pack, file);
    fclose(file);
    
    return result;
}

int animation_framepack_load_from_file(AnimationFramePack* pack, FILE* file)
{
    if (pack == NULL || file == NULL) return 0;
    
    /* Initialize pack */
    animation_framepack_init(pack);
    
    /* Read size as uint16_t */
    uint16_t size;
    if (fread(&size, sizeof(uint16_t), 1, file) != 1) {
        return 0;
    }
    
    if (size == 0) {
        return 1;
    }
    
    /* Pre-allocate */
    vecInit(&pack->types, size);
    
    /* Read each FrameArray */
    for (uint16_t i = 0; i < size; i++) {
        FrameArray fa;
        if (!frame_array_load(&fa, file)) {
            animation_framepack_free(pack);
            return 0;
        }
        vecAppend(&pack->types, fa);
    }
    
    return 1;
}

/* ============================================================================
 * ImageFramePack Implementation
 * ============================================================================ */

void image_framepack_init(ImageFramePack* pack)
{
    if (pack == NULL) return;
    pack->frames.v.data = NULL;
    pack->frames.v.len = 0;
    pack->frames.v.cap = 0;
}

void image_framepack_free(ImageFramePack* pack)
{
    if (pack == NULL) return;
    vecFree(&pack->frames);
}

int image_framepack_size(const ImageFramePack* pack)
{
    if (pack == NULL) return 0;
    return vecLen(&pack->frames);
}

Frame* image_framepack_get(ImageFramePack* pack, int index)
{
    if (pack == NULL || index < 0 || index >= vecLen(&pack->frames)) {
        return NULL;
    }
    return vecRef(&pack->frames, index);
}

int image_framepack_load(ImageFramePack* pack, const char* filename)
{
    if (pack == NULL || filename == NULL) return 0;
    
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    int result = image_framepack_load_from_file(pack, file);
    fclose(file);
    
    return result;
}

int image_framepack_load_from_file(ImageFramePack* pack, FILE* file)
{
    if (pack == NULL || file == NULL) return 0;
    
    /* Initialize pack */
    image_framepack_init(pack);
    
    /* Read size as uint16_t */
    uint16_t size;
    if (fread(&size, sizeof(uint16_t), 1, file) != 1) {
        return 0;
    }
    
    if (size == 0) {
        return 1;
    }
    
    /* Pre-allocate */
    vecInit(&pack->frames, size);
    
    /* Read each Frame */
    for (uint16_t i = 0; i < size; i++) {
        Frame f;
        if (!frame_load(&f, file)) {
            image_framepack_free(pack);
            return 0;
        }
        vecAppend(&pack->frames, f);
    }
    
    return 1;
}
