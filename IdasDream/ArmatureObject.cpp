#include "pch.h"
#include "ArmatureObject.h"
#include "Bones.h"

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
	bd[id] = getModelMatrix() * _offsetMatrix;
}

void ArmatureObject::addAnimation(std::string name, Animation anim)
{
	animations[name] = anim;
}

void ArmatureObject::animate(float time)
{
	std::string name = "Armature|WalkingCycle.1";
	auto anim = animations[name];

	auto mm = anim.getCurrentMatrix(time);
	if (mm) {
		setLocalModelMatrix(mm.value());
	}
}
