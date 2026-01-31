#include "RenderTargetSpriteSurface.h"

#include "SpriteLib/CSpriteSurface.h"
#include "SpriteLib/SpriteLibBackend.h"

namespace TextSystem {

SpriteSurfaceRenderTarget::SpriteSurfaceRenderTarget(CSpriteSurface* surface)
	: m_surface(surface)
{
}

SpriteSurfaceRenderTarget::~SpriteSurfaceRenderTarget() = default;

void* SpriteSurfaceRenderTarget::GetNative(NativeTargetType type) const
{
	if (!m_surface)
		return NULL;
	if (type == NativeTargetType::SpriteCtlSurface)
		return reinterpret_cast<void*>(m_surface->GetBackendSurface());
	return NULL;
}

int SpriteSurfaceRenderTarget::GetWidth() const
{
	return m_surface ? m_surface->GetWidth() : 0;
}

int SpriteSurfaceRenderTarget::GetHeight() const
{
	return m_surface ? m_surface->GetHeight() : 0;
}

} // namespace TextSystem
