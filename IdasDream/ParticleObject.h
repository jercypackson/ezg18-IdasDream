#pragma once
#include "BaseGeometry.h"
class ParticleObject :
	public BaseGeometry
{
public:
	ParticleObject(int maxCount);
	DECLARE_CSTR_DEFAULT(ParticleObject)
};

