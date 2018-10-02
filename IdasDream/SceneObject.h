#pragma once

#include <glm\glm.hpp>
#include <memory>

#include "Material.h"
#include "Geometry.h"

class SceneObject
{
public:
	SceneObject(glm::mat4 modelMatrix, SceneObject* parent);
	~SceneObject();

	void addData(GeometryData geometryData, std::shared_ptr<Material> material);

	glm::mat4 getModelMatrix();
	glm::mat3 getNormalMatrix();
	GeometryData getGeometryData();
	std::shared_ptr<Material> getMaterial();

	void addChild(SceneObject* child);
	std::vector<SceneObject*>& getChildren();

	bool getHasData();

private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::mat4 _modelMatrix;
	glm::mat3 _normalMatrix;
	SceneObject* _parent;

	bool hasData = false;

	std::shared_ptr<Material> _material;
	GeometryData _geometryData;

	std::vector<SceneObject*> _children;
};
