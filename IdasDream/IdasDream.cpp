#include "pch.h"
#include "IdasDream.h"
#include <glm/glm.hpp>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "Geometry.h"
#include "Extensions.h"

#include <limits>

IdasDream::IdasDream(int width, int height, bool fullscreen)
	: Application({ width, height, fullscreen, "Idas Dream", 4, 6 })
{
}

IdasDream::~IdasDream()
{
}

void IdasDream::init()
{
	// load and use default shader
	_shader = std::make_unique<Shader>("assets/shader/simple", ShaderList{ ShaderType::VERTEX, ShaderType::FRAGMENT });
	_shader->registerToWindow(_window);
	_shader->use();

	Assimp::Importer importer;
	std::string path = "C:/OneDrive/OneDrive - student.tuwien.ac.at/uni/Masterstudium Visual Computing/18WS/RTR/objects/";
	std::string name = "Modell_Lvl1_reduced.fbx";
	std::string s = path + name;
	const aiScene* scene = importer.ReadFile(s.c_str(), aiProcess_Triangulate);

	auto corpusMesh = scene->mMeshes[scene->mRootNode->FindNode("Corpus")->mMeshes[0]];

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
	BoundingBox boundingBox {
		glm::vec4(glm::vec3(std::numeric_limits<float>::max()), 1), //minVertex -> set to max
		glm::vec4(glm::vec3(std::numeric_limits<float>::min()), 1)  //maxVertex -> set to min
	};

	for (unsigned int i = 0; i < corpusMesh->mNumVertices; i++)
	{
		positions.push_back(Extensions::toGlmVec3(corpusMesh->mVertices[i]));
		normals  .push_back(Extensions::toGlmVec3(corpusMesh->mNormals [i]));

		//todo uvs

		//bb
		boundingBox.minVertex.x = std::min(boundingBox.minVertex.x, positions.back().x);
		boundingBox.minVertex.y = std::min(boundingBox.minVertex.y, positions.back().y);
		boundingBox.minVertex.z = std::min(boundingBox.minVertex.z, positions.back().z);

		boundingBox.maxVertex.x = std::max(boundingBox.maxVertex.x, positions.back().x);
		boundingBox.maxVertex.y = std::max(boundingBox.maxVertex.y, positions.back().y);
		boundingBox.maxVertex.z = std::max(boundingBox.maxVertex.z, positions.back().z);
	}

	for (unsigned int i = 0; i < corpusMesh->mNumFaces; i++)
	{
		auto face = corpusMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	auto gd = GeometryData
	{
		positions,
		normals,
		uvs,
		indices,
		boundingBox
	};

	// create single-draw geometry
	std::vector<GeometryData> geometryData = { gd };
	DrawCallInfo dci = DrawCallInfo::fromGeometryData(geometryData);
	_obj.push_back(Geometry(dci, geometryData));

	// some OpenGL defaults
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

glm::mat4 m(1.0f);

void IdasDream::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader->setUniform("VP", m);
	_shader->setUniform("M", m);

	_obj[0].bindVertexArray();
	_obj[0].draw();

}

void IdasDream::destroy()
{
}
