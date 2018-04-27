#pragma once
#include "Pass.h"
#include "Scene.h"
#include "ShaderMaterial.h"
#include "DepthShader.h"

class ShadowMap {
public:
	Camera *camera;
	ShaderMaterial *depthMaterial;
	RenderTarget *renderTarget;
	bool loaded = false;

	ShadowMap(float width, float height, float near, float far, int textureWidth, int textureHeight) {
		camera = new OrthographicCamera(-width / 2, width / 2, -height / 2, height / 2, near, far);
		renderTarget = new RenderTarget(textureWidth, textureHeight);
		depthMaterial = new ShaderMaterial(new DepthShader());
	}

	GLuint getShadowMap() {
		return renderTarget->depthTexture;
	}

	void load() {
		depthMaterial->load();
		renderTarget->load();
	}

	void unload() {
		depthMaterial->unload();
		renderTarget->unload();
	}

	void render(Renderer *renderer, Scene *scene) {
		if (!loaded) {
			load();
			loaded = true;
		}
		scene->override_material = depthMaterial;
		renderer->render(scene, camera, renderTarget);
		scene->override_material = 0;
	}

	~ShadowMap() {
		delete renderTarget;
		delete depthMaterial;
	}
};