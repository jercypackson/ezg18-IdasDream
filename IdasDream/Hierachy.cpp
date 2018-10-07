#include "pch.h"
#include "Hierachy.h"

Hierachy::Hierachy(SceneObject * root)
	: _root (root) {}

Hierachy::~Hierachy()
{
	delete _root;
}

void Hierachy::forEach(const std::function<void(SceneObject*)>& func)
{
	forEach(_root, func);
}

SceneObject* Hierachy::find(const std::string & name)
{
	SceneObject* foundObj = nullptr;

	forEach(_root, [&f = foundObj, &n = name](SceneObject* s){
		if (f == nullptr && s->getName().find(n) != std::string::npos){
			f = s;
		}
	});

	return foundObj;
}

void Hierachy::forEach(SceneObject * s, const std::function<void(SceneObject*)>& func)
{
	func(s);

	for (SceneObject* so : s->getChildren())
	{
		forEach(so, func);
	}
}
