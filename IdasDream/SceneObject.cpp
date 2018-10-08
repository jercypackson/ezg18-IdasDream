#include "pch.h"
#include "SceneObject.h"
#include <glm/gtx/matrix_decompose.hpp>

SceneObject::SceneObject(std::string name, glm::mat4 modelMatrix, SceneObject * parent)
	: _name(name), _parent(parent), _localModelMatrix(modelMatrix)
{
	if (_parent) {
		_parent->addChild(this);
		setMatrices(_parent->getModelMatrix());
	}
	else {
		setMatrices(glm::mat4(1.0f));
	}
	calcTransf();
}

SceneObject::~SceneObject()
{
	for (auto c : _children) {
		delete c;
	}
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

std::string SceneObject::getName()
{
	return _name;
}

void SceneObject::setLocalModelMatrix(glm::mat4 mm)
{
	_localModelMatrix = mm;
	setMatrices(_parent->getModelMatrix());
	calcTransf();
}

bool SceneObject::setAnimation(Animation anim)
{
	if (!_animation.has_value()) {
		_animation = anim;
		return true;
	}
	return false;
}

void SceneObject::animate(float time)
{
	if (_animation.has_value()) {
		auto mm = _animation->getCurrentMatrix(time);
		if (mm) {
			setLocalModelMatrix(mm.value());
		}
	}
}

void SceneObject::setMatrices(glm::mat4 parentMM)
{
	_modelMatrix = parentMM * _localModelMatrix;
	_normalMatrix = glm::transpose(glm::inverse(_modelMatrix));
	for (const auto & c : _children) {
		c->setMatrices(_modelMatrix);
	}
}

void SceneObject::calcTransf()
{
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 v3;
	glm::vec4 v4;
	glm::decompose(_localModelMatrix, v3, orientation, translation, v3, v4);
	_transform = Transform(translation, orientation);
}

//glm::mat4 SceneObject::getModelMatrixRecursive()
//{
//	if (_parent != nullptr) {
//		return _parent->getModelMatrix() * _localModelMatrix;
//	}
//	return _localModelMatrix;
//}

