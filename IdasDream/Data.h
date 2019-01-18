#pragma once

#include <glm/glm.hpp>

struct VertData {
	glm::mat4 modelMatix;
	glm::mat4 normalMatrix;
	float twistParam = 0;
	float waveParam = -1;
	glm::vec2 _padding;
};

const int NUM_BONES_PER_VEREX = 7;
struct BoneData {
	float weight[NUM_BONES_PER_VEREX] = {};
};

struct FragData {
	glm::vec4 col;					//16 byte
	float specularCoeff;			//4 byte
	int receivesShadow;				//4 byte
	std::uint64_t textureBuffer;	//8 byte
};

