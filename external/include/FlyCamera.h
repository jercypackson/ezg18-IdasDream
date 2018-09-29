#pragma once

#include <glm\glm.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Camera.h"
#include "Window.h"

class FlyCamera : public Camera
{
private:
	int _centerX, _centerY;
	float _yaw, _pitch;
	float _keyspeed;

public:
	FlyCamera(ProjectionMode projMode);
	~FlyCamera();
	
	void update(const Window& window, float dt) override;
	void registerToWindow(Window& window) override;
	void unregisterFromWindow(Window& window) override;

};