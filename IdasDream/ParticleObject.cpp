#include "pch.h"
#include "ParticleObject.h"


ParticleObject::ParticleObject(int maxCount)
	: BaseGeometry(DrawCallInfo{ DrawCallInfo::GeometryType::POINT_ARRAYS, DrawCallInfo::DrawCallType::SINGLE, { 0 }, { 3 } })
{
	const int TTL = 10;
	std::vector<glm::vec4> positions;

	positions.push_back(glm::vec4( 0.0f,  0.0f, -0.1f, TTL));
	positions.push_back(glm::vec4(-5.0f,  5.0f, -0.1f, TTL));
	positions.push_back(glm::vec4( 0.0f, 10.0f, -0.1f, TTL));

	Buffer positionBuffer = Buffer(&positions[0], positions.size() * sizeof(glm::vec4), BufferUsage::DYNAMIC);
	addVertexAttrib(std::move(positionBuffer), { 4, VertexAttribFormat::DataType::FLOAT }, to_underlying(VertexAttribDefaults::POSITIONS));
}

ParticleObject::~ParticleObject()
{
}
