#pragma once

#include <GL\glew.h>
#include <limits>


#define BUFFER_OFFSET(offset) (static_cast<char*>(0) + (offset))


enum class BufferUsage {
	STATIC = 0,
	DYNAMIC = GL_DYNAMIC_STORAGE_BIT/*,
	MAP_READ = GL_MAP_READ_BIT,
	MAP_WRITE = GL_MAP_WRITE_BIT,
	MAP_READ_WRITE = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT,
	MAP_PERSISTENT = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT*/
};

/*enum class BufferMapUsage {
	READ = GL_READ_ONLY,
	WRITE = GL_WRITE_ONLY,
	READ_WRITE = GL_READ_WRITE
};*/

enum class BufferType {
	SSBO = GL_SHADER_STORAGE_BUFFER,
	UBO = GL_UNIFORM_BUFFER,
	ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER
};

class Buffer {
protected:

	friend class Geometry;

	GLuint _handle;
	size_t _size;

public:
	Buffer(const void* data, size_t size, BufferUsage usage);
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;
	Buffer(Buffer&& other);
	Buffer& operator=(Buffer&& other);
	~Buffer();

	void bind(BufferType type, unsigned int block) const;
	void update(const void* data, size_t size = std::numeric_limits<size_t>::max(), size_t offset = 0);

	template <typename T>
	void read(T* data, size_t length = std::numeric_limits<size_t>::max(), size_t offset = 0)
	{
		length = glm::min(length, _size);
		glGetNamedBufferSubData(_handle, offset, length, data);
	}
	
	/*template <typename T>
	T* map(BufferMapUsage usage, size_t length = std::numeric_limits<size_t>::max(), size_t offset = 0)
	{
		length = glm::min(length, _size);
		void* ptr = glMapNamedBufferRange(_handle, offset, length, (GLenum)usage);
		return static_cast<T*>(ptr);
	}

	void unmap();*/
};
