#ifndef TEXTSYSTEM_RENDERTARGET_H
#define TEXTSYSTEM_RENDERTARGET_H

#include <cstdint>

namespace TextSystem {

enum class NativeTargetType {
	SpriteCtlSurface
};

class RenderTarget {
public:
	virtual ~RenderTarget() {}
	virtual void* GetNative(NativeTargetType type) const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
};

} // namespace TextSystem

#endif // TEXTSYSTEM_RENDERTARGET_H
