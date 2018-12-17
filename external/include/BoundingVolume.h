#pragma once

#include <glm/glm.hpp>

struct BoundingBox
{
	glm::vec3 minVertex;
	glm::vec3 maxVertex;

	glm::vec3 center() 
	{ 
		return 0.5f * (minVertex + maxVertex); 
	}

	bool inside(const glm::vec3& p) 
	{		
		return glm::all(glm::greaterThanEqual(p, minVertex)) && glm::all(glm::lessThanEqual(p, maxVertex));
	}

	glm::vec4 sphere()
	{
		glm::vec3 c = center();
		return glm::vec4(c, glm::distance(c, minVertex));
	}
};

struct BoundingSphere
{
	glm::vec3 center;
	float radius;

	bool inside(const glm::vec3& p)
	{
		return glm::distance(p, center) <= radius;
	}
};