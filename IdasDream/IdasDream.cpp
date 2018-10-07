#include "pch.h"
#include "IdasDream.h"
#include <glm/glm.hpp>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "Geometry.h"
#include "Extensions.h"

#include "ShaderManager.h"

#include "Importer.h"

#include <limits>

#include "Data.h"

IdasDream::IdasDream(int width, int height, bool fullscreen)
	: Application({ width, height, fullscreen, "Idas Dream", 4, 6 }),
	_arcballCamera({ 60.0f, width / (float)height, 0.1f, 100.0f })
{
	_arcballCamera.setZoom(50);
}

IdasDream::~IdasDream()
{
	delete _scene;
}

//iterates the scene in a depth-first manner
void ForEach(SceneObject* s, const std::function <void(SceneObject*)>& func)
{
	func(s);

	for (SceneObject* so : s->getChildren())
	{
		ForEach(so, func);
	}
}

void IdasDream::init()
{
	_arcballCamera.registerToWindow(_window);

	_scene = Importer(Extensions::assets + "objects").import();

	// load and use default shader
	_shader = ShaderManager::getShader("phongPhong");
	_shader->registerToWindow(_window);
	_shader->use();

	std::vector<GeometryData> geometryData;
	std::vector<VertData> vertData;
	std::vector<FragData> fragData;

	ForEach(_scene, [&gd = geometryData, &vd = vertData, &fd = fragData](SceneObject* s) {
		if (s->getHasData()) {
			gd.push_back(s->getGeometryData());

			vd.push_back({
				s->getModelMatrix(),
				s->getNormalMatrix()
			});

			s->getMaterial()->setFragmentData(fd);
		}
	});

	DrawCallInfo dci = DrawCallInfo::fromGeometryData(geometryData);
	_obj.push_back(Geometry(dci, geometryData));

	_vertDataBuffer = std::make_unique<Buffer>(&vertData[0], sizeof(VertData) * vertData.size(), BufferUsage::DYNAMIC);
	_vertDataBuffer->bind(BufferType::SSBO, 0);

	_fragDataBuffer = std::make_unique<Buffer>(&fragData[0], sizeof(FragData) * fragData.size(), BufferUsage::STATIC);
	_fragDataBuffer->bind(BufferType::SSBO, 1);


	// Initialize lights
	DirectionalLight dirL = DirectionalLight(glm::vec3(1.0f), glm::vec3(0.5,-1,-0.3));

	_shader->setUniform("dirL.color", dirL.color);
	_shader->setUniform("dirL.direction", dirL.direction);


	// some OpenGL defaults
	glClearColor(0.33f,0.4f,0.5f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void IdasDream::update(float dt)
{
	_arcballCamera.update(_window, dt);

	//todo: animations
}

void IdasDream::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader->setUniform("viewProjMatrix", _arcballCamera.getViewProjectionMatrix());
	_shader->setUniform("camera_world", _arcballCamera.getPosition());

	_obj[0].bindVertexArray();
	_obj[0].draw();
}	

