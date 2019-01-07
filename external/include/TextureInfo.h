#pragma once

#include <GL/glew.h>

enum class TextureFormat
{
	R8,
	RGBA8,
	SRGBA8,
	R32UI,
	RGBA32UI,
	R32F,
	RGBA32F,
	DEPTH
};

enum class ImageUsage
{
	READ_ONLY = GL_READ_ONLY,
	WRITE_ONLY = GL_WRITE_ONLY,
	READ_WRITE = GL_READ_WRITE
};

struct TextureFormatData
{
	GLenum format;
	GLenum internalFormat;
	GLenum dataType;
};

struct SamplerInfo
{
	enum class Filtering
	{
		NEAREST_NEIGHBOR, BILINEAR, TRILINEAR
	} filtering = Filtering::BILINEAR;

	enum class Wrapping
	{
		REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER
	} wrapping = Wrapping::CLAMP_TO_EDGE;
};