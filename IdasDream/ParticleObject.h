#pragma once
#include "BaseGeometry.h"
#include "Buffer.h"

class ParticleObject :
	public BaseGeometry
{
public:
	ParticleObject(unsigned int location, int maxCount);
	DECLARE_CSTR_MOVE(ParticleObject)

	std::shared_ptr<Buffer> getVelocitiesBuffer();

private:
	/// settings
	const BufferUsage bufferUsage = BufferUsage::DYNAMIC;
	const VertexAttribFormat format = {
		4,									//components
		VertexAttribFormat::DataType::FLOAT	//dataType
	};
	///

	std::shared_ptr<Buffer> velocities;
};

