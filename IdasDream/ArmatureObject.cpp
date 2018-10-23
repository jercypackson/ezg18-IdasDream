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

	glm::mat4 v;

	if (_globalInverseArm) {
		v = getModelMatrix() * _offsetMatrix * _globalInverseArm.value();
	}
	else {
		v = getModelMatrix() * _offsetMatrix;
	}


	bd[id] = v;
}

void ArmatureObject::addAnimation(std::string name, Animation anim)
{
	animations[name] = anim;
}

void ArmatureObject::animate(float time)
{
	if (!animations.empty()) {
		//

		/*
		std::string name = "Armature|ArmatureActionadf";
		auto anim = animations[name];
		/*/
		auto anim = animations.begin()->second;
		//*/

		auto mm = anim.getCurrentMatrix(time);
		if (mm) {
			setLocalModelMatrix(mm.value());
		}
	}
}

glm::mat4 ArmatureObject::getLocalModelMatrix()
{
	return _localModelMatrix;
}

bool ArmatureObject::setGlobalInverse(glm::mat4 gi)
{
	//todo: replace with if has bone
	if (_globalInverseArm == std::nullopt && _name == "Body") {
		_globalInverseArm = gi;
		return true;
	}
	return false;
}
