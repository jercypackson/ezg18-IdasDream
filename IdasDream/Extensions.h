#pragma once

#include <glm/glm.hpp>
#include <assimp\types.h>
#include <string>

class Extensions
{
public:
	Extensions();
	~Extensions();

	static glm::vec3 toGlmVec3(const aiVector3D& aiVec3);
	static glm::vec3 toGlmVec3(const aiColor4D& aiCol4);
	static glm::mat4 toGlmMat4(const aiMatrix4x4& aiMat4);
	static inline const std::string assets = "../assets/";
};

