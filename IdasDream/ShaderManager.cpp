#include "pch.h"
#include "ShaderManager.h"
#include "Extensions.h"

std::map<std::string, std::shared_ptr<Shader>> ShaderManager::init_map()
{
	return std::map<std::string, std::shared_ptr<Shader>>();
}

std::map<std::string, std::shared_ptr<Shader>> ShaderManager::map = init_map();

std::shared_ptr<Shader> ShaderManager::getShader(std::string name)
{
	auto shader = map[name];
	if (shader == nullptr) {
		shader = std::make_unique<Shader>(Extensions::assets + "shader/" + name, ShaderList{ ShaderType::VERTEX, ShaderType::FRAGMENT });
		map[name] = shader;
	}

	return shader;
}
