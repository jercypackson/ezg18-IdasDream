#pragma once
#include <Camera.h>
class OrthoArcballCamera :
	public Camera
{
public:
	OrthoArcballCamera(OrthographicProjection op);
	~OrthoArcballCamera();
	void update(const Window& window, float dt) override;

	void registerToWindow(Window& window) override;
	void unregisterFromWindow(Window& window) override {};


private:
	float _keyspeed;

	bool _dragging;
	bool _strafing;
	int _mouseX, _mouseY;
	float _yaw, _pitch;
	glm::vec3 _strafe;

};

