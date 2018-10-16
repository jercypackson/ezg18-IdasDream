#version 460 core
/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
    flat int drawID;
} vert;

uniform mat4 viewProjMatrix;

struct VertData {
    mat4 modelMatix;
    mat4 normalMatrix;
};

layout(std430, binding = 0) buffer dataBuffer {
	VertData data[];
};

//layout(std430, binding = 1) buffer bonesBuffer {
//	mat4 bones[];
//};
//
//const int NUM_BONES_PER_VEREX = 7;
//
//struct BoneData {
//    uint boneIdx[NUM_BONES_PER_VEREX];
//    float weigth[NUM_BONES_PER_VEREX];
//};
//
//layout(std430, binding = 2) buffer boneDataBuffer {
//	BoneData boneData[];
//};
//
//layout(std430, binding = 3) buffer boneDataStartBuffer {
//	int boneDataStartIdx[];
//};

void main() {

    //int boneDataIdx = boneDataStartIdx[gl_DrawID] + gl_VertexID;
    //BoneData bd = boneData[boneDataIdx];
    //
    //mat4 boneTransform = mat4(0.0f);
    //
    //for (int i = 0; i < NUM_BONES_PER_VEREX; i++){
    //    boneTransform += bones[bd.boneIdx[i]] * bd.weigth[i];
    //}

    //todo: use boneTransform

    VertData d = data[gl_DrawID];
	vert.normal_world = mat3(d.normalMatrix) * normal;
	vec4 position_world_ = d.modelMatix * vec4(position, 1);
	vert.position_world = position_world_.xyz;
    vert.uv = uv;
    vert.drawID = gl_DrawID;
	gl_Position = viewProjMatrix * position_world_;
}
