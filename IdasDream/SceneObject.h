#pragma once

#include <glm\glm.hpp>
#include <memory>

#include "Material.h"
#include "StaticMesh.h"
#include "Transform.h"

#include <optional>

#include "Animation.h"

class SceneObject
{
public:
	SceneObject(std::string name, glm::mat4 modelMatrix, SceneObject* parent);
	~SceneObject();

	void addData(GeometryData geometryData, std::shared_ptr<Material> material);
	void addBoneData(std::vector<BoneData> boneData);

	glm::mat4 getModelMatrix();
	glm::mat4 getNormalMatrix();
	GeometryData getGeometryData();
	std::shared_ptr<Material> getMaterial();

	void addChild(SceneObject* child);
	std::vector<SceneObject*>& getChildren();

	bool getHasData();

	std::string getName();

	void setLocalModelMatrix(glm::mat4 mm);

	bool setAnimation(Animation anim);

	virtual void animate(float time);

	virtual SceneObject* createChild(std::string name, glm::mat4 modelMatrix, SceneObject* parent);
	virtual void setBones(std::vector<glm::mat4>& bd);

	void setBoneData(std::vector<BoneData>& bd, std::vector<int>& boneDataStartBuffer);

	virtual bool setGlobalInverse(glm::mat4 gi);

	virtual void addAnimationSequence(std::vector<float> time, std::vector<std::string> name);

protected:
	std::string _name;
	glm::mat4 _localModelMatrix;
	SceneObject* _parent;

private:
	Transform _transform; //local
	glm::mat4 _modelMatrix; //global
	glm::mat4 _normalMatrix; //global

	bool hasData = false;
	bool _hasBoneData = false;

	std::shared_ptr<Material> _material;
	GeometryData _geometryData;
	std::vector<BoneData> _boneData;

	std::vector<SceneObject*> _children;

	void setMatrices(glm::mat4 parentMM);

	void calcTransf();

	std::optional<Animation> _animation;

	std::optional<glm::mat4> _globalInverse; //if there is a amrature this is needed

	//glm::mat4 getModelMatrixRecursive();
};
