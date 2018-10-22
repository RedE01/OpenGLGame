#pragma once
#include <GLFW\glfw3.h>

namespace input {
	extern float mouseX = 0;
	extern float mouseY = 0;

	extern float mouseLastX = 0;
	extern float mouseLastY = 0;

	extern float mouseDeltaX = 0;
	extern float mouseDeltaY = 0;

	extern bool firstMouse = true;

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		mouseX = xpos;
		mouseY = ypos;

		if (firstMouse) {
			mouseLastX = xpos;
			mouseLastY = ypos;
			firstMouse = false;
		}

		mouseDeltaX = xpos - mouseLastX;
		mouseDeltaY = mouseLastY - ypos;

		mouseLastX = xpos;
		mouseLastY = ypos;
	}

	void resetMouseDelta() {
		mouseDeltaX = 0;
		mouseDeltaY = 0;
	}
}