#pragma once

#include <string>
#include <vector>
#include "Geometry.h"

class MeshLoader {
private:
	MeshLoader() { }
	~MeshLoader() { }

	static GeometryData createSphereGeometry(glm::vec3 center, unsigned int longitudeSegments, unsigned int latitudeSegments, float radius);

public:
	static bool loadOBJ(std::string file, std::vector<Geometry>& geometry, bool packMultiDraw = false);
	static Geometry createSphere(glm::vec3 center, unsigned int longitudeSegments, unsigned int latitudeSegments, float radius);
	static Geometry createPlane();
	static Geometry createMultiIndirectTest();
};