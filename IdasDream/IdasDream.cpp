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

IdasDream::IdasDream(int width, int height, bool fullscreen)
	: Application({ width, height, fullscreen, "Idas Dream", 4, 6 }),
	_arcballCamera({ 60.0f, width / (float)height, 0.1f, 100.0f })
{
	_arcballCamera.setZoom(50);
}

IdasDream::~IdasDream()
{
}

void ForEach(SceneObject* s, const std::function <void(SceneObject*)>& func)
{
	func(s);

	for (SceneObject* so : s->getChildren())
	{
		ForEach(so, func);
	}
}

struct Data {
	glm::mat4 modelMatix;
	glm::mat4 normalMatrix;
	glm::vec4 diffuseColor;
};

void IdasDream::init()
{
	_arcballCamera.registerToWindow(_window);

	_scene = Importer(Extensions::assets + "objects").import();

	// load and use default shader
	_shader = ShaderManager::getShader("phongPhong");
	_shader->registerToWindow(_window);
	_shader->use();

	std::vector<GeometryData> geometryData;
	std::vector<Data> data;

	ForEach(_scene, [&gd = geometryData, &d = data](SceneObject* s) {
		if (ColorMaterial* mat = dynamic_cast<ColorMaterial*>(s->getMaterial().get())) { //todo sort by different shaders
			if (s->getHasData()) {
				gd.push_back(s->getGeometryData());
				d.push_back({
					s->getModelMatrix(),
					glm::mat4(s->getNormalMatrix()),
					glm::vec4(mat->getColor(),1)
				});
			}
		}
	});

	DrawCallInfo dci = DrawCallInfo::fromGeometryData(geometryData);
	_obj.push_back(Geometry(dci, geometryData));

	_dataBuffer = std::make_unique<Buffer>(&data[0], sizeof(Data) * data.size(), BufferUsage::DYNAMIC);
	_dataBuffer->bind(BufferType::SSBO, 0);

	// Initialize lights
	_dirL = DirectionalLight(glm::vec3(1.0f), glm::vec3(0.5,-1,-0.3));

	_shader->setUniform("dirL.color", _dirL.color);
	_shader->setUniform("dirL.direction", _dirL.direction);

	// some OpenGL defaults
	glClearColor(0.3f,0.5f,0.8f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void IdasDream::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader->setUniform("viewProjMatrix", _arcballCamera.getViewProjectionMatrix());
	_shader->setUniform("camera_world", _arcballCamera.getPosition());

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
