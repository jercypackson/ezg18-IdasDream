#include "pch.h"
#include "Animation.h"
#include <algorithm>

Animation::Animation()
{
}

Animation::Animation(const std::vector<float> time, const std::vector<Transform> transform)
	: _time(time), _transform(transform)
{
}

Animation::Animation(std::map<float, Transform>& timeTransformMap)
{
	for (auto const&[key, val] : timeTransformMap)
	{
		_time.push_back(key);
		_transform.push_back(val);
	}
}

std::optional<glm::mat4> Animation::getCurrentMatrix(const float currTime)
{
	auto t = getCurrentTransform(currTime);
	if (t)
	{
		return t->getMatrix();
	}
	return std::nullopt;
}

float prev = 0;

std::optional<Transform> Animation::getCurrentTransform(const float currTime, const bool fadeInOut)
{
	if (_time.empty()) return std::nullopt;

	if (currTime < _time.front())
	{
		return _transform.front();
	}

	if (currTime > _time.back())
	{
		return _transform.back();
	}

	const auto lessOrEqual = std::prev(std::upper_bound(_time.begin(), _time.end(), currTime), 1);
	const auto greaterOrEqual = std::lower_bound(_time.begin(), _time.end(), currTime);

	float delta = (currTime - (*lessOrEqual)) / ((*greaterOrEqual) - (*lessOrEqual));

	if (fadeInOut)
	{
		delta = -glm::cos(glm::pi<float>() * (delta)) / 2.0f + 0.5f;
	}

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
