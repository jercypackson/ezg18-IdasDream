#pragma once

#include <glm\glm.hpp>

struct VertData {
	glm::mat4 modelMatix;
	glm::mat4 normalMatrix;
};

struct FragData {
	glm::vec4 col;					//16 byte
	std::uint64_t textureBuffer;    //8 byte
	std::uint64_t _padding;         //8 byte padding
};

struct VertexBoneData
{
	unsigned int id;
	float weight;
};
