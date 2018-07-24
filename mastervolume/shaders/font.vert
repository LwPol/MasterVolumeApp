#version 330

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoords;

out vec2 vTex;

uniform mat4 mTransform;

void main() {
	gl_Position = mTransform * vec4(vPos, 0.0, 1.0);
	vTex = vTexCoords;
}