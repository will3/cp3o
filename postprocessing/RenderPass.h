#pragma once

#include "RenderTarget.h"
#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
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