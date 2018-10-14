#include "pch.h"
#include "Hierachy.h"

SceneObject* Hierachy::find(SceneObject* s, const std::string & name)
{
	SceneObject* foundObj = nullptr;

	forEach(s, [&f = foundObj, &n = name](SceneObject* s){
		if (f == nullptr && s->getName().find(n) != std::string::npos){
			f = s;
		}
	});

	return foundObj;
}

void Hierachy::forEach(SceneObject * s, const std::function<void(SceneObject*)>& func)
{
	func(s);

	for (auto so : s->getChildren())
	{
		forEach(so, func);
	}
}
