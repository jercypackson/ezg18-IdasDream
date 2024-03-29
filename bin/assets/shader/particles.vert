#version 460 core

layout(location = 0) in vec4 position;

out VertexData {
    flat float TTL;
} vert;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main() {
	gl_Position = viewMatrix * modelMatrix * vec4(position.xyz, 1);
    vert.TTL = position.w;
    gl_PointSize = 20;
}