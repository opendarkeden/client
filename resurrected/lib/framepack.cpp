/**
 * @file framepack.cpp
 * @brief CCreatureFramePack implementation
 * 
 * Requirements: 3.3
 */

#include "framepack.h"
#include <stdio.h>

/* ============================================================================
 * CCreatureFramePack Implementation
 * ============================================================================ */

CCreatureFramePack::CCreatureFramePack()
{
    /* Parent constructor handles initialization */
}

CCreatureFramePack::~CCreatureFramePack()
{
    /* Parent destructor handles cleanup */
}

void CCreatureFramePack::InfoToFile(const char* filename)
{
    if (filename == NULL) {
        return;
    }
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return;
    }
    
    fprintf(file, "=== CCreatureFramePack Info ===\n");
    fprintf(file, "Total CreatureTypes: %d\n\n", m_Size);
    
    /* Iterate through all creature types */
    for (TYPE_FRAMEID type = 0; type < m_Size; type++) {
        ACTION_FRAME_ARRAY& actionArray = m_pData[type];
        
        if (actionArray.GetSize() == 0) {
            fprintf(file, "[%d] (empty)\n", type);
            continue;
        }
        
        /* Iterate through all actions */
        for (uint8_t action = 0; action < actionArray.GetSize(); action++) {
            DIRECTION_FRAME_ARRAY& dirArray = actionArray[action];
            
            if (dirArray.GetSize() == 0) {
                continue;
            }
            
            /* Iterate through all directions */
            for (uint8_t dir = 0; dir < dirArray.GetSize(); dir++) {
                FRAME_ARRAY& frameArray = dirArray[dir];
                
                if (frameArray.GetSize() == 0) {
                    continue;
                }
                
                /* Iterate through all frames */
                for (uint16_t frame = 0; frame < frameArray.GetSize(); frame++) {
                    CFrame& f = frameArray[frame];
                    
                    fprintf(file, "[%d][%d][%d][%d] = ( %d, %d, %d )\n",
                            type, action, dir, frame,
                            f.GetSpriteID(), f.GetCX(), f.GetCY());
                }
            }
        }
    }
    
    fclose(file);
}
