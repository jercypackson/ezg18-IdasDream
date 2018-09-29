#pragma once

#include <GL/glew.h>

enum class TextureFormat
{
	R8,
	RGBA8,
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
	} wrapping = Wrapping::REPEAT;
};

class Texture2D
{
private:
	friend class Framebuffer;

	GLuint _handle;
	unsigned int _width;
	unsigned int _height;
	SamplerInfo _info;
	unsigned int _levels;
	TextureFormatData _textureFormat;

public:
	Texture2D(unsigned int width, unsigned int height, TextureFormat format, SamplerInfo info, const void* data);
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;
	Texture2D(Texture2D&& other);
	Texture2D& operator=(Texture2D&& other);
	~Texture2D();

	SamplerInfo getSamplerInfo() const { return _info; }
	void getDimensions(unsigned int& width, unsigned int& height) { width = _width; height = _height; }

	void bindTexture(unsigned int unit);
	void bindImage(unsigned int unit, ImageUsage usage, unsigned int level = 0);
	void setSamplerInfo(SamplerInfo info);

};