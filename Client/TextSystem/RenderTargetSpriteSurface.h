#ifndef TEXTSYSTEM_RENDERTARGET_SPRITESURFACE_H
#define TEXTSYSTEM_RENDERTARGET_SPRITESURFACE_H

#include "RenderTarget.h"

class CSpriteSurface;

namespace TextSystem {

class SpriteSurfaceRenderTarget : public RenderTarget {
public:
	explicit SpriteSurfaceRenderTarget(CSpriteSurface* surface);
	~SpriteSurfaceRenderTarget() override;

	void* GetNative(NativeTargetType type) const override;
	int GetWidth() const override;
	int GetHeight() const override;

private:
	CSpriteSurface* m_surface;
};

} // namespace TextSystem

#endif // TEXTSYSTEM_RENDERTARGET_SPRITESURFACE_H
