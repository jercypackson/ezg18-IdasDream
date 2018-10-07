#pragma once

#include <glm\glm.hpp>
#include <memory>

#include "Material.h"
#include "Geometry.h"
#include "Transformation.h"

class SceneObject
{
public:
	SceneObject(std::string name, glm::mat4 modelMatrix, SceneObject* parent);
	~SceneObject();

	void addData(GeometryData geometryData, std::shared_ptr<Material> material);

	glm::mat4 getModelMatrix();
	glm::mat4 getNormalMatrix();
	GeometryData getGeometryData();
	std::shared_ptr<Material> getMaterial();

	void addChild(SceneObject* child);
	std::vector<SceneObject*>& getChildren();

	bool getHasData();

	std::string getName();

	void setLocalModelMatrix(glm::mat4 mm);

private:
	std::string _name;

	Transformation _transformation; //local
	glm::mat4 _localModelMatrix;
	glm::mat4 _modelMatrix; //global
	glm::mat4 _normalMatrix; //global
	SceneObject* _parent;

	bool hasData = false;

	std::shared_ptr<Material> _material;
	GeometryData _geometryData;

	std::vector<SceneObject*> _children;

	void setMatrices(glm::mat4 parentMM);

	void calcTransf();
	
	//glm::mat4 getModelMatrixRecursive();
};
