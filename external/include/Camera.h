#pragma once

#include <memory>

#include <glm\glm.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Window.h"

class Camera
{
private:
	bool _dragging;
	bool _strafing;
	glm::mat4 _viewMatrix;
	glm::mat4 _projMatrix;
	int _mouseX, _mouseY;
	float _yaw, _pitch;
	float _zoom;
	glm::vec3 _position;
	glm::vec3 _direction;
	glm::vec3 _strafe;
	glm::vec4 _projSettings;
	
public:
	Camera(float fov, float aspect, float near, float far);
	~Camera();

	void setDragging(bool dragging) { _dragging = dragging; }
	void setStrafing(bool strafing) { _strafing = strafing; }
	void setZoom(float zoom) { _zoom = zoom; }
	float getZoom() { return _zoom; }
	glm::vec4 getProjectionSettings() { return _projSettings; } // x = fov, y = aspect, z = near, w = far

	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewProjectionMatrix();
	void update(int x, int y);

	void registerToWindow(Window& window);
};