#version 330

out vec4 color;

uniform vec3 vColor;

void main() {
	color = vec4(vColor, 1.0);
}