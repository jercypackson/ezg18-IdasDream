#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VertexData {
	vec3 position_ws;
	vec3 normal_ws;
	vec2 uv;
    flat int drawID;
} vert;

uniform mat4 VP;

void main() {
	vert.position_ws = position;
	vert.normal_ws = normal;
	vert.uv = uv;
    vert.drawID = gl_DrawID;
	gl_Position = VP * vec4(position, 1);
}