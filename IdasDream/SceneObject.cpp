#include "pch.h"
#include "SceneObject.h"
#include <glm/gtx/matrix_decompose.hpp>

SceneObject::SceneObject(glm::mat4 modelMatrix, SceneObject * parent)
	: _parent(parent), _modelMatrix(modelMatrix)
{
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 v3;
	glm::vec4 v4;

	glm::decompose(modelMatrix, v3, orientation, translation, v3, v4);

	_position = translation;
	_rotation = glm::eulerAngles(orientation);

	if (_parent) _parent->addChild(this);

	_normalMatrix = glm::transpose(glm::inverse(getModelMatrix()));
}

SceneObject::~SceneObject()
{
}

void SceneObject::addData(GeometryData geometryData, std::shared_ptr<Material> material)
{
	if (!hasData) {
		_geometryData = geometryData;
		_material = material;
		hasData = true;
	}
}

glm::mat4 SceneObject::getModelMatrix() {
	if (_parent != nullptr) {
		return _parent->getModelMatrix() * _modelMatrix;
	}
	return _modelMatrix;
}

glm::mat4 SceneObject::getNormalMatrix() {
	return _normalMatrix;
}

GeometryData SceneObject::getGeometryData()
{
	return _geometryData;
}

std::shared_ptr<Material> SceneObject::getMaterial()
{
	return _material;
}

void SceneObject::addChild(SceneObject * child)
{
	_children.push_back(child);
}

std::vector<SceneObject*>& SceneObject::getChildren()
{
	return _children;
}

bool SceneObject::getHasData()
{
	return hasData;
}

