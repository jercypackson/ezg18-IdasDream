#include "pch.h"
#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(OrthographicProjection op)
	: Camera({ op.left, op.right, op.bottom, op.top, op.near, op.far }),
	_mouseX(0), _mouseY(0), _yaw(0.0f), _pitch(0.0f), _strafe(glm::vec3(0)), _zoom(15.0f), _speed(0.005f)
{	
	_resolution = (abs(op.left) + abs(op.right)) / (abs(op.bottom) + abs(op.top));
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::update(const Window& window, float dt)
{
	int x, y;
	window.getCursorPosition(x, y);

	int dx = x - _mouseX;
	int dy = y - _mouseY;

	_mouseX = x;
	_mouseY = y;

	glm::vec3 pos;


	if (_dragging) {
		_yaw += dx * _speed;
		_pitch += dy * _speed;

		_pitch = glm::min(_pitch, glm::pi<float>()*0.5f - 0.01f);
		_pitch = glm::max(_pitch, -glm::pi<float>()*0.5f + 0.01f);
	}

	pos.x = glm::cos(_pitch) * -glm::sin(_yaw);
	pos.y = glm::sin(_pitch);
	pos.z = glm::cos(_pitch) * glm::cos(_yaw);
	_direction = -glm::normalize(pos);

	glm::vec3 left = -glm::cross(_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 up = glm::cross(_direction, left);

	if (window.isKeyPressed(KeyInput::Key::W)) {
		_position += _speed * 100 * _direction;
	}
	else if (window.isKeyPressed(KeyInput::Key::S)) {
		_position -= _speed * 100 * _direction;
	}

	if (_strafing) {
		_position += dx * _speed * left;
		_position += dy * _speed * up;
	}

	_viewMatrix = glm::lookAt(_position, _position + _direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

void OrthoCamera::registerToWindow(Window& window)
{
	if (_registeredMouseCallbacks.size() == 1) return;

	ID id = window.registerMouseInputHandler([this](const MouseInput& inp) {
		if (inp.button == MouseInput::Button::LEFT) {
			if (inp.action == MouseInput::Action::PRESSED)  this->setDragging(true);
			else this->setDragging(false);
		}
		else if (inp.button == MouseInput::Button::RIGHT) {
			if (inp.action == MouseInput::Action::PRESSED) this->setStrafing(true);
			else this->setStrafing(false);
		}

		if (inp.action == MouseInput::Action::SCROLLED) {

			auto op = _projectionMode.getOrthographicProjection();
			op.left += inp.scrollDelta;
			op.right -= inp.scrollDelta;
			op.bottom += inp.scrollDelta / _resolution;
			op.top -= inp.scrollDelta / _resolution;

			_projectionMode = ProjectionMode({ op.left, op.right, op.bottom, op.top, op.near, op.far });
			_projMatrix = _projectionMode.getProjectionMatrix();
		}
	});

	_registeredMouseCallbacks.push_back(id);

	window.registerKeyInputHandler([this](const KeyInput& input) {
		if (input.action != KeyInput::Action::RELEASED) return;

		if ((int)input.key == 93) // +
			_speed *= 2.0f;
		else if ((int)input.key == 47) // -
			_speed *= 0.5f;
		else if (input.key == KeyInput::Key::UP)
			this->setZoom(this->getZoom() - 0.5f * this->getZoom() * 0.1f);
		else if (input.key == KeyInput::Key::DOWN)
			this->setZoom(this->getZoom() + 0.5f * this->getZoom() * 0.1f);
	});
}

void OrthoCamera::unregisterFromWindow(Window& window)
{
	if (_registeredMouseCallbacks.size() == 0) return;

	window.unregisterMouseInputHandler(_registeredMouseCallbacks[0]);
	//window.unregisterKeyInputHandler
	_registeredMouseCallbacks.clear();
}