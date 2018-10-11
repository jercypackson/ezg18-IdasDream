#include "pch.h"
#include "AnimatedCamera.h"

AnimatedCamera::AnimatedCamera(ProjectionMode projMode)
	: Camera(projMode) { }

AnimatedCamera::~AnimatedCamera()
{
}

void AnimatedCamera::update(Transform transform)
{
	_position = transform.pos;
	_direction = transform.rot;
	_viewMatrix = glm::inverse(transform.getMatrix());
}

void AnimatedCamera::registerToWindow(Window & window)
{
	//window.hideCursor();
}

void AnimatedCamera::unregisterFromWindow(Window & window)
{
	//window.showCursor();
}
