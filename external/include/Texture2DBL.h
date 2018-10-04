#pragma once

#include <vector>

#include "TextureInfo.h"

typedef GLuint64 TextureHandle;
typedef GLuint64 ImageHandle;

class Texture2DBL
{
private:
	friend class Framebuffer;

	GLuint _handle;
	GLuint64 _texHandle;
	std::vector<GLuint64> _imgHandles;
	unsigned int _width;
	unsigned int _height;
	SamplerInfo _info;
	unsigned int _levels;
	TextureFormatData _textureFormat;

public:
	Texture2DBL(unsigned int width, unsigned int height, TextureFormat format, SamplerInfo info, const void* data);
	Texture2DBL(const Texture2DBL&) = delete;
	Texture2DBL& operator=(const Texture2DBL&) = delete;
	Texture2DBL(Texture2DBL&& other);
	Texture2DBL& operator=(Texture2DBL&& other);
	~Texture2DBL();

	void getDimensions(unsigned int& width, unsigned int& height) const { width = _width; height = _height; }
	TextureHandle getTextureHandle() const { return _texHandle; }
	ImageHandle getImageHandle(ImageUsage usage, unsigned int level = 0);
};