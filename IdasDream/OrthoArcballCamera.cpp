#include "pch.h"
#include "OrthoArcballCamera.h"


OrthoArcballCamera::OrthoArcballCamera(OrthographicProjection op)
	: Camera(ProjectionMode(op.left, op.right, op.bottom, op.top, op.near, op.far)), _keyspeed(0.1f), _mouseX(0), _mouseY(0), _yaw(0.0f), _pitch(0.0f), _strafe(glm::vec3(0))
{
}


OrthoArcballCamera::~OrthoArcballCamera()
{
}

void OrthoArcballCamera::update(const Window& window, float dt)
{
	int x, y;
	window.getCursorPosition(x, y);

	int dx = x - _mouseX;
	int dy = y - _mouseY;
	float speed = 0.005f;
	glm::vec3 pos;

	if (_dragging) {
		_yaw += dx * speed;
		_pitch += dy * speed;

		_pitch = glm::min(_pitch, glm::pi<float>()*0.5f - 0.01f);
		_pitch = glm::max(_pitch, -glm::pi<float>()*0.5f + 0.01f);
	}

	pos.x = /*getZoom() **/ glm::cos(_pitch) * -glm::sin(_yaw);
	pos.y = /*getZoom() **/ glm::sin(_pitch);
	pos.z = /*getZoom() **/ glm::cos(_pitch) * glm::cos(_yaw);
	_position = pos;
	_direction = -glm::normalize(pos);

	if (_strafing) {
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = glm::normalize(glm::cross(-pos, up));
		up = glm::normalize(glm::cross(right, -pos));

		float strafeSpeed = speed /** this->getZoom()*/ * 0.1f;
		_strafe += up * float(dy) * strafeSpeed + right * -float(dx) * strafeSpeed;
	}

	_position = _position + _strafe;

	/*glm::vec3 left = -glm::cross(_direction, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 up = glm::cross(_direction, left);

	if (window.isKeyPressed(KeyInput::Key::W)) {
		_position += _keyspeed * _direction;
	}
	else if (window.isKeyPressed(KeyInput::Key::S)) {
		_position -= _keyspeed * _direction;
	}
	if (window.isKeyPressed(KeyInput::Key::A)) {
		_position += _keyspeed * left;
	}
	else if (window.isKeyPressed(KeyInput::Key::D)) {
		_position -= _keyspeed * left;
	}
	if (window.isKeyPressed(KeyInput::Key::SPACE)) {
		_position += _keyspeed * up;
	}
	else if (window.isKeyPressed(KeyInput::Key::LEFT_SHIFT)) {
		_position -= _keyspeed * up;
	}*/


	_viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), _position) * glm::yawPitchRoll(-_yaw, -_pitch, 0.0f));

	_mouseX = x;
	_mouseY = y;
}

void OrthoArcballCamera::registerToWindow(Window & window)
{
	if (_registeredMouseCallbacks.size() == 1) return;

	ID id = window.registerMouseInputHandler([this](const MouseInput& inp) {
		if (inp.button == MouseInput::Button::LEFT) {
			//if (inp.action == MouseInput::Action::PRESSED)  this->setDragging(true);
			//else this->setDragging(false);
		}
		else if (inp.button == MouseInput::Button::RIGHT) {
			//if (inp.action == MouseInput::Action::PRESSED) this->setStrafing(true);
			//else this->setStrafing(false);
		}

		if (inp.action == MouseInput::Action::SCROLLED) {

			auto op = _projectionMode.getOrthographicProjection();
			op.left		-= inp.scrollDelta;
			op.right	+= inp.scrollDelta;
			op.bottom	-= inp.scrollDelta * 9.f / 16.f;
			op.top		+= inp.scrollDelta * 9.f / 16.f;

			//_projectionMode = ProjectionMode(op);
			_projMatrix = _projectionMode.getProjectionMatrix();
		}
	});

	_registeredMouseCallbacks.push_back(id);


	window.registerKeyInputHandler([this](const KeyInput& input) {
		if (input.action != KeyInput::Action::RELEASED) return;

		if (input.key == KeyInput::Key::UP)
			_keyspeed *= 2.0f;
		else if (input.key == KeyInput::Key::DOWN)
			_keyspeed *= 0.5f;
		//else if (input.key == KeyInput::Key::LEFT)
		//	this->setZoom(this->getZoom() - 0.5f * this->getZoom() * 0.1f);
		//else if (input.key == KeyInput::Key::RIGHT)
		//	this->setZoom(this->getZoom() + 0.5f * this->getZoom() * 0.1f);
	});
}
