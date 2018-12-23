#version 460 core

in VertexData {
    flat float TTL;
} vert;

uniform vec3 diffuseColor;

out vec4 color;

vec3 pow3(vec3 v, float p) {
    return pow(abs(v), vec3(p));
}

vec3 glowSphere(vec3 color, vec2 p, float radius, float glow) 
{
    float term = 1.0f/length(p)/5.0f;
    return pow3(color * term * 0.5, glow) * radius;
}

void main() {
    vec3 baseColor = diffuseColor * clamp(vert.TTL, 0.0f, 1.0f);

    vec3 fcolor = glowSphere(baseColor, gl_PointCoord * 2 - 1, 3, 2);
    float lum = dot(fcolor, vec3(0.299, 0.587, 0.114));

	color = vec4(fcolor, lum);
}