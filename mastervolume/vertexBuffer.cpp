#include "stdafx.h"
#include "vertexBuffer.h"

VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &bufferId);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) :
	bufferId(other.bufferId)
{
	other.bufferId = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer &&other) {
	glDeleteBuffers(1, &bufferId);

	bufferId = other.bufferId;
	other.bufferId = 0;
	return *this;
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &bufferId);
}

void VertexBuffer::bindBuffer(GLenum target) {
	glBindBuffer(target, bufferId);
}

void VertexBuffer::unbindBuffer(GLenum target) {
	glBindBuffer(target, 0);
}

void VertexBuffer::loadData(GLenum target, GLsizei size, const void *data, GLenum usage) {
	glBufferData(target, size, data, usage);
}
