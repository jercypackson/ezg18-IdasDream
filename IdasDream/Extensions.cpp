#include "pch.h"
#include "Extensions.h"


Extensions::Extensions()
{
}


Extensions::~Extensions()
{
}

glm::vec3 Extensions::toGlmVec3(const aiVector3D& aiVec3)
{
	return glm::vec3(aiVec3.x, aiVec3.y, aiVec3.z);
}
