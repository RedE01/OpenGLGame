#include "Camera.h"

Camera::Camera(glm::vec3 pos, float yaw, float pitch, float fov, float aspectRatio, float nearPlane, float farPlane, float mouseX, float mouseY)
		: m_cameraPos(pos), yaw(yaw -90), pitch(pitch), m_proj(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane)),
			m_view(glm::lookAt(m_cameraPos, m_cameraPos + cameraFront, m_cameraUp)), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)) {

}

void Camera::processInput(GLFWwindow* window, float& deltaTime) {
	float movementSpeed = 10.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W)) {
		move(0.0f, 0.0f, movementSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		move(-movementSpeed, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		move(0.0f, 0.0f, -movementSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		move(movementSpeed, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		move(0.0f, -movementSpeed, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		move(0.0f, movementSpeed, 0.0f);
	}
}

void Camera::move(float x, float y, float z) {
	if (x) {
		glm::vec3 movement = glm::cross(cameraFront, m_cameraUp);
		movement = glm::normalize(glm::vec3(movement.x, movement.y, movement.z));
		m_cameraPos += movement * x;
	}
	if (y) {
		glm::vec3 movement = glm::normalize(glm::vec3(m_cameraUp.x, m_cameraUp.y, m_cameraUp.z));
		m_cameraPos += movement * y;
	}
	if (z) {
		glm::vec3 movement = glm::normalize(glm::vec3(cameraFront.x, cameraFront.y, cameraFront.z));
		m_cameraPos += movement * z;
	}
}

void Camera::rotate(float rotX, float rotY) {
	rotX *= m_sensitivity;
	rotY *= m_sensitivity;

	yaw += rotX;
	pitch += rotY;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	m_direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	m_direction.y = sin(glm::radians(pitch));
	m_direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	m_view = glm::lookAt(m_cameraPos, m_cameraPos + cameraFront, m_cameraUp);
}
