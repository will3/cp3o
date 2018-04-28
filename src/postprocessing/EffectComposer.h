#pragma once
#include "../core/Renderer.h"
#include <vector>
#include "../core/RenderTarget.h"
#include "Pass.h"

class EffectComposer {
private:
	std::vector<Pass *> passes;
	RenderTarget *writeBuffer;
	RenderTarget *readBuffer;

public:
	Renderer *renderer;

	EffectComposer(Renderer *renderer) : renderer(renderer) {
		writeBuffer = new RenderTarget(renderer->window->width, renderer->window->height);
		readBuffer = new RenderTarget(renderer->window->width, renderer->window->height);
	}

	void add_pass(Pass *pass) {
		passes.push_back(pass);
	}

	void render() {
		for (auto pass : passes) {
			pass->render(renderer, writeBuffer, readBuffer);

			swapBuffers();
		}
	}

	void swapBuffers() {
		auto temp = readBuffer;
		readBuffer = writeBuffer;
		writeBuffer = temp;
	}
};
