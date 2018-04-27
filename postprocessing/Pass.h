#pragma once

#include "Renderer.h"
#include "RenderTarget.h"

class Pass {
public:
	bool renderToScreen = false;
	virtual void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) {
		throw std::exception("must override");
	}
};