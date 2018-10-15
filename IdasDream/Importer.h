#pragma once

#include <string>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "SceneObject.h"
#include "ArmatureObject.h"

class Importer
{
public:
	Importer(std::string path);
	~Importer();

	SceneObject* import();
	void importFile(std::string file, SceneObject* root);

private:
	std::string _path;
};

class FileImporter
{
public:
	FileImporter(std::string file, SceneObject* root);
	~FileImporter();
private:
	const aiScene* _scene;
	void readNode(const aiNode* node, SceneObject* parent);
	Assimp::Importer _importer;

	ArmatureObject* _armature = nullptr;
};