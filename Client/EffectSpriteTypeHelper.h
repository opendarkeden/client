//----------------------------------------------------------------------
// EffectSpriteTypeHelper.h
//----------------------------------------------------------------------
//
// Helper functions for effect sprite type calculations.
// Extracted from MTopView::GetEffectSpriteType for better decoupling.
//
// Benefits:
// - MSector no longer depends on MTopView
// - Logic is centralized and testable
// - Both game and demo can use this without full MTopView dependency
//
//----------------------------------------------------------------------

#ifndef __EFFECTSPRITETYPEHELPER_H__
#define __EFFECTSPRITETYPEHELPER_H__

#include "MTypeDef.h"
#include "DrawTypeDef.h"      // For BLT_TYPE enum
#include "EffectSpriteTypeDef.h" // For MAX_EFFECTSPRITETYPE_* defines

/**
 * Calculate effect sprite type from BLT type and frame ID.
 *
 * This function was extracted from MTopView::GetEffectSpriteType
 * to allow MSector to work without depending on MTopView.
 *
 * @param bltType The blit type (BLT_SCREEN, BLT_EFFECT, BLT_NORMAL, BLT_SHADOW)
 * @param frameID The frame ID
 * @return The calculated effect sprite type
 */
inline int GetEffectSpriteType(BLT_TYPE bltType, TYPE_FRAMEID frameID)
{
	switch (bltType)
	{
		case BLT_SCREEN :
			return frameID + MAX_EFFECTSPRITETYPE_ALPHAEFFECT;

		case BLT_EFFECT :
			return frameID;

		case BLT_NORMAL :
			return frameID + MAX_EFFECTSPRITETYPE_SCREENEFFECT;

		case BLT_SHADOW :
			return frameID + MAX_EFFECTSPRITETYPE_NORMALEFFECT;
	}

	return 0;
}

#endif // __EFFECTSPRITETYPEHELPER_H__
