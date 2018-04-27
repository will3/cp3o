#pragma once
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <exception>

namespace WindowManager {
	static bool focus = true;

	static void window_focus_callback(GLFWwindow* window, int focused)
	{
		if (focused)
		{
			focus = true;
		}
		else
		{
			focus = false;
		}
	}
}

// only supports one window
class Window {
public:
	int width = 0;
	int height = 0;
	GLFWwindow *window;

	void show(int width, int height) {
		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			throw std::exception("Failed to initialize GLFW\n");
		}

		glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(width, height, "kestrel", NULL, NULL);
		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window.\n");
			getchar();
			glfwTerminate();
			throw std::exception("Failed to open GLFW window.\n");
		}

		glfwMakeContextCurrent(window);

		glfwSwapInterval(1);

		glfwGetFramebufferSize(window, &this->width, &this->height);

		glfwSetWindowFocusCallback(window, WindowManager::window_focus_callback);
	}

	bool hasFocus() {
		return WindowManager::focus;
	}

	void hide() {
		glfwTerminate();
	}
};