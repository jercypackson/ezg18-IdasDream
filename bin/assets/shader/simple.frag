#version 460 core

in VertexData {
	vec3 position_ws;
	vec3 normal_ws;
	vec2 uv;
	vec3 diffuseColor;
} vert;

out vec4 color;

void main() {	
	/*float th = 0.002;
	float spacing = 0.02;
	if(mod(vert.uv.x, spacing) < th || mod(vert.uv.y, spacing) < th) 
		color = vec4(0.0, 1.0, 0.0, 1.0);
	else 
		color = vec4(diffuseColor * max(dot(vert.normal_ws, vec3(0, 1, 0)), 0), 1.0);*/

	//color = vec4(texture(tex, vert.uv).rgb, 1);
	color = vec4(vert.diffuseColor, 1);
}