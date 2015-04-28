#version 400

layout(location = 0) in vec3 vertex_position;

uniform mat4 MVP;
uniform float light;

smooth out vec3 color;

void main () {

		color = vec3(light/400,0,1.0);
		gl_Position = vec4 (vertex_position, 1.0);
}