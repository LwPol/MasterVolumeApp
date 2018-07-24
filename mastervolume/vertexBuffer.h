#pragma once

#include "stdafx.h"

class VertexBuffer {
	private:
		GLuint bufferId;
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer &&other);

		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer &&other);

		~VertexBuffer();

		void bindBuffer(GLenum target);
		static void unbindBuffer(GLenum target);

		static void loadData(GLenum target, GLsizei size, const void *data, GLenum usage = GL_STATIC_DRAW);

		operator bool() const { return bufferId != 0; }

		GLuint getId() const { return bufferId; }
};