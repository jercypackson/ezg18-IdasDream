#version 460 core

layout(location = 0) in vec4 position;

//out VertexData {
//    flat float TTL1;
//} vert;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

//const vec3 arr[3] = vec3[3](vec3(0,0,-0.1), vec3(-5,5,-0.1), vec3(0,10,-0.1));

void main() {

	gl_Position = viewMatrix * modelMatrix * vec4(position.xyz, 1);
    //vert.TTL1 = position.w;
    gl_PointSize = 10;
}