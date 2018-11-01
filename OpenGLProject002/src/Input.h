#pragma once
#include <GLFW\glfw3.h>

namespace input {
	extern GLFWwindow* window;
	extern float mouseX;
	extern float mouseY;

	extern float mouseLastX;
	extern float mouseLastY;

	extern float mouseDeltaX;
	extern float mouseDeltaY;

	extern bool firstMouse;

	extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	extern void resetMouseDelta();

	extern bool isKeyDown(int key);
}