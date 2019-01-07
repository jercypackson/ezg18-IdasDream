#include "pch.h"
#include "Importer.h"
#include <filesystem>
#include "Extensions.h"
#include "StaticMesh.h"
#include "ShaderManager.h"
#include "TextureInfo.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

#include "Hierachy.h"
#include "ArmatureObject.h"
#include "Bones.h"

Importer::Importer(std::string path)
	: _path(path)
{
}

Importer::~Importer()
{
}

SceneObject* Importer::import()
{
	auto so = new SceneObject("root", glm::mat4(1.f), nullptr);

	//for each file in path
	for (const auto & p : std::filesystem::directory_iterator(_path)) {
		auto filePath = p.path().string();
		std::string ending = "_";
		if (0 == filePath.compare(filePath.length() - ending.length(), ending.length(), ending)) continue;
		importFile(filePath, so);
	}

	return so;
}

void Importer::importFile(std::string file, SceneObject* root)
{
	auto f = FileImporter(file, root);
}

void FileImporter::readNode(const aiNode* node, SceneObject* parent) {

	SceneObject* s;

	if (strcmp(node->mName.C_Str(), "Armature") == 0) {
		_armatureInverse = glm::inverse(Extensions::toGlmMat4(node->mTransformation));

		ArmatureObject* a = new ArmatureObject(node->mName.C_Str(), glm::mat4(1.0f), parent);
		_armature = a;

		s = a;
	}
	else {
		s = parent->createChild(node->mName.C_Str(), Extensions::toGlmMat4(node->mTransformation), parent);

		if (_armature != nullptr) {
			s->setGlobalInverse(_armatureInverse);
		}
	}


	//if (node->mNumMeshes == 0) do nothing, empty SceneObject

	if (node->mNumMeshes == 1) {
		auto mesh = _scene->mMeshes[node->mMeshes[0]];

		auto gd = GeometryData();

		gd.positions.reserve(mesh->mNumVertices);
		gd.normals.reserve(mesh->mNumVertices);
		gd.uvs.reserve(mesh->mNumVertices);
		gd.indices.reserve(mesh->mNumVertices);

		gd.boundingBox.minVertex = glm::vec4(glm::vec3(std::numeric_limits<float>::max()), 1);
		gd.boundingBox.maxVertex = glm::vec4(glm::vec3(std::numeric_limits<float>::min()), 1);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			auto position = Extensions::toGlmVec3(mesh->mVertices[i]);
			gd.positions.push_back(position);
			gd.normals.push_back(Extensions::toGlmVec3(mesh->mNormals[i]));

			gd.uvs.push_back(mesh->mTextureCoords[0] ?
				glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) :
				glm::vec2(0.0f, 0.0f));

			//bb
			if (position.x < gd.boundingBox.minVertex.x) gd.boundingBox.minVertex.x = position.x;
			if (position.y < gd.boundingBox.minVertex.y) gd.boundingBox.minVertex.y = position.y;
			if (position.z < gd.boundingBox.minVertex.z) gd.boundingBox.minVertex.z = position.z;

			if (position.x > gd.boundingBox.maxVertex.x) gd.boundingBox.maxVertex.x = position.x;
			if (position.y > gd.boundingBox.maxVertex.y) gd.boundingBox.maxVertex.y = position.y;
			if (position.z > gd.boundingBox.maxVertex.z) gd.boundingBox.maxVertex.z = position.z;
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			auto face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				gd.indices.push_back(face.mIndices[j]);
			}
		}

		std::shared_ptr<Material> mat;

		aiMaterial *aiMat = _scene->mMaterials[mesh->mMaterialIndex];
		auto diffMatCount = aiMat->GetTextureCount(aiTextureType_DIFFUSE);

		aiColor4D* spec = new aiColor4D(0);
		aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_SPECULAR, spec);
		glm::vec3 matCoeffs = glm::vec3(0.2, 0.9, spec->r * 2);
		delete spec;

		if (diffMatCount == 0) {
			aiColor4D* pOut = new aiColor4D(1, 0, 0, 1); //default
			aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, pOut);

			mat = std::make_shared<ColorMaterial>(ShaderManager::getShader("phongPhong"), Extensions::toGlmVec4(*pOut), matCoeffs, 1.0f);

			delete pOut;
		}
		else if (diffMatCount == 1) {
			aiString str;
			aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);

			auto tex = Extensions::assets + "textures/" + str.C_Str();

			int width = 0, height = 0, nrComponents = 0;
			unsigned char *data = stbi_load(tex.c_str(), &width, &height, &nrComponents, 4);

			TextureFormat f;

			if (nrComponents == 1) {
				f = TextureFormat::R8;
			}
			else if (nrComponents == 4) {
				f = TextureFormat::RGBA8;
			}
			else {
				std::cout << "ERROR: This texture is not supported." << std::endl;
			}

			mat = std::make_shared<TextureMaterial>(ShaderManager::getShader("phongPhong"), matCoeffs, 0.0f,
				std::make_shared<Texture2DBL>(width, height, f, SamplerInfo({ SamplerInfo::Filtering::BILINEAR }), data));
		}
		else {
			std::cout << "ERROR: Multiple Textures on one object not supported." << std::endl;
		}

		aiString* matName = new aiString();
		aiGetMaterialString(aiMat, AI_MATKEY_NAME, matName);

		if (strstr(matName->C_Str(), "NOSHADOW")) {
			mat->setReceivesShadow(false);
		}

		delete matName;

		s->addData(gd, mat);

		if (mesh->HasBones()) {

			std::vector<BoneData> boneData(_scene->mMeshes[node->mMeshes[0]]->mNumVertices);

			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				auto b = mesh->mBones[i];
				unsigned int boneIdx = Bones::bone(b->mName.C_Str());
				auto arm = dynamic_cast<ArmatureObject*>(Hierachy::find(_armature, b->mName.C_Str()));
				arm->setBoneIdx(boneIdx);
				arm->setOffsetMatrix(Extensions::toGlmMat4(b->mOffsetMatrix));

				for (unsigned int j = 0; j < b->mNumWeights; j++) {
					auto w = b->mWeights[j];
					boneData[w.mVertexId].weight[boneIdx] = w.mWeight;
				}
			}

			s->addBoneData(boneData);
		}


	}
	else if (node->mNumMeshes > 1) {
		std::cout << "Error: More than one mesh in node!" << std::endl;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		readNode(node->mChildren[i], s);
	}
}

