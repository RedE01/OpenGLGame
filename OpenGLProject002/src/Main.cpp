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

	Model terrainModel("res/models/Terrain001.obj");
	GameObject object1(&terrainModel);
	object1.m_translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -15.0f, 0.0f));

	Model shrekModel("res/models/GoodShrek.obj");
	GameObject object2(&shrekModel);
	object2.m_translation = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -5.0f));

	std::string shaderPath = "res/shaders/Basic.shader";
	Shader shader(shaderPath);

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 90.0f, 4.0f / 3.0f, 0.1f, 250.0f, input::mouseX, input::mouseY);

	//glm::mat4 MVP = camera.getProjMat() * camera.getViewMat() * object1.m_translation * object1.m_rotationY;
	GLCall(int viewLocation = glGetUniformLocation(shader.getProgramID(), "u_view"));
	GLCall(glUniformMatrix4fv(viewLocation, 1, false, &camera.getViewMat()[0][0]));

	GLCall(int projLocation = glGetUniformLocation(shader.getProgramID(), "u_proj"));
	GLCall(glUniformMatrix4fv(projLocation, 1, false, &camera.getProjMat()[0][0]));

	glm::mat4 model1 = object1.m_translation * object1.m_rotationX * object1.m_rotationY * object1.m_rotationZ;
	glm::mat4 model2 = object2.m_translation * object2.m_rotationX * object2.m_rotationY * object2.m_rotationZ;
	GLCall(int modelLocation = glGetUniformLocation(shader.getProgramID(), "u_model"));
	GLCall(glUniformMatrix4fv(modelLocation, 1, false, &model1[0][0]));

	GLCall(int lightLocation = glGetUniformLocation(shader.getProgramID(), "u_lightPos"));
	GLCall(glUniform3f(lightLocation, 0.0f, 50.0f, 0.0f));

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


	std::cout << "obj1";
	object1.m_model->printVertCount();
	std::cout << "obj2";
	object2.m_model->printVertCount();

	//glPolygonMode(GL_FRONT, GL_LINE);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		float currentTime = (float)glfwGetTime();
		global::deltaTime = currentTime - global::previousTime;
		global::previousTime = currentTime;

		camera.processInput(window, global::deltaTime);
		camera.rotate(input::mouseDeltaX, input::mouseDeltaY);

		GLCall(glUniformMatrix4fv(viewLocation, 1, false, &camera.getViewMat()[0][0]));
		GLCall(glUniformMatrix4fv(projLocation, 1, false, &camera.getProjMat()[0][0]));

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		input::resetMouseDelta();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}

		//GLCall(glUseProgram(program));
		glBindVertexArray(object1.m_vao);
		model1 = object1.m_translation * object1.m_rotationX * object1.m_rotationY * object1.m_rotationZ;
		GLCall(glUniformMatrix4fv(modelLocation, 1, false, &model1[0][0]));
		object1.draw();

		glBindVertexArray(object2.m_vao);
		object2.m_rotationY = glm::rotate(object2.m_rotationY, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
		//object2.m_translation = glm::translate(object2.m_translation, glm::vec3(0.0f, 0.0f, 0.0f));
		model2 = object2.m_translation * object2.m_rotationX * object2.m_rotationY * object2.m_rotationZ;
		GLCall(glUniformMatrix4fv(modelLocation, 1, false, &model2[0][0]));

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