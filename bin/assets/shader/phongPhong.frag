#version 460 core

#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_shader_draw_parameters : enable

in VertexData {
	vec3 position_world;
	vec3 normal_world;
	vec2 uv;
    flat int drawID;
    vec3 debugColor;
} vert;

out vec4 color;

//per frame
uniform vec3 camera_world; 
//

// fake shadow
uniform vec3 feetPosition;

//once
uniform vec3 materialCoefficients = vec3(0.2,0.9,0.0); // x = ambient, y = diffuse, z = specular 
uniform float specularAlpha = 2.f;

uniform struct DirectionalLight {
	vec3 color;
	vec3 direction;
} dirL;
//

struct FragData {
    vec4 col;
    float specularCoeff;
    int receivesShadow;
    sampler2D textureBuffer;
};

layout(std430, binding = 4) buffer fragColorBuffer {
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
    //todo: remove
    DirectionalLight dirL = DirectionalLight(vec3(1), vec3(0.5,-1,-0.3));

	vec3 n = normalize(vert.normal_world);
	vec3 v = -normalize(vert.position_world - camera_world);

    vec4 c = data[vert.drawID].col;
    if (c.r < 0 || c.r > 1){ //invalid color
        c = texture(data[vert.drawID].textureBuffer, vert.uv);
    }

    if (c.a < 0.99) discard;
	
	color = vec4(c.rgb * materialCoefficients.x, 1); // ambient
	
	// add directional light contribution
	color.rgb += phong(n, -dirL.direction, v, dirL.color * c.rgb, materialCoefficients.y, dirL.color, data[vert.drawID].specularCoeff, specularAlpha, false, vec3(0));

    // fake shadow
    float fadeOut = (min(distance(feetPosition, vert.position_world), 1.0));
    if (data[vert.drawID].receivesShadow == 0) fadeOut = 1.0;
    fadeOut = smoothstep(0.15, 1.0, fadeOut);
    fadeOut = fadeOut * 0.7 + 0.3;
    color.rgb = color.rgb * fadeOut + (1.0 - fadeOut) * vec3(0.1);
    
    //color.rgb = abs(n);
    //color = vec4(vert.debugColor, 1);
}

