#version 330

layout (location = 0) in vec3 vPos;

uniform mat4 mMVP;

void main() {
	gl_Position = mMVP * vec4(vPos, 1.0);
}