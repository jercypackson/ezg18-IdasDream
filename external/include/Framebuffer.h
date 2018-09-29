#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <assert.h>

#include "Texture2D.h"

enum class FramebufferUsage
{
	READ = GL_READ_FRAMEBUFFER,
	WRITE = GL_DRAW_FRAMEBUFFER,
	READ_WRITE = GL_FRAMEBUFFER
};

class Framebuffer
{
private:
	GLuint _handle;
	unsigned int _width;
	unsigned int _height;
	GLuint _depthRenderBuffer;
	std::vector<Texture2D> _renderTargets;
	std::optional<Texture2D> _depthBuffer;
	std::vector<GLenum> _drawBuffers;

	void init(std::vector<Texture2D>& renderTargets);

public:
	Framebuffer(std::vector<Texture2D> renderTargets, Texture2D depthBuffer);
	Framebuffer(std::vector<Texture2D> renderTargets, bool createDepthBuffer);
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& other);
	Framebuffer& operator=(Framebuffer&& other);
	~Framebuffer();

	void bind(FramebufferUsage usage = FramebufferUsage::READ_WRITE) const;
	Texture2D* getRenderTarget(unsigned int i);

};