#include "pch.h"
#include "Importer.h"
#include <filesystem>
#include "Extensions.h"
#include "Geometry.h"
#include "ShaderManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>


// todo create hierachy, sort after material/shader

Importer::Importer(std::string path) 
	: _path(path)
{
}

Importer::~Importer()
{
}

SceneObject* Importer::import()
{
	auto so = new SceneObject(glm::mat4(1.f), nullptr);

	//for each file in path
	for (const auto & p : std::filesystem::directory_iterator(_path)) {
		auto filePath = p.path().string();
		importFile(filePath, so);
	}

	return so;
}

void Importer::importFile(std::string file, SceneObject* root)
{
	auto f = FileImporter(file, root);
}

void FileImporter::readNode(const aiNode* node, SceneObject* parent) {

	auto s = new SceneObject(Extensions::toGlmMat4(node->mTransformation), parent);

	//if (node->mNumMeshes == 0) do nothing, empty SceneObject

	if (node->mNumMeshes == 1) {
		auto mesh = _scene->mMeshes[node->mMeshes[0]];

		auto gd = GeometryData();

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

		if (diffMatCount == 0) {
			aiColor4D* pOut = new aiColor4D(1,0,0,1); //default
			aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, pOut);

			mat = std::make_shared<ColorMaterial>(ShaderManager::getShader("phongPhong"), Extensions::toGlmVec4(*pOut), glm::vec3(1), 1.0f);

			delete pOut;
		}
		else if (diffMatCount == 1) {
			aiString str;
			aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);

			auto tex = Extensions::assets + "textures/" + str.C_Str();

			int width, height, nrComponents;
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

			mat = std::make_shared<TextureMaterial>(ShaderManager::getShader("phongPhong"), glm::vec3(), 0.0f, 
				std::make_shared<Texture2DBL>(width, height, f, SamplerInfo(), data));
		}
		else {
			std::cout << "ERROR: Multiple Textures on one object not supported." << std::endl;
		}

		s->addData(gd, mat);
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
	_scene = _importer.ReadFile(file.c_str(), aiProcess_Triangulate);
	readNode(_scene->mRootNode, root);
}

FileImporter::~FileImporter()
{
}