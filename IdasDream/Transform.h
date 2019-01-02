#pragma once

#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

struct Transform
{
	glm::vec3 pos;
	glm::vec3 rot;
	float scale;

	glm::quat quat;

	Transform() {
		pos = glm::vec3(0.0f);
		quat = glm::quat();
		rot = glm::eulerAngles(quat);
		scale = 1;
	}

	Transform(glm::vec3 pos, float sc = 1) {
		this->pos = pos;
		quat = glm::quat();
		rot = glm::eulerAngles(quat);
		scale = sc;
	}

	Transform(glm::vec3 pos, glm::vec3 rot, float sc = 1) {
		this->pos = pos;
		this->rot = rot;
		quat = glm::quat(rot);
		scale = sc;
	}

	Transform(glm::vec3 pos, glm::quat quat, float sc = 1) {
		this->pos = pos;
		this->quat = quat;
		rot = glm::eulerAngles(quat);
		scale = sc;
	}

	glm::mat4 getMatrix() {
		return glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(quat);
	}

	static Transform mix(Transform t1, Transform t2, float delta) {
		return Transform(glm::mix(t1.pos, t2.pos, delta), glm::slerp(t1.quat, t2.quat, delta), glm::mix(t1.scale, t2.scale, delta));
	}
};