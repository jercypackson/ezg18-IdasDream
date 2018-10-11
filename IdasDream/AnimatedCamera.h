#pragma once

#include "Camera.h"
#include "Transform.h"

class AnimatedCamera : public Camera
{
public:
	AnimatedCamera(ProjectionMode projMode);
	~AnimatedCamera();

	void update(Transform transform);

	void registerToWindow(Window& window) override;
	void unregisterFromWindow(Window& window) override;

private:
	void update(const Window& window, float dt) override { };
};

