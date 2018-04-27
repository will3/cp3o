#pragma once
#include "ShaderLoader.h"
#include <map>

class UniformAttribute {
public:
	std::string name;
	int texture_position = 0;
	UniformAttribute() {};
	UniformAttribute(std::string name) : name(name) {};
};

class UniformAttributes {
	std::map<std::string, UniformAttribute> map;
	std::map<std::string, GLuint> handles;
public:
	UniformAttributes() {
		add(UniformAttribute("MVP"));
	};

	void add(UniformAttribute attribute) {
		map[attribute.name] = attribute;
	}

	void set(std::string name, glm::mat4 v) {
		GLuint handle = handles[name];
		glUniformMatrix4fv(handle, 1, GL_FALSE, &v[0][0]);
	}

	void set(std::string name, float x, float y, float z) {
		GLuint handle = handles[name];
		glUniform3f(handle, x, y, z);
	}
	
	void set(string name, bool v) {
		GLuint handle = handles[name];
		glUniform1i(handle, v);
	}

	void set(string name, float v) {
		GLuint handle = handles[name];
		glUniform1f(handle, v);
	}

	void set_texture(std::string name, GLuint texture) {
		GLuint handle = handles[name];
		UniformAttribute &attribute = map[name];

		if (attribute.texture_position == 0) {
			glActiveTexture(GL_TEXTURE0);
		}
		else if (attribute.texture_position == 1) {
			glActiveTexture(GL_TEXTURE1);
		}
		else {
			throw std::exception("not implemented");
		}
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(handle, attribute.texture_position);
	}

	void get_handles(int programID) {
		for (auto kv : map) {
			std::string name = kv.first;
			handles[name] = glGetUniformLocation(programID, name.c_str());
		}
	}
};

class Shader {
public:
	std::string programName;
	UniformAttributes uniforms;
};