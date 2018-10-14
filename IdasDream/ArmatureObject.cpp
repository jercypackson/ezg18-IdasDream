#include "pch.h"
#include "ArmatureObject.h"


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
