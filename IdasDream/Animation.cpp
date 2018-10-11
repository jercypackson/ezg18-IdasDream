#include "pch.h"
#include "Animation.h"
#include <algorithm>

Animation::Animation() { }

Animation::Animation(std::vector<float> time, std::vector<Transform> transform)
	: _time(time), _transform(transform) { }

Animation::Animation(std::map<float, Transform>& timeTransformMap)
{
	for (auto const&[key, val] : timeTransformMap)
	{
		_time.push_back(key);
		_transform.push_back(val);
	}
}

std::optional<glm::mat4> Animation::getCurrentMatrix(float currTime)
{
	auto t = getCurrentTransform(currTime);
	if (t) {
		return t->getMatrix();
	}
	return std::nullopt;
}

std::optional<Transform> Animation::getCurrentTransform(float currTime)
{
	if (_time.empty() || currTime < _time.front() || currTime > _time.back()) return std::nullopt;

	auto lessOrEqual = std::prev(std::upper_bound(_time.begin(), _time.end(), currTime), 1);
	auto greaterOrEqual = std::lower_bound(_time.begin(), _time.end(), currTime);

	auto delta = (currTime - (*lessOrEqual)) / ((*greaterOrEqual) - (*lessOrEqual));

	auto transf = Transform::mix(
		_transform[lessOrEqual - _time.begin()],
		_transform[greaterOrEqual - _time.begin()],
		delta
	);

	return transf;
}

Animation::~Animation()
{
}
