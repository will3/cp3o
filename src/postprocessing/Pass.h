#pragma once

#include "../core/Renderer.h"
#include "../core/RenderTarget.h"

class Pass {
public:
	bool renderToScreen = false;
	virtual void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) {
		throw std::runtime_error("must override");
	}
};
