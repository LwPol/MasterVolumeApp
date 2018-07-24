#pragma once

#include "stdafx.h"

class VertexArray {
	private:
		GLuint vaoId;
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray &&other);

		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray &&other);

		~VertexArray();

		void bindVertexArray();
		static void unbindVertexArray();

		void enableAttrib(int attrib);
		void disableAttrib(int attrib);
		void setVertexAttribPointer(int attrib, int size, GLenum type, GLsizei stride, GLsizei offset, bool normalized = false);

		operator bool() const { return vaoId != 0; }

		GLuint getId() const { return vaoId; }
};