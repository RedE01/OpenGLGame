#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <stb_image\stb_image.h>
#include "Debug.h"

#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Input.h"
#include "GameObject.h"

namespace global {
	float deltaTime = 0.0f;
	float previousTime = 0.0f;
}

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 720, "OpenGLProject", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, input::mouse_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::GLEW_INIT_FAILED!" << std::endl;
		glfwTerminate();
	}

	Model shrekModel("res/models/GoodShrek.obj");
	GameObject object1(&shrekModel);
	GameObject object2(&shrekModel);

	std::string shaderPath = "res/shaders/Basic.shader";
	Shader shader(shaderPath);

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 90.0f, 4.0f / 3.0f, 0.1f, 250.0f, input::mouseX, input::mouseY);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 MVP = camera.getProjMat() * camera.getViewMat() * model * rotationY;

	GLCall(int mvpLocation = glGetUniformLocation(shader.getProgramID(), "u_MVP"));
	GLCall(glUniformMatrix4fv(mvpLocation, 1, false, &MVP[0][0]));

	int width = 0;
	int height = 0;
	int bpp = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("res/img/Grass.png", &width, &height, &bpp, 4);

	unsigned int texture;
	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (data) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		std::cout << "FAILED TO READ IMAGE" << std::endl;
	}
	stbi_image_free(data);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glPolygonMode(GL_FRONT, GL_LINE);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		float currentTime = (float)glfwGetTime();
		global::deltaTime = currentTime - global::previousTime;
		global::previousTime = currentTime;

		camera.processInput(window, global::deltaTime);
		camera.rotate(input::mouseDeltaX, input::mouseDeltaY);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -5.0f));
		rotationY = glm::rotate(rotationY, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = camera.getProjMat() * camera.getViewMat() * model * rotationX * rotationY * rotationZ;
		GLCall(glUniformMatrix4fv(mvpLocation, 1, false, &MVP[0][0]));

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		input::resetMouseDelta();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}

		//GLCall(glUseProgram(program));
		object1.draw();

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -5.0f));
		MVP = camera.getProjMat() * camera.getViewMat() * model * rotationX * rotationY * rotationZ;
		GLCall(glUniformMatrix4fv(mvpLocation, 1, false, &MVP[0][0]));

		object2.draw();

		/* Swap front and back buffers */
		GLCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		GLCall(glfwPollEvents());
	}

	GLCall(glDeleteProgram(shader.getProgramID()));

	glfwTerminate();
	return 0;
}