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
#include "Texture.h"

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
	window = glfwCreateWindow(1280, 720, "OpenGLProject", NULL, NULL);
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

	Texture grassTexture("res/img/Green.png", 1.0f);
	Texture waterTexture("res/img/Blue.png", 1.0f);
	Texture redTexture("res/img/RedThing.png", 1.0f);

	Model terrainModel("res/models/Terrain003.obj", &grassTexture);
	GameObject object1(&terrainModel, 0.0f, -15.0f, 0.0f);
	Model waterModel("res/models/Terrain003Water.obj", &waterTexture);
	GameObject waterObject(&waterModel, 0.0f, -15.0f, 0.0f);

	Model shrekModel("res/models/GoodShrek.obj", &redTexture);
	GameObject object2(&shrekModel, 0.0f, 0.0f, -5.0f);

	Model lightModel("res/models/SickCube.obj", &redTexture);
	GameObject lightObject(&lightModel, 0.0f, 20.0f, 0.0f);

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 90.0f, 4.0f / 3.0f, 0.1f, 250.0f, input::mouseX, input::mouseY);

	std::string shaderPath = "res/shaders/LowPoly.shader";
	Shader shader(shaderPath);
	//Sends view, projection and model matricies to shader
	shader.setUniformMatrix4fv("u_view", &camera.getViewMat()[0][0]);
	shader.setUniformMatrix4fv("u_proj", &camera.getProjMat()[0][0]);

	glm::mat4 model1Mat = object1.m_translation * object1.m_rotationX * object1.m_rotationY * object1.m_rotationZ;
	glm::mat4 model2Mat = object2.m_translation * object2.m_rotationX * object2.m_rotationY * object2.m_rotationZ;
	GLCall(int modelLocation = glGetUniformLocation(shader.getProgramID(), "u_model"));
	GLCall(glUniformMatrix4fv(modelLocation, 1, false, &model1Mat[0][0]));

	GLCall(int lightLocation = glGetUniformLocation(shader.getProgramID(), "u_lightPos"));

	GLCall(int cameraLocation = glGetUniformLocation(shader.getProgramID(), "u_cameraPos"));
	GLCall(glUniform3fv(cameraLocation, 1, &camera.getPos()[0]));

	//Tells OpenGL to not render backs of triangles or objects behind other objects
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//Makes OpenGL render lines instead of filled triangles
	//glPolygonMode(GL_FRONT, GL_LINE);

	double timer = 0;
	unsigned int frameCounter = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		//set's deltaTime
		float currentTime = (float)glfwGetTime();
		global::deltaTime = currentTime - global::previousTime;
		global::previousTime = currentTime;
		
		//calculate fps
		timer += global::deltaTime;
		frameCounter++;
		if (timer >= 1.0) {
			timer -= 1.0;
			std::cout << frameCounter << std::endl;
			frameCounter = 0;
		}

		//keyboard and mouse movement
		camera.processInput(window, global::deltaTime);
		camera.rotate(input::mouseDeltaX, input::mouseDeltaY);

		//Closes window on esc
		input::resetMouseDelta();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, true);
		}

		//Update view matrix uniform in shader
		shader.setUniformMatrix4fv("u_view", &camera.getViewMat()[0][0]);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Binds model1s vertex and index array
		glBindVertexArray(object1.m_vao);
		//update modelMat and sends it to shader
		GLCall(glUniformMatrix4fv(modelLocation, 1, false, &model1Mat[0][0]));
		object1.draw(shader);
		glBindVertexArray(waterObject.m_vao);
		waterObject.draw(shader);

		glBindVertexArray(object2.m_vao);
		object2.m_rotationY = glm::rotate(object2.m_rotationY, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
		model2Mat = object2.m_translation * object2.m_rotationX * object2.m_rotationY * object2.m_rotationZ;
		GLCall(glUniformMatrix4fv(modelLocation, 1, false, &model2Mat[0][0]));
		object2.draw(shader);

		glBindVertexArray(lightObject.m_vao);
		glm::mat4 lObj = lightObject.m_translation * lightObject.m_rotationX * lightObject.m_rotationY * lightObject.m_rotationZ;
		GLCall(glUniformMatrix4fv(modelLocation, 1, false, &lObj[0][0]));
		lightObject.draw(shader);
		lightObject.move(0.0f, 0.0f, 0.0f);
		GLCall(glUniform3fv(lightLocation, 1, &lightObject.m_pos[0]));

		/* Swap front and back buffers */
		GLCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		GLCall(glfwPollEvents());
	}

	GLCall(glDeleteProgram(shader.getProgramID()));

	glfwTerminate();
	return 0;
}