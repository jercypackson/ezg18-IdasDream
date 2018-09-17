#include "pch.h"
#include "IdasDream.h"
#include <glm/glm.hpp>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>


IdasDream::IdasDream(int width, int height, bool fullscreen)
	: Application({ width, height, fullscreen, "Idas Dream", 4, 6 })
{
}

IdasDream::~IdasDream()
{
}

void IdasDream::init()
{
	Assimp::Importer importer;
	std::string path = "C:/OneDrive/OneDrive - student.tuwien.ac.at/uni/Masterstudium Visual Computing/18WS/RTR/objects/";
	std::string name = "ida - StartWalking.fbx";
	std::string s = path + name;
	const aiScene* scene = importer.ReadFile(s.c_str(), 0);

}

void IdasDream::render(float dt)
{
}

void IdasDream::destroy()
{
}
