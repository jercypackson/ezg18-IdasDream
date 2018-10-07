/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Data.h"

/*!
 * Base material
 */
class Material
{
protected:
	/*!
	 * Base material constructor
	 * @param shader: The shader used for rendering this material
	 * @param materialCoefficients: The material's coefficients (x = ambient, y = diffuse, z = specular)
	 * @param alpha: Alpha value, i.e. the shininess constant
	 */
	Material(std::shared_ptr<Shader> shader, glm::vec3 materialCoefficients, float alpha);

	virtual ~Material();

	/*!
	 * The shader used for rendering this material
	 */
	std::shared_ptr<Shader> _shader;
	/*!
	 * The material's coefficients (x = ambient, y = diffuse, z = specular)
	 */
	glm::vec3 _materialCoefficients; 

	/*!
	 * Alpha value, i.e. the shininess constant
	 */
	float _alpha;

public:
	/*!
	 * @return The shader associated with this material
	 */
	Shader* getShader();

	/*!
	 * Sets this material's parameters as uniforms in the shader
	 */
	virtual void setUniforms();

	virtual void setFragmentData(std::vector<FragData>& data);
};

/*!
 * Texture material
 */
class TextureMaterial : public Material
{
protected:
	/*!
	 * The diffuse texture of this material
	 */
	std::shared_ptr<Texture2DBL> _diffuseTexture;

public:
	/*!
	 * Texture material constructor
	 * @param shader: The shader used for rendering this material
	 * @param materialCoefficients: The material's coefficients (x = ambient, y = diffuse, z = specular)
	 * @param alpha: Alpha value, i.e. the shininess constant
	 * @param diffuseTexture: The diffuse texture of this material
	 */
	TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 materialCoefficients, float alpha, std::shared_ptr<Texture2DBL> diffuseTexture);
	
	virtual ~TextureMaterial();

	/*!
	 * Set's this material's parameters as uniforms in the shader
	 */
	virtual void setUniforms();

	virtual void setFragmentData(std::vector<FragData>& data);
};


/*!
 * Colored material
 */
class ColorMaterial : public Material
{
protected:
	/*!
	 * The material's color
	 */
	glm::vec4 _color;

public:
	/*!
	 * Base material constructor
	 * @param shader: The shader used for rendering this material
	 * @param color: The material's color
	 * @param materialCoefficients: The material's coefficients (x = ambient, y = diffuse, z = specular)
	 * @param alpha: Alpha value, i.e. the shininess constant
	 */
	ColorMaterial(std::shared_ptr<Shader> shader, glm::vec4 color, glm::vec3 materialCoefficients, float alpha);

	virtual ~ColorMaterial();

	/*!
	 * Set's this material's parameters as uniforms in the shader
	 */
	virtual void setUniforms();

	glm::vec4 getColor();

	virtual void setFragmentData(std::vector<FragData>& data);

};