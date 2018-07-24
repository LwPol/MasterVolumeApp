#pragma once

#include "stdafx.h"
#include "shaders.h"
#include "vertexArray.h"
#include "vertexBuffer.h"

class Crank {
	private:
		VertexArray vao;
		VertexBuffer vbo;

		float fCrankLength, fWidth;
		int circlePoints;

		bool bHandleHeld = false;
		float fAngle = 0.f;
	public:
		Crank(float crankLength, float width, int circlePoints);

		void render(ShaderProgram &program);

		void renderHandleOnly(ShaderProgram &program, const glm::vec3 &color);
		void renderControlSqaure(ShaderProgram &program, const glm::vec3 &color);

		bool isHeld() const { return bHandleHeld; }
		void hold() { bHandleHeld = true; }
		void release() { bHandleHeld = false; }

		float getAngle() const { return fAngle; }
		void addAngle(float add);

		std::function<void(int)> rotationMade;
};