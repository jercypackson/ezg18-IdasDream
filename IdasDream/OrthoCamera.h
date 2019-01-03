#pragma once

#include <glm\glm.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Camera.h"
#include "Transform.h"

class OrthoCamera : public Camera
{
protected:
	bool _dragging = false;
	bool _strafing = false;
	int _mouseX, _mouseY;
	float _yaw, _pitch;
	float _zoom;
	glm::vec3 _strafe;

	float _speed;
	float _resolution;

	float _orthoScale;

	OrthographicProjection _defaultOrthoProj;

	void setOrthoScale(float orthoScale);

	void calcDir();
	void updateViewMat();

public:
	OrthoCamera(OrthographicProjection op);
	~OrthoCamera();

	void setDragging(bool dragging) { _dragging = dragging; }
	void setStrafing(bool strafing) { _strafing = strafing; }
	void setZoom(float zoom) { _zoom = zoom; }
	float getZoom() { return _zoom; }


	void update(const Window& window, float dt) override;
	virtual void update(Transform transform) {};
	void registerToWindow(Window& window) override;
	void unregisterFromWindow(Window& window) override;

	glm::vec3 getRot() {
		return glm::vec3(_pitch, _yaw, 0);
	}
};


class OrthoCameraAnimated : public OrthoCamera
{
public:
	OrthoCameraAnimated(OrthographicProjection op);
	void update(Transform transform) override;
};