#version 460 core

in VertexData {
    flat float TTL1;
} vert;

uniform vec3 diffuseColor;

out vec4 color;

vec3 pow3(vec3 v, float p)
{
    return pow(abs(v), vec3(p));
}

vec3 glowSphere(vec3 color, vec2 p, float radius, float glow) 
{
    float term = 1./length(p)/5.;
    return pow3(color * term * 0.5, glow) * radius;
}

void main() {
    //vec3 baseColor = diffuseColor * clamp(TTL1, 0, 0, 1);
    //vec3 color = glowSphere(baseColor, 
	color = vec4(diffuseColor, 1);
}