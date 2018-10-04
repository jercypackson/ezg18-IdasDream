#version 460 core

#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_shader_draw_parameters : enable

//layout(bindless_sampler) uniform sampler2D tex;

layout(std430, binding=0) buffer TextureHandleBuffer
{
    sampler2D textureBuffer[];
};

in VertexData {
	vec3 position_ws;
	vec3 normal_ws;
	vec2 uv;
    flat int drawID;
} vert;

out vec4 color;

void main() {	
	color = texture(textureBuffer[vert.drawID], vert.uv).rgba;
    //color = vec4(vert.uv.r, vert.uv.g, 0, 1);
}