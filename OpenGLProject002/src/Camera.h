#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

class Camera {
private:
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_direction = glm::vec3(0.0f);

	glm::mat4 m_proj;
	glm::mat4 m_view;

	float m_sensitivity = 0.1f;
public:
	float yaw;
	float pitch;
	glm::vec3 cameraFront;
public:
	Camera(glm::vec3 pos, float yaw, float pitch, float fov, float aspectRatio, float nearPlane, float farPlane, float mouseX, float mouseY);

	void processInput(GLFWwindow* window, float& deltaTime);
	void move(float x, float y, float z);
	void rotate(float rotX, float rotY);

	inline glm::mat4 getProjMat() {
		return m_proj;
	}
	inline glm::mat4 getViewMat() {
		return m_view;
	}
	inline glm::vec3 getPos() {
		return m_cameraPos;
	}
};