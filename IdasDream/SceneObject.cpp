#include "pch.h"
#include "SceneObject.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <algorithm>

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

void SceneObject::addBoneData(std::vector<BoneData> boneData)
{
	_boneData = boneData;
	_hasBoneData = true;
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

	if (_globalInverse) {
		_localModelMatrix = _globalInverse.value() * _localModelMatrix;
	}

	setMatrices(_parent->getModelMatrix());
	calcTransf();
}

void SceneObject::setAnimation(Animation anim)
{
	_animation = anim;
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

SceneObject * SceneObject::createChild(std::string name, glm::mat4 modelMatrix, SceneObject * parent)
{
	return new SceneObject(name, modelMatrix, parent);
}

void SceneObject::setBones(std::vector<glm::mat4>& bd)
{
	//do nothing
}

void SceneObject::setBoneData(std::vector<BoneData>& bd, std::vector<int>& boneDataStartBuffer)
{
	if (!_hasBoneData) {
		boneDataStartBuffer.push_back(-1);
		return;
	}

	boneDataStartBuffer.push_back(static_cast<int>(bd.size()));
	bd.insert(bd.end(), _boneData.begin(), _boneData.end());
}

bool SceneObject::setGlobalInverse(glm::mat4 gi)
{
	if (_globalInverse == std::nullopt) {
		_globalInverse = gi;
		setLocalModelMatrix(_localModelMatrix); //so that the gi is added
		return true;
	}
	return false;
}

void SceneObject::addAnimationSequence(std::vector<float> time, std::vector<std::string> name)
{
	//do nothing
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
#if _DEBUG
	glm::vec3 Scale, Translation, Skew;
	glm::quat Orientation;
	glm::vec4 Perspective;

	glm::decompose(_modelMatrix, Scale, Orientation, Translation, Skew, Perspective);

	//if (Scale != glm::vec3(1.0f) || Skew != glm::vec3(0.0f) || Perspective != glm::vec4(glm::vec3(0.0f),1.0f)) {
	//	std::cout << "Attentione" << std::endl;
	//}

	_transform = Transform(Translation, Orientation);
#endif
}

//glm::mat4 SceneObject::getModelMatrixRecursive()
//{
//	if (_parent != nullptr) {
//		return _parent->getModelMatrix() * _localModelMatrix;
//	}
//	return _localModelMatrix;
//}

