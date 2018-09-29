#pragma once

#include <glm/glm.hpp>
#include <assimp\vector3.h>

class Extensions
{
public:
	Extensions();
	~Extensions();


	static glm::vec3 toGlmVec3(const aiVector3D& aiVec3);
};

