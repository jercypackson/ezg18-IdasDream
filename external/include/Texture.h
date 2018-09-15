#pragma once

#include <GL/glew.h>

enum class TextureFormat
{
	R8, 
	R16F, 
	R32F, 
	RGB8, 
	RGBA8, 
	RGB16, 
	RGB16F, 
	RGB32F, 
	RGBA32F
};

struct SamplerInfo 
{
	enum class Filtering 
	{
		NEAREST_NEIGHBOR, BILINEAR, TRILINEAR, ANISOTROPIC
	} filtering;

	enum class Wrapping 
	{
		REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER
	} wrapping = Wrapping::REPEAT;
};

class Texture 
{
private:
	GLuint _handle;

public:
	Texture(unsigned int width, unsigned int height, TextureFormat format, SamplerInfo info, const void* data);
	~Texture();

};