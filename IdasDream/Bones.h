#pragma once

#include <string>
#include <glm\glm.hpp>

struct Bone {
	std::string name;
	glm::mat4 mat;
};

class Bones
{
public:
	Bones();
	~Bones();

private:

};

