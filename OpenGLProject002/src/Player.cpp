#include "Player.h"
#include "Input.h"
#include "Time.h"
#include <algorithm>

Player::Player(Camera& camera, float x, float y, float z, float height, float speed, Terrain* terrainObj) 
		: m_camera(&camera), m_pos(x, y, z), m_height(height), m_speed(speed), m_terrainObj(terrainObj) {

}

Player::~Player() {
}

void Player::Update() {
	calculateMovement();
	calculateTerrainCollision();

	m_camera->setPos(m_pos.x, m_pos.y + m_height, m_pos.z);
	m_camera->rotate(input::mouseDeltaX, input::mouseDeltaY);
}

void Player::calculateMovement() {
	float moveSpeed = m_speed * time::deltaTime;
	glm::vec3 rawMovement(0.0f);
	if (input::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		moveSpeed *= 1.5f;
	}
	if (input::isKeyDown(GLFW_KEY_W)) {
		rawMovement.z += moveSpeed;
	}
	if (input::isKeyDown(GLFW_KEY_S)) {
		rawMovement.z -= moveSpeed;
	}
	if (input::isKeyDown(GLFW_KEY_A)) {
		rawMovement.x -= moveSpeed;
	}
	if (input::isKeyDown(GLFW_KEY_D)) {
		rawMovement.x += moveSpeed;
	}
	if (input::isKeyDown(GLFW_KEY_SPACE)) {
		if (!jumpKeyPressed) {
			if(isGrounded)
				m_force.y = 0.08f;
		}
		jumpKeyPressed = true;
	}
	else {
		jumpKeyPressed = false;
	}


	if (rawMovement.x) {
		glm::vec3 movement = glm::cross(m_camera->cameraFront, m_camera->getCameraUp());
		movement = glm::normalize(glm::vec3(movement.x, 0, movement.z));
		m_pos += movement * rawMovement.x;
	}
	if (rawMovement.y) {
		glm::vec3 movement = glm::normalize(glm::vec3(m_camera->getCameraUp()));
		m_pos += movement * rawMovement.y;
	}
	if (rawMovement.z) {
		glm::vec3 movement = glm::normalize(glm::vec3(m_camera->cameraFront.x, 0, m_camera->cameraFront.z));
		m_pos += movement * rawMovement.z;
	}
}

void Player::calculateTerrainCollision() {
	unsigned int hTerrainSize = 100;
	unsigned int tVertX = std::floor(m_pos.x + hTerrainSize);
	unsigned int tVertZ = std::floor(m_pos.z + hTerrainSize);
	tVertX = std::max(1.0f, std::min((float)tVertX, (float)(hTerrainSize * 2 - 1)));
	tVertZ = 200 - std::max(1.0f, std::min((float)tVertZ, (float)(hTerrainSize  * 2 - 1)));
	float dX = m_pos.x - std::floor(m_pos.x);
	float dZ = m_pos.z - std::floor(m_pos.z);

	float hX1 = m_terrainObj->getHeight(tVertZ, tVertX) * dX + m_terrainObj->getHeight(tVertZ + 1,     tVertX) * (1.0f - dX);
	float hX2 = m_terrainObj->getHeight(tVertZ, tVertX + 1) * dX + m_terrainObj->getHeight(tVertZ + 1, tVertX + 1) * (1.0f - dX);
	float hFinal = hX1 * (dZ)+hX2 * (1.0f - dZ) - 13.0f;

	if (m_pos.y > hFinal) {
		isGrounded = false;
		m_force = glm::vec3(m_force.x, m_force.y - 0.2f * time::deltaTime, m_force.z);
	}

	m_pos.y += m_force.y;

	if (m_pos.y <= hFinal) {
		m_force = { 0.0f, 0.0f, 0.0f };
		m_pos = { m_pos.x, hFinal, m_pos.z };
	}
	if (m_pos.y - hFinal < 0.2f) {
		isGrounded = true;
	}
}
