#include "pch.h"
/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#include "Material.h"

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */

Material::Material(std::shared_ptr<Shader> shader, glm::vec3 materialCoefficients, float alpha)
	: _shader(shader), _materialCoefficients(materialCoefficients), _alpha(alpha)
{
}

Material::~Material()
{
}

Shader* Material::getShader()
{
	return _shader.get();
}

void Material::setUniforms()
{
	_shader->setUniform("materialCoefficients", _materialCoefficients);
	_shader->setUniform("specularAlpha", _alpha);
}

/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 materialCoefficients, float alpha, std::shared_ptr<Texture2DBL> diffuseTexture)
	: Material(shader, materialCoefficients, alpha), _diffuseTexture(diffuseTexture)
{
}

TextureMaterial::~TextureMaterial()
{
}

void TextureMaterial::setUniforms()
{
	//Material::setUniforms();

	//_diffuseTexture->bind(0);
	//_shader->setUniform("diffuseTexture", 0);
}

/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

ColorMaterial::ColorMaterial(std::shared_ptr<Shader> shader, glm::vec4 color, glm::vec3 materialCoefficients, float alpha)
	: Material(shader, materialCoefficients, alpha),  _color(color)
{
}

ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::setUniforms()
{
	Material::setUniforms();

	_shader->setUniform("diffuseColor", _color);
}

glm::vec4 ColorMaterial::getColor()
{
	return _color;
}
