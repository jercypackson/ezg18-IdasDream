#include "pch.h"
#include "ArmatureObject.h"
#include "Bones.h"
#include <glm\gtc\matrix_transform.hpp>
#include "Extensions.h"

ArmatureObject::ArmatureObject(std::string name, glm::mat4 modelMatrix, SceneObject* parent)
	: SceneObject(name, modelMatrix, parent)
{
}

ArmatureObject::~ArmatureObject()
{
}

void ArmatureObject::setOffsetMatrix(glm::mat4 offsetMatrix)
{
	_offsetMatrix = offsetMatrix;
}

SceneObject * ArmatureObject::createChild(std::string name, glm::mat4 modelMatrix, SceneObject * parent)
{
	return new ArmatureObject(name, modelMatrix, parent);
}

void ArmatureObject::setBoneIdx(unsigned int boneIdx)
{
	_boneIdx = boneIdx;
	_isBone = true;
}

void ArmatureObject::setBones(std::vector<glm::mat4>& bd)
{
	auto id = Bones::getBone(_name);
	if (id < 0) return;

	glm::mat4 v = getModelMatrix() * _offsetMatrix;
	if (_globalInverseArm) {
		v = v * _globalInverseArm.value();
	}

	bd[id] = v;
}

void ArmatureObject::addAnimation(std::string name, Animation anim)
{
	animations[name] = anim;
}

void ArmatureObject::animate(float time)
{
	if (_animationTime.empty() || time < _animationTime.front()) return;

	auto lessOrEqual = std::prev(std::upper_bound(_animationTime.begin(), _animationTime.end(), time), 1);
	auto lessOrEqualIdx = lessOrEqual - _animationTime.begin();

	auto name = _animationName[lessOrEqualIdx];

	auto mm = animations[name].getCurrentMatrix(time - *lessOrEqual);
	if (mm) {
		setLocalModelMatrix(mm.value());
	}
}

glm::mat4 ArmatureObject::getLocalModelMatrix()
{
	return _localModelMatrix;
}

bool ArmatureObject::setGlobalInverse(glm::mat4 gi)
{
	if (_name == "Body") { //idk why this works but i'm not complaining
		_globalInverseArm = gi;
		return true;
	}
	return false;
}

void ArmatureObject::addAnimationSequence(std::vector<float> time, std::vector<std::string> name)
{
	_animationTime = time;
	_animationName = name;
}
