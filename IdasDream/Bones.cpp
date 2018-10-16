#include "pch.h"
#include "Bones.h"


std::vector<std::string> Bones::names = init_names();


unsigned int Bones::bone(std::string name)
{
	auto it = std::find(names.begin(), names.end(), name);

	//if not found
	if (it == names.end()) {
		names.push_back(name);
		it = names.end();
	}

	return (unsigned int)(it - names.begin()); //return index
}

int Bones::getBone(std::string name)
{
	auto it = std::find(names.begin(), names.end(), name);

	//if not found
	if (it == names.end()) {
		return -1;
	}

	return (unsigned int)(it - names.begin()); //return index
}

size_t Bones::size()
{
	return names.size();
}

std::vector<std::string> Bones::init_names()
{
	return std::vector<std::string>();
}
