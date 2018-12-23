#include "pch.h"
#include "ParticleObject.h"

ParticleObject::ParticleObject(unsigned int location, int maxCount)
	: BaseGeometry(DrawCallInfo{ 
		DrawCallInfo::GeometryType::POINT_ARRAYS,
		DrawCallInfo::DrawCallType::SINGLE, 
		{ 0 }, 
		{ maxCount } }
	  )
{
	int bufferSize = maxCount * format.getByteSize();

	addVertexAttrib(
		std::move(
			Buffer(nullptr, bufferSize, bufferUsage)
		), 
		format,
		location
	);

	velocities = std::make_shared<Buffer>(nullptr, bufferSize, bufferUsage);
}

std::shared_ptr<Buffer> ParticleObject::getVelocitiesBuffer() {
	return velocities;
}

ParticleObject::~ParticleObject() { }

// Use this if custom move behavior is desired -> DECLARE_CSTR_MOVE
ParticleObject::ParticleObject(ParticleObject&& other)
	: BaseGeometry(std::move(other))
{
	velocities = other.velocities;
}

ParticleObject& ParticleObject::operator=(ParticleObject&& other)
{
	BaseGeometry::operator=(std::move(other));
	return *this;
}

//todo: find some way to integrate this into the engine?
void ParticleObject::draw(unsigned int count) const
{
	glDrawArrays(GL_POINTS, 0, count);
}
