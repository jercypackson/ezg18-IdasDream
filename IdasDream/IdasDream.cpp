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
	: Application({ width, height, fullscreen, "Idas Dream", 4, 6 }),
	_arcballCamera({ 60.0f, (float)width / height, 0.1f, 10000.0f })
{
}

IdasDream::~IdasDream()
{
}

void IdasDream::init()
{
	_arcballCamera.registerToWindow(_window);

	// load and use default shader
	_shader = std::make_unique<Shader>(Extensions::assets + "shader/simple", ShaderList{ ShaderType::VERTEX, ShaderType::FRAGMENT });
	_shader->registerToWindow(_window);
	_shader->use();
	_shader->setUniform("color", glm::vec4(0, 0, 1, 1));

	Assimp::Importer importer;
	std::string path = "C:/OneDrive/OneDrive - student.tuwien.ac.at/uni/Masterstudium Visual Computing/18WS/RTR/objects/";
	std::string name = "Modell_Lvl1_reduced.fbx";
	std::string s = path + name;
	const aiScene* scene = importer.ReadFile(s.c_str(), aiProcess_Triangulate);

	auto nodeMesh = scene->mRootNode->FindNode("Corpus");
	auto corpusMesh = scene->mMeshes[nodeMesh->mMeshes[0]];

	auto mm = nodeMesh->mTransformation.Transpose();

	_mm = glm::mat4();
	_mm[0].x = mm.a1;
	_mm[0].y = mm.a2;
	_mm[0].z = mm.a3;
	_mm[0].w = mm.a4;
	_mm[1].x = mm.b1;
	_mm[1].y = mm.b2;
	_mm[1].z = mm.b3;
	_mm[1].w = mm.b4;
	_mm[2].x = mm.c1;
	_mm[2].y = mm.c2;
	_mm[2].z = mm.c3;
	_mm[2].w = mm.c4;
	_mm[3].x = mm.d1;
	_mm[3].y = mm.d2;
	_mm[3].z = mm.d3;
	_mm[3].w = mm.d4;

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

void IdasDream::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader->setUniform("VP", _arcballCamera.getViewProjectionMatrix());
	_shader->setUniform("M", _mm);

	_obj[0].bindVertexArray();
	_obj[0].draw();
}

void IdasDream::update(float dt)
{
	_arcballCamera.update(_window, dt);
}

void IdasDream::destroy()
{
}
