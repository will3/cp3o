#pragma once

#include "../core/RenderTarget.h"
#include "../core/Renderer.h"
#include "../core/Camera.h"
#include "../core/Scene.h"
#include "Pass.h"

class RenderPass : public Pass {
private:
	Scene * scene;
	Camera *camera;
public:

	RenderPass(Scene *scene, Camera *camera) : scene(scene), camera(camera) {}

	void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) override {
		if (renderToScreen) {
			renderer->render(scene, camera);
		}
		else {
			renderer->render(scene, camera, writeBuffer);
		}
	}
};
