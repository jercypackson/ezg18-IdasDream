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
    vec3 color;
} vert;

uniform mat4 viewProjMatrix;

struct Data {
    mat4 modelMatix;
    mat4 normalMatrix;
    vec4 diffuseColor;
};

layout(std430, binding = 0) buffer dataBuffer {
	Data data[];
};

void main() {
    Data d = data[gl_DrawID];
	vert.normal_world = mat3(d.normalMatrix) * normal;//
	vec4 position_world_ = d.modelMatix * vec4(position, 1);//
	vert.position_world = position_world_.xyz;
    vert.color = vec3(d.diffuseColor);
	gl_Position = viewProjMatrix * position_world_;
}