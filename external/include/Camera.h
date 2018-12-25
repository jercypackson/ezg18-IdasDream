#pragma once

#include <type_traits>

#include <glm\glm.hpp>
#include <glm\gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Window.h"


struct PerspectiveProjection {
	float fov;
	float aspect;
	float near;
	float far;
};

struct OrthographicProjection {
	float left;
	float right;
	float bottom;
	float top;
	float near;
	float far;
};

struct ProjectionMode {
private:
	PerspectiveProjection _perspectiveProjection;
	OrthographicProjection _orthographicProjection;
	bool _isPerspective;

public:
	ProjectionMode(float fov, float aspect, float near, float far) 
		: _perspectiveProjection({ glm::radians(fov), aspect, near, far }), _isPerspective(true)
	{
	}

	ProjectionMode(PerspectiveProjection perspectiveProejction)
		: _perspectiveProjection(perspectiveProejction), _isPerspective(true)
	{
	}

	ProjectionMode(float left, float right, float bottom, float top, float near, float far)
		: _orthographicProjection({ left, right, bottom, top, near, far }), _isPerspective(false)
	{
	}

	ProjectionMode(OrthographicProjection orthographicProjection)
		: _orthographicProjection(orthographicProjection), _isPerspective(false)
	{
	}

	bool isPerspective() { return _isPerspective; }
	bool isOrthographic() { return !_isPerspective; }

	PerspectiveProjection getPerspectiveProjection() { return _perspectiveProjection; }
	OrthographicProjection getOrthographicProjection() { return _orthographicProjection; }

	glm::mat4 getProjectionMatrix() {
		if (isPerspective())
			return glm::perspective(
						_perspectiveProjection.fov, 
						_perspectiveProjection.aspect, 
						_perspectiveProjection.near, 
						_perspectiveProjection.far);
		else
			return glm::ortho(
						_orthographicProjection.left, 
						_orthographicProjection.right, 
						_orthographicProjection.bottom, 
						_orthographicProjection.top, 
						_orthographicProjection.near, 
						_orthographicProjection.far);
	}

};

class Camera
{
protected:
	glm::mat4 _viewMatrix = glm::mat4(1.0f);
	glm::mat4 _projMatrix = glm::mat4(1.0f);
	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _direction = glm::vec3(0.0f);
	ProjectionMode _projectionMode;

	std::vector<ID> _registeredKeyCallbacks;
	std::vector<ID> _registeredMouseCallbacks;

public:
	Camera(ProjectionMode projMode) 
		: _projectionMode(projMode), _projMatrix(projMode.getProjectionMatrix())
	{}

	Camera(glm::mat4 viewMatrix, glm::vec3 position, glm::vec3 direction, ProjectionMode projMode) 
		: _viewMatrix(viewMatrix), _projMatrix(projMode.getProjectionMatrix()), _position(position), _direction(direction), _projectionMode(projMode)
	{}

	ProjectionMode getProjectionMode() { return _projectionMode; }
	glm::vec3 getPosition() { return _position; }
	glm::vec3 getDirection() { return _direction; }
	glm::mat4 getViewMatrix() { return _viewMatrix; }
	glm::mat4 getProjectionMatrix() { return _projMatrix; }
	glm::mat4 getViewProjectionMatrix() { return _projMatrix * _viewMatrix; }

	virtual void update(const Window& window, float dt) = 0;
	virtual void registerToWindow(Window& window) = 0;
	virtual void unregisterFromWindow(Window& window) = 0;
};