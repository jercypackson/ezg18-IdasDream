#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 offsetPosition;

out VertexData {
	vec3 position_ws;
	vec3 normal_ws;
	vec2 uv;
	vec3 diffuseColor;
} vert;

//layout(std430, binding = 0) buffer diffuseColorBuffer {
//	vec4 diffuseColors[];
//};

uniform vec4 color;

uniform mat4 M;
uniform mat4 VP;

void main() {
	vert.position_ws = vec3(M * vec4(position, 1));
	vert.normal_ws = vec3(M * vec4(normal, 0));
	vert.uv = uv;
	vert.diffuseColor = color.rgb;
	gl_Position = VP * ((M * vec4(position, 1)) + vec4(offsetPosition, 0));
}