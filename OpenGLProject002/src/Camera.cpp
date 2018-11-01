#include "Camera.h"
#include <iostream>
#include "Time.h"

Camera::Camera(glm::vec3 pos, float yaw, float pitch, float fov, float aspectRatio, float nearPlane, float farPlane, float mouseX, float mouseY)
		: m_cameraPos(pos), m_cameraForce(0.0f, 0.0f, 0.0f), yaw(yaw -90), pitch(pitch), m_proj(glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane)),
			m_view(glm::lookAt(m_cameraPos, m_cameraPos + cameraFront, m_cameraUp)), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)) {

}


void Camera::setPos(float x, float y, float z) {
	m_cameraPos = { x, y, z };
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

void Camera::addForce(float x, float y, float z) {
	m_cameraForce = glm::vec3( m_cameraForce.x + x, m_cameraForce.y + y, m_cameraForce.z + z );
}

void Camera::setForce(float x, float y, float z) {
	m_cameraForce = { x, y, z };
}
