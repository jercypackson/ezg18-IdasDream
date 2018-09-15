#pragma once

#include <GL\glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Buffer.h"


enum class ShaderType {
	VERTEX = GL_VERTEX_SHADER, 
	FRAGMENT = GL_FRAGMENT_SHADER, 
	GEOMETRY = GL_GEOMETRY_SHADER,
	TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER, 
	TESSELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER, 
	COMPUTE = GL_COMPUTE_SHADER
};

class Shader
{
private:
	GLuint _handle;

	std::unordered_map<ShaderType, std::string> _sources;
	std::unordered_set<ShaderType> _types;

	std::unordered_map<std::string, GLint> _locations;
	std::unordered_map<std::string, GLint> _blockIndices;

	GLuint loadShaders();
	void loadShader_(std::string file, GLenum shaderType, GLuint& handle);
	void reload();

	GLint getUniformLocation(std::string uniform);
	GLint getUniformBlockIndex(std::string uniform);

public:

	Shader(std::string name, std::initializer_list<ShaderType> types);
	~Shader();

	void use() const;
	void unuse() const;

	void registerToWindow(Window& window);

	void setUniform(std::string uniform, const int i);
	void setUniform(GLint location, const int i);
	void setUniform(std::string uniform, const unsigned int i);
	void setUniform(GLint location, const unsigned int i);
	void setUniform(std::string uniform, const float f);
	void setUniform(GLint location, const float f);
	void setUniform(std::string uniform, const glm::mat4& mat);
	void setUniform(GLint location, const glm::mat4& mat);
	void setUniform(std::string uniform, const glm::mat3& mat);
	void setUniform(GLint location, const glm::mat3& mat);
	void setUniform(std::string uniform, const glm::vec2& vec);
	void setUniform(GLint location, const glm::vec2& vec);
	void setUniform(std::string uniform, const glm::vec3& vec);
	void setUniform(GLint location, const glm::vec3& vec);
	void setUniform(std::string uniform, const glm::vec4& vec);
	void setUniform(GLint location, const glm::vec4& vec);
	void setUniformArr(std::string arr, unsigned int i, std::string prop, const glm::vec3& vec);
	void setUniformArr(std::string arr, unsigned int i, std::string prop, const float f);
	void bindUniformBuffer(std::string uniform, const Buffer& ubo, unsigned int block);
	void bindUniformBuffer(std::string uniform, const std::unique_ptr<Buffer>& ubo, unsigned int block);
};
