#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Util.h"
#include "Buffer.h"
#include "DrawCall.h"
#include "BoundingVolume.h"


// This deletes copy constructor and assignment operator and sets move operators to default (base class), also creates virtual destructor
#define DECLARE_CSTR_DEFAULT(class_name) \
	class_name(const class_name& other) = delete; \
	class_name& operator=(const class_name& other) = delete; \
	class_name(class_name&& other) : BaseGeometry(std::move(other)) { } \
	using BaseGeometry::operator=; \
	virtual ~class_name();

// This deletes copy constructor and assignment operator and declares move operators, also creates virtual destructor
#define DECLARE_CSTR_MOVE(class_name) \
	class_name(const class_name& other) = delete; \
	class_name& operator=(const class_name& other) = delete; \
	class_name(class_name&& other); \
	class_name& operator=(class_name&& other); \
	virtual ~class_name();


struct GeometryData
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<unsigned int> indices;
	BoundingBox boundingBox;
	BoundingSphere boundingSphere;
};

struct VertexAttribFormat
{
	unsigned int components;
	enum class DataType {
		FLOAT = GL_FLOAT,
		INT = GL_INT,
		UINT = GL_UNSIGNED_INT
	} dataType;
	unsigned int relativeOffset = 0;
	unsigned int attribDivisor = 0;

	int getByteSize() const
	{
		int sizeDataType = 0;
		switch (dataType) {
		case VertexAttribFormat::DataType::FLOAT:
			sizeDataType = sizeof(float);
			break;
		case VertexAttribFormat::DataType::INT:
		case VertexAttribFormat::DataType::UINT:
			sizeDataType = sizeof(int);
			break;
		}

		return components * sizeDataType;
	}
};

enum class VertexAttribDefaults : unsigned int
{
	POSITIONS = 0,
	NORMALS = 1,
	UVS = 2,
	COLORS = 3,
	TANGENTS = 4,
	BITANGENTS = 5
};

enum class ElementBufferDataType
{
	UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
	UNSIGNED_INT = GL_UNSIGNED_INT,
	UNSIGNED_SHORT = GL_UNSIGNED_SHORT
};


class BaseGeometry 
{
	friend class DrawCall;

private:
	GLuint _vao;
	unsigned int _elements;
	ElementBufferDataType _ebDataType;
	std::vector<Buffer> _vertexBuffers;
	std::optional<Buffer> _elementBuffer;
	std::optional<Buffer> _indirectBuffer;
	std::optional<Buffer> _indirectDrawCountBuffer;

	std::vector<VertexAttribFormat> _vertexBufferFormats;
	std::unordered_map<unsigned int, unsigned int> _bindingLocations; // location -> index in _vertexBuffers

	std::unique_ptr<DrawCall> _drawCall;

	void addVertexAttrib_(GLuint bufferHandle, const VertexAttribFormat& format, unsigned int location);

public:
	BaseGeometry(DrawCallInfo drawCallInfo, const std::vector<GeometryData>& geometryData = {});
	BaseGeometry(const BaseGeometry& other) = delete;
	BaseGeometry& operator=(const BaseGeometry& other) = delete;
	BaseGeometry(BaseGeometry&& other);
	BaseGeometry& operator=(BaseGeometry&& other);
	virtual ~BaseGeometry();

	void bindVertexArray() const;
	void draw() const; 

	int addVertexAttrib(Buffer&& buffer, const VertexAttribFormat& format);
	int addVertexAttrib(Buffer&& buffer, const VertexAttribFormat& format, unsigned int location);
	int attachVertexAttrib(const Buffer& buffer, const VertexAttribFormat& format);
	int attachVertexAttrib(const Buffer& buffer, const VertexAttribFormat& format, unsigned int location);
	int attachVertexAttrib(const Buffer* buffer, const VertexAttribFormat& format);
	int attachVertexAttrib(const Buffer* buffer, const VertexAttribFormat& format, unsigned int location);
	void removeVertexAttrib(unsigned int location);

	void setIndirectBuffer(Buffer&& buffer);
	const Buffer& getIndirectBuffer() const;
	void setIndirectDrawCountBuffer(Buffer&& buffer);
	const Buffer& getIndirectDrawCountBuffer() const;
	const Buffer& getVertexBuffer(unsigned int i) const;
	const Buffer& getElementBuffer() const;
	void setElementBuffer(Buffer&& buffer, unsigned int elements, ElementBufferDataType type);

};