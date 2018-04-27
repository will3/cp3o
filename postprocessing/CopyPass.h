#pragma once

#include "Pass.h"
#include "common\shader.hpp"
#include "ShaderLoader.h"

class CopyPass : public Pass {

private:

	GLuint programID;
	GLuint texID;
	GLuint quad_vertexbuffer;
	GLuint MatrixID;

	void load() {
		// The fullscreen quad's FBO
		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
		};

		glGenBuffers(1, &quad_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		programID = ShaderLoader::LoadProgram("copy");
		texID = glGetUniformLocation(programID, "renderedTexture");
		MatrixID = glGetUniformLocation(programID, "MVP");
	}

public:

	Camera *camera = new OrthographicCamera(-1, 1, -1, 1, 0, 1);

	CopyPass() {
		load();
	}

	~CopyPass() { }

	void render(Renderer *renderer, RenderTarget *writeBuffer, RenderTarget *readBuffer) override {
		if (renderToScreen) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, writeBuffer->FramebufferName);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glm::mat4 MVP = camera->Projection * camera->View;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, readBuffer->renderedTexture);
		glUniform1i(texID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

		glDisableVertexAttribArray(0);
	}
};