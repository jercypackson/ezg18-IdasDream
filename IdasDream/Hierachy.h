#pragma once

#include "SceneObject.h"

class Hierachy
{
public:
	Hierachy(SceneObject * root);
	~Hierachy();

	void forEach(const std::function <void(SceneObject*)>& func);
	SceneObject* find(const std::string& name);

private:
	SceneObject* _root;

	void forEach(SceneObject* s, const std::function <void(SceneObject*)>& func);

};

