#include "pch.h"
#include "ParticleObject.h"


ParticleObject::ParticleObject(int maxCount)
	: BaseGeometry(DrawCallInfo{ DrawCallInfo::GeometryType::POINT_ARRAYS, DrawCallInfo::DrawCallType::SINGLE, { 0 }, { maxCount } })
{
	Buffer positionBuffer = Buffer(nullptr, maxCount * sizeof(glm::vec4), BufferUsage::DYNAMIC);
	addVertexAttrib(std::move(positionBuffer), { 4, VertexAttribFormat::DataType::FLOAT }, to_underlying(VertexAttribDefaults::POSITIONS));
}


ParticleObject::~ParticleObject()
{
}
