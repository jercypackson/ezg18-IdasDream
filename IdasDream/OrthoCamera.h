#pragma once

#include <glm\glm.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Camera.h"

class OrthoCamera : public Camera
{
private:
	bool _dragging;
	bool _strafing;
	int _mouseX, _mouseY;
	float _yaw, _pitch;
	float _zoom;
	glm::vec3 _strafe;

	float _speed;
	float _resolution;
	
public:
	OrthoCamera(OrthographicProjection op);
	~OrthoCamera();

	void setDragging(bool dragging) { _dragging = dragging; }
	void setStrafing(bool strafing) { _strafing = strafing; }
	void setZoom(float zoom) { _zoom = zoom; }
	float getZoom() { return _zoom; }

	void update(const Window& window, float dt) override;
	void registerToWindow(Window& window) override;
	void unregisterFromWindow(Window& window) override;

	glm::vec3 getRot() {
		return glm::vec3(_pitch, _yaw, 0);
	}
};