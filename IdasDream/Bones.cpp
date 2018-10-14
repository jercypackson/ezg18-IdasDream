#include "pch.h"
#include "Bones.h"


Bones::Bones()
{
	map = std::map<std::string, glm::mat4>();
}


Bones::~Bones()
{
}

bool Bones::addBone(std::string name, glm::mat4 offset)
{
	if (map.find(name) != map.end()) {
		return false;
	}

	map[name] = offset;
	return true;
}

glm::mat4 Bones::getBone(std::string name)
{
	return map[name];
}

