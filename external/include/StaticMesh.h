#pragma once

#include "BaseGeometry.h"


class StaticMesh : public BaseGeometry
{
private:

public:
	StaticMesh(DrawCallInfo drawCallInfo, const std::vector<GeometryData>& geometryData = {});
	DECLARE_CSTR_DEFAULT(StaticMesh)
	//DECLARE_CSTR_MOVE(StaticMesh)

};
