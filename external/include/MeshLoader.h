#pragma once

#include <string>
#include <vector>

#include "BaseGeometry.h"

class MeshLoader {

private:
	MeshLoader() { }
	~MeshLoader() { }

public:
	static bool loadOBJ(std::string file, std::vector<GeometryData>& geometryData);
	static GeometryData createSphereGeometry(glm::vec3 center, unsigned int longitudeSegments, unsigned int latitudeSegments, float radius);
	static GeometryData createPlaneGeometry(glm::vec3 center, float width, float height);

};