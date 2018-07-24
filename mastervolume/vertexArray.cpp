#include "stdafx.h"
#include "vertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vaoId);
}

VertexArray::VertexArray(VertexArray &&other) :
	vaoId(other.vaoId)
{
	other.vaoId = 0;
}

VertexArray& VertexArray::operator=(VertexArray &&other) {
	glDeleteVertexArrays(1, &vaoId);

	vaoId = other.vaoId;
	other.vaoId = 0;
	return *this;
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoId);
}

void VertexArray::bindVertexArray() {
	glBindVertexArray(vaoId);
}

void VertexArray::unbindVertexArray() {
	glBindVertexArray(0);
}

void VertexArray::enableAttrib(int attrib) {
	bindVertexArray();
	glEnableVertexAttribArray(attrib);
}

void VertexArray::disableAttrib(int attrib) {
	bindVertexArray();
	glDisableVertexAttribArray(attrib);
}

void VertexArray::setVertexAttribPointer(int attrib, int size, GLenum type, GLsizei stride, GLsizei offset, bool normalized) {
	bindVertexArray();
	glVertexAttribPointer(attrib, size, type, normalized, stride, reinterpret_cast<const void*>(offset));
}
