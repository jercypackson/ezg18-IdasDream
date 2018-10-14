#pragma once

#include <string>
#include <glm\glm.hpp>
#include <map>

struct Bone {
	std::string name;
	glm::mat4 offset;
};

class Bones
{
public:
	Bones();
	~Bones();

	bool addBone(std::string name, glm::mat4 offset);
	glm::mat4 getBone(std::string name);

private:
	std::map<std::string, glm::mat4> map;
};

