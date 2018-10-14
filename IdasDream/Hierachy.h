#pragma once

#include "SceneObject.h"

class Hierachy
{
public:
	static SceneObject* find(SceneObject* s, const std::string & name);
	static void forEach(SceneObject* s, const std::function <void(SceneObject*)>& func);
};

