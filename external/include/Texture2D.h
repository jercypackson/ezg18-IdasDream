#pragma once

#include "TextureInfo.h"

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