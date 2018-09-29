#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Buffer.h"
#include "IDManager.h"


struct BoundingBox
{
	glm::vec4 minVertex;
	glm::vec4 maxVertex;

	glm::vec4 center() { return 0.5f * (minVertex + maxVertex); }
	glm::vec4 sphere()
	{
		glm::vec4 c_ = center();
		glm::vec3 c = glm::vec3(c_.x, c_.y, c_.z);
		float r = glm::distance(c, glm::vec3(minVertex.x, minVertex.y, minVertex.z));
		return glm::vec4(c, r);
	}
};

struct GeometryData
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
	BoundingBox boundingBox;
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
	UVS = 2
};

struct DrawCallInfo
{
	enum class GeometryType
	{
		TRIANGLE_ARRAYS = 0,
		TRIANGLE_ELEMENTS = 1,
		TRIANGLE_STRIP = 2,
		TRIANGLE_FAN = 3,
		PATCH_ARRAYS = 4,
		PATCH_ELEMENTS = 5
	} geometryType = GeometryType::TRIANGLE_ARRAYS;

	enum class DrawCallType
	{
		SINGLE = 100,
		INSTANCED = 200,
		MULTI = 300,
		INDIRECT = 400,
		MULTI_INDIRECT = 500,
		MULTI_INDIRECT_COUNT = 600
	} drawCallType = DrawCallType::SINGLE;

	std::vector<int> first = { 0 };
	std::vector<int> count = { 0 };
	unsigned int instances = 0;
	unsigned int patches = 0;
	std::vector<void*> indices = { 0 };
	std::vector<unsigned int> multiDrawRange = {};
	std::vector<unsigned int> baseVertex = { 0 };
	void* indirect = 0;

	static DrawCallInfo fromGeometryData(const std::vector<GeometryData>& geometryData);
};

enum class ElementBufferDataType
{
	UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
	UNSIGNED_INT = GL_UNSIGNED_INT,
	UNSIGNED_SHORT = GL_UNSIGNED_SHORT
};


class Geometry
{
private:
	GLuint _vao;
	unsigned int _elements;
	ElementBufferDataType _ebDataType;
	std::vector<Buffer> _vertexBuffers;
	std::optional<Buffer> _elementBuffer;
	std::unique_ptr<Buffer> _indirectBuffer;
	std::unique_ptr<Buffer> _indirectDrawCountBuffer;
	std::vector<VertexAttribFormat> _vertexBufferFormats;
	std::unordered_map<unsigned int, unsigned int> _bindingLocations; // location -> index in _vertexBuffers

	DrawCallInfo _dcInfo;
	struct DrawCallInfoIntern {
		std::vector<unsigned int> mdRange;
		std::vector<int> mdRangeCount;
	} _dcInfoIntern;
	void (Geometry::*_draw)() const;

	std::vector<BoundingBox> _boundingBoxes;

	void addVertexAttrib_(GLuint bufferHandle, const VertexAttribFormat& format, unsigned int location);

public:
	Geometry(DrawCallInfo drawCallInfo, const std::vector<GeometryData>& geometryData = {});
	Geometry(const Geometry& other) = delete;
	Geometry& operator=(const Geometry& other) = delete;
	Geometry(Geometry&& other);
	Geometry& operator=(Geometry&& other);
	~Geometry();

	DrawCallInfo getDrawCallInfo() const { return _dcInfo; }
	void setDrawCallInfo(DrawCallInfo drawCallInfo);

	BoundingBox getBoundingBox() const { return _boundingBoxes[0]; }
	std::vector<BoundingBox> getBoundingBoxes() const { return _boundingBoxes; }

	void bindVertexArray() const;
	void draw() const;
	void drawTriangleStrip() const;
	void drawTriangleStripInstanced() const;
	void drawTriangleStripIndirect() const;
	void drawTriangleFan() const;
	void drawTriangleFanInstanced() const;
	void drawTriangleFanIndirect() const;
	void drawTriangleArrays() const;
	void drawTriangleArraysInstanced() const;
	void drawTriangleArraysIndirect() const;
	void drawMultiTriangleArraysIndirect() const;
	void drawTriangleElements() const;
	void drawTriangleElementsInstanced() const;
	void drawTriangleElementsIndirect() const;
	void drawMultiTriangleElementsIndirect() const;
	void drawMultiTriangleElementsIndirectCount() const;
	void drawPatchArrays() const;
	void drawPatchElements() const;
	int addVertexAttrib(Buffer&& buffer, const VertexAttribFormat& format);
	int addVertexAttrib(Buffer&& buffer, const VertexAttribFormat& format, unsigned int location);
	void setElementBuffer(Buffer&& buffer, unsigned int elements, ElementBufferDataType type);
	Buffer* const getIndirectBuffer() const;
	Buffer* const getIndirectDrawCountBuffer() const;
	void removeVertexAttrib(unsigned int location);
	int attachVertexAttrib(const Buffer& buffer, const VertexAttribFormat& format);
	int attachVertexAttrib(const Buffer& buffer, const VertexAttribFormat& format, unsigned int location);
	int attachVertexAttrib(const Buffer* buffer, const VertexAttribFormat& format);
	int attachVertexAttrib(const Buffer* buffer, const VertexAttribFormat& format, unsigned int location);

};