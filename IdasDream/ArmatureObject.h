#pragma once
#include "SceneObject.h"
class ArmatureObject :
	public SceneObject
{
public:
	ArmatureObject(std::string name, glm::mat4 modelMatrix, SceneObject* parent);
	~ArmatureObject();

	void setOffsetMatrix(glm::mat4 offsetMatrix);

private:
	glm::mat4 _offsetMatrix;
};

