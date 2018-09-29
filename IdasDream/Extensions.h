#pragma once

#include <glm/glm.hpp>
#include <assimp\vector3.h>
#include <string>

class Extensions
{
public:
	Extensions();
	~Extensions();

	static glm::vec3 toGlmVec3(const aiVector3D& aiVec3);

	static inline const std::string assets = "../assets/";
};

