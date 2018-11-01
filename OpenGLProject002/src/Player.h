#pragma once
#include "Camera.h"
#include <glm\glm.hpp>
#include "GameObject.h"

class Player {
private:
	Camera* m_camera;
	glm::vec3 m_pos;
	glm::vec3 m_force;
	float m_height;
	float m_speed;
	GameObject* m_terrainObj;
	bool isGrounded;
	bool jumpKeyPressed;
public:
	Player(Camera& camera, float x, float y, float z, float height, float speed, GameObject* terrainObj);
	~Player();

	void Update();
private:
	void calculateMovement();
	void calculateTerrainCollision();
};