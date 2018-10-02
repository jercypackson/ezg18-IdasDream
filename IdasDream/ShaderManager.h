#pragma once

#include <iostream>
#include <memory>
#include "Shader.h"
#include <map>
#include <string>

class ShaderManager
{
public:
	static std::map<std::string, std::shared_ptr<Shader>> map;
	static std::map<std::string, std::shared_ptr<Shader>> init_map();

	static std::shared_ptr<Shader> getShader(std::string name);
};

