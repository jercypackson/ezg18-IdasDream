#version 460 core
/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_shader_draw_parameters : enable

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
    flat int drawID;
} vert;

out vec4 color;

//per frame
uniform vec3 camera_world; 
//


//once
uniform vec3 materialCoefficients = vec3(0.2,0.9,0.01); // x = ambient, y = diffuse, z = specular 
uniform float specularAlpha = 2;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;
//

struct FragData {
    vec4 col;
    sampler2D textureBuffer;
};

layout(std430, binding = 1) buffer fragColorBuffer {
    FragData data[];
};

vec3 phong(vec3 n, vec3 l, vec3 v, vec3 diffuseC, float diffuseF, vec3 specularC, float specularF, float alpha, bool attenuate, vec3 attenuation) {
	float d = length(l);
	l = normalize(l);
	float att = 1.0;	
	if(attenuate) att = 1.0f / (attenuation.x + d * attenuation.y + d * d * attenuation.z);
	vec3 r = reflect(-l, n);
	return (diffuseF * diffuseC * max(0, dot(n, l)) + specularF * specularC * pow(max(0, dot(r, v)), alpha)) * att; 
}

void main() {	
	vec3 n = normalize(vert.normal_world);
	vec3 v = normalize(camera_world - vert.position_world);

    vec4 c = data[vert.drawID].col;
    if (c.r < 0 || c.r > 1){ //invalid color
        c = texture(data[vert.drawID].textureBuffer, vert.uv);
    }
	
	color = vec4(c.rgb * materialCoefficients.x, 1); // ambient
	
	// add directional light contribution
	color.rgb += phong(n, -dirL.direction, v, dirL.color * c.rgb, materialCoefficients.y, dirL.color, materialCoefficients.z, specularAlpha, false, vec3(0));

    //color.rgb = abs(n);
}