FileImporter::FileImporter(std::string file, SceneObject* root)
{
	auto from = std::max(file.find_last_of("\\"), file.find_last_of('/')) + 1;
	auto to = file.find_last_of('.');

	_scene = _importer.ReadFile(file.c_str(), aiProcess_Triangulate);

	if (_scene == nullptr) {
		std::cout << "Scene " << file << " was null." << std::endl;
		return;
	}

	_scene->mRootNode->mName = aiString(file.substr(from, to - from));

	readNode(_scene->mRootNode, root);


	//read animations
	for (unsigned int a = 0; a < _scene->mNumAnimations; a++)
	{
		auto anim = _scene->mAnimations[a];
		auto secondsPerTick = 1 / anim->mTicksPerSecond; //todo: compare to IdasDream::_ticksPerSecond

		//iterate positionkeys, rot, scale
		for (unsigned int c = 0; c < anim->mNumChannels; c++)
		{
			auto channel = anim->mChannels[c];

			unsigned int numKeys = channel->mNumPositionKeys;

			if (channel->mNumRotationKeys != numKeys) {
				std::cout << "Error: Invalid animaion keys." << std::endl;
			}

			std::vector<float> time;
			std::vector<Transform> transform;

			for (unsigned int p = 0; p < channel->mNumPositionKeys; p++)
			{
				auto pos = channel->mPositionKeys[p];
				auto rot = channel->mRotationKeys[p];

				if (pos.mTime != rot.mTime) {
					std::cout << "Error: Position and rotation timings do not match." << std::endl;
				}

				time.push_back(static_cast<float>(pos.mTime * secondsPerTick));

				transform.push_back(Transform(Extensions::toGlmVec3(pos.mValue), Extensions::toGlmQuat(rot.mValue)));
			}

			//just checking
			for (unsigned int s = 0; s < channel->mNumScalingKeys; s++)
			{
				if (Extensions::round(Extensions::toGlmVec3(channel->mScalingKeys[s].mValue), 3)
					!= glm::vec3(1)) {
					std::cout << "Error: Scaling in animation not supported" << std::endl;
				}
			}

			auto arm = dynamic_cast<ArmatureObject*>(Hierachy::find(_armature, channel->mNodeName.C_Str()));
			arm->addAnimation(anim->mName.C_Str(), Animation(time, transform));
		}
	}

}

FileImporter::~FileImporter()
{
}
