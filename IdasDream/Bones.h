#pragma once

#include <string>
#include <glm\glm.hpp>
#include <vector>

class Bones
{
public:
	static unsigned int bone(std::string name);
	static int getBone(std::string name);
	static size_t size();
private:
	static std::vector<std::string> names;
	static std::vector<std::string> init_names();
};
