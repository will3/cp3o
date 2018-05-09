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
    int programID = -1;

	void add(UniformAttribute attribute) {
		map[attribute.name] = attribute;
	}

	void set(std::string name, glm::mat4 v) {
        loadHandleIfNeeded(name);
		GLuint handle = handles[name];
		glUniformMatrix4fv(handle, 1, GL_FALSE, &v[0][0]);
	}

	void set(std::string name, float x, float y, float z) {
        loadHandleIfNeeded(name);
		GLuint handle = handles[name];
		glUniform3f(handle, x, y, z);
	}
	
    void set(std::string name, bool v) {
        loadHandleIfNeeded(name);
		GLuint handle = handles[name];
		glUniform1i(handle, v);
	}

    void set(std::string name, float v) {
        loadHandleIfNeeded(name);
		GLuint handle = handles[name];
		glUniform1f(handle, v);
	}

	void set_texture(std::string name, GLuint texture) {
        loadHandleIfNeeded(name);
		GLuint handle = handles[name];
		UniformAttribute &attribute = map[name];

		if (attribute.texture_position == 0) {
			glActiveTexture(GL_TEXTURE0);
		}
		else if (attribute.texture_position == 1) {
			glActiveTexture(GL_TEXTURE1);
		}
		else {
			throw std::runtime_error("not implemented");
		}
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(handle, attribute.texture_position);
	}
    
    void loadHandleIfNeeded(std::string name) {
        if (programID == -1) {
            throw std::runtime_error("program not loaded");
        }
        if (handles.find(name) == handles.end()) {
            handles[name] = glGetUniformLocation(programID, name.c_str());
        }
    }
};

class Shader {
public:
	std::string programName;
	UniformAttributes uniforms;
};
