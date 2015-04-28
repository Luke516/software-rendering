#version 400

uniform sampler2D myTextureSampler;

smooth in vec2 uv;

out vec4 frag_color;

void main () {
	vec3 color = texture2D( myTextureSampler, uv ).rgb;
	//vec3 color = vec3(0.5,0.5,0);
	frag_color = vec4(color, 1.0);

	
}