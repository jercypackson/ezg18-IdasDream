#version 460 core
/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

in VertexData {
	vec3 position_world;
	vec3 normal_world;
    vec3 color;
} vert;

out vec4 color;

uniform vec3 camera_world;

uniform vec3 materialCoefficients = vec3(0.1,0.9,0.3); // x = ambient, y = diffuse, z = specular 
uniform float specularAlpha = 10;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;

uniform struct PointLight {
	vec3 color;
	vec3 position;
	vec3 attenuation;
} pointL;

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
	
	color = vec4(vert.color * materialCoefficients.x, 1); // ambient
	
	// add directional light contribution
	color.rgb += phong(n, -dirL.direction, v, dirL.color * vert.color, materialCoefficients.y, dirL.color, materialCoefficients.z, specularAlpha, false, vec3(0));
			
	// add point light contribution
	color.rgb += phong(n, pointL.position - vert.position_world, v, pointL.color * vert.color, materialCoefficients.y, pointL.color, materialCoefficients.z, specularAlpha, true, pointL.attenuation);
}

