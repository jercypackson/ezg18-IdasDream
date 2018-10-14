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

glm::vec3 Extensions::toGlmVec3(const aiColor4D & aiCol4)
{
	return glm::vec3(aiCol4.r, aiCol4.g, aiCol4.b);
}

glm::vec4 Extensions::toGlmVec4(const aiColor4D & aiCol4)
{
	return glm::vec4(aiCol4.r, aiCol4.g, aiCol4.b, aiCol4.a);
}

glm::mat4 Extensions::toGlmMat4(const aiMatrix4x4 & aiMat4)
{
	glm::mat4 glmMat4(1.f);

	glmMat4[0].x = aiMat4.a1;
	glmMat4[0].y = aiMat4.b1;
	glmMat4[0].z = aiMat4.c1;
	glmMat4[0].w = aiMat4.d1;

	glmMat4[1].x = aiMat4.a2;
	glmMat4[1].y = aiMat4.b2;
	glmMat4[1].z = aiMat4.c2;
	glmMat4[1].w = aiMat4.d2;

	glmMat4[2].x = aiMat4.a3;
	glmMat4[2].y = aiMat4.b3;
	glmMat4[2].z = aiMat4.c3;
	glmMat4[2].w = aiMat4.d3;

	glmMat4[3].x = aiMat4.a4;
	glmMat4[3].y = aiMat4.b4;
	glmMat4[3].z = aiMat4.c4;
	glmMat4[3].w = aiMat4.d4;

	return glmMat4;
}

float Extensions::round(float f, int precision)
{
	unsigned int multiplier = 10 ^ (precision);

	return roundf(f * multiplier) / multiplier;
}
