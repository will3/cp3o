#pragma once

#include "Material.h"
#include <common/shader.hpp>
#include "DirectionalLight.h"
#include <map>
#include "Shader.h"
#include "ShadowMap.h"

class StandardMaterial : public Material {
public:
	GLuint programID;
	UniformAttributes uniforms;

	StandardMaterial() {
		uniforms.add(UniformAttribute("light_dir"));
		uniforms.add(UniformAttribute("light_intensity"));
		uniforms.add(UniformAttribute("ambient_light"));
		uniforms.add(UniformAttribute("hasShadowMap"));
		uniforms.add(UniformAttribute("shadowMap"));
		uniforms.add(UniformAttribute("shadowMVP"));
		uniforms.add(UniformAttribute("modelMatrix"));
		uniforms.add(UniformAttribute("viewMatrix"));
	}

	void load() {
		programID = LoadShaders("shaders/standard.vertexshader", "shaders/standard.fragmentshader");
		uniforms.get_handles(programID);
	}

	void bind() override {
		glUseProgram(programID);

		for (auto light : scene->directional_lights) {
			uniforms.set("light_dir", light->direction[0], light->direction[1], light->direction[2]);
			uniforms.set("light_intensity", light->intensity);
			// only support one directional light
			break;
		}
		for (auto light : scene->ambient_lights) {
			uniforms.set("ambient_light", light->color[0], light->color[1], light->color[2]);
			// only support one ambient light
			break;
		}

		glm::mat4 MVP = currentCamera->Projection * currentCamera->View * currentNode->matrix;
		uniforms.set("MVP", MVP);

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		uniforms.set("hasShadowMap", scene->shadowMap != 0);

		if (scene->shadowMap != 0) {
			Camera *shadowMapCamera = scene->shadowMap->camera;
			uniforms.set_texture("shadowMap", scene->shadowMap->getShadowMap());
			glm::mat4 shadowMVP = shadowMapCamera->Projection * shadowMapCamera->View * currentNode->matrix;
			glm::mat4 shadowBiasMVP = biasMatrix * shadowMVP;

			uniforms.set("shadowMVP", shadowBiasMVP);
		}

		uniforms.set("modelMatrix", currentNode->matrix);
		uniforms.set("viewMatrix", currentCamera->View);
	}
};