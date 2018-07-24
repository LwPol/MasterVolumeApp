#pragma once

#include "stdafx.h"
#include "shaders.h"
#include "vertexArray.h"
#include "vertexBuffer.h"

class DigitsTable {
	private:
		GLuint texture[10];
		VertexArray vao;
		VertexBuffer vbo;

		ShaderProgram spRender;
	public:
		DigitsTable();

		~DigitsTable();

		template <class ...Args>
		void linkFontShader(Args&& ...args) {
			spRender = ShaderProgram(args...);
		}

		void printNumber(float x, float y, float size, int value);

		GLuint getTex(int index) const { return texture[index]; }
		ShaderProgram& getProgram() { return spRender; }
};