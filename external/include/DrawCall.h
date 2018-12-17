#pragma once

#include <vector>

class BaseGeometry;
struct GeometryData;

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

class DrawCall 
{
	friend class BaseGeometry;

private:
	BaseGeometry* _geometry;

	DrawCallInfo _dcInfo;
	struct DrawCallInfoIntern {
		std::vector<unsigned int> mdRange;
		std::vector<int> mdRangeCount;
	} _dcInfoIntern;

	void (DrawCall::*_draw)() const;

public:
	DrawCall(BaseGeometry* base, const DrawCallInfo& dci);
	DrawCall(const DrawCall& other) = delete;
	DrawCall& operator=(const DrawCall& other) = delete;
	DrawCall(DrawCall&& other) = default;
	DrawCall& operator=(DrawCall&& other) = default;
	~DrawCall();

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

};