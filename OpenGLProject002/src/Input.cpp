#include "Input.h"

namespace input {
	GLFWwindow* window;
	float mouseX = 0;
	float mouseY = 0;
	
	float mouseLastX = 0;
	float mouseLastY = 0;

	float mouseDeltaX = 0;
	float mouseDeltaY = 0;

	bool firstMouse = true;

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

	extern bool isKeyDown(int key) {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}
}