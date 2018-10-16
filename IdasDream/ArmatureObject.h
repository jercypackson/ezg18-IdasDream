#pragma once
#include "SceneObject.h"
class ArmatureObject :
	public SceneObject
{
public:
	ArmatureObject(std::string name, glm::mat4 modelMatrix, SceneObject* parent);
	~ArmatureObject();

	void setOffsetMatrix(glm::mat4 offsetMatrix);

	SceneObject* createChild(std::string name, glm::mat4 modelMatrix, SceneObject* parent) override;

	void setBoneIdx(unsigned int boneIdx);

	void setBones(std::vector<glm::mat4>& bd) override;


private:

	glm::mat4 _offsetMatrix = glm::mat4(1.0f);

	unsigned int _boneIdx;
	bool _isBone = false;
};

