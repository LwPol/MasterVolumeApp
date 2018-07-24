#version 330

out vec4 outputColor;

in vec2 vTex;

uniform sampler2D samp;

void main() {
	outputColor = texture(samp, vTex);
}