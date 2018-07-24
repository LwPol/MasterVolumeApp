#include "stdafx.h"
#include "crank.h"

Crank::Crank(float crankLength, float width, int circlePoints) :
	fCrankLength(crankLength),
	fWidth(width),
	circlePoints((circlePoints + 1) / 2 * 2)
{
	if (crankLength < 0.f || width < 0.f || circlePoints < 0)
		throw std::exception();
	if (!vao || !vbo)
		throw std::exception();

	// down half circle
	std::vector<glm::vec3> vertices(this->circlePoints / 2 + 2);
	vertices[0] = glm::vec3(0.f);
	float interval = static_cast<float>(M_PI) / (vertices.size() - 2);
	for (std::size_t i = 0; i < vertices.size() - 1; ++i) {
		float x = static_cast<float>(cos(i * interval));
		float y = static_cast<float>(-sin(i * interval));
		vertices[i + 1] = (width / 2) * glm::vec3(x, y, 0.f);
	}

	// rectangle
	vertices.push_back(glm::vec3(width / 2, 0.f, 0.f));
	vertices.push_back(glm::vec3(-width / 2, 0.f, 0.f));
	vertices.push_back(glm::vec3(width / 2, crankLength, 0.f));
	vertices.push_back(glm::vec3(-width / 2, crankLength, 0.f));

	// upper half circle
	vertices.push_back(glm::vec3(0.f, crankLength, 0.f));
	for (int i = 0; i < this->circlePoints / 2 + 1; ++i) {
		float x = (width / 2) * static_cast<float>(cos(i * interval));
		float y = (width / 2) * static_cast<float>(sin(i * interval)) + crankLength;
		vertices.push_back(glm::vec3(x, y, 0.f));
	}

	// handle circle
	vertices.push_back(glm::vec3(0.f, crankLength, 0.1f));
	float fullCircleInterval = 2 * static_cast<float>(M_PI) / this->circlePoints;
	for (int i = 0; i < this->circlePoints + 1; ++i) {
		float x = 0.75f * (width / 2) * static_cast<float>(cos(i * fullCircleInterval));
		float y = 0.75f * (width / 2) * static_cast<float>(sin(i * fullCircleInterval)) + crankLength;
		vertices.push_back(glm::vec3(x, y, 0.01f));
	}

	// add control square
	vertices.push_back(glm::vec3(-width / 2, -width / 2, 0.f));
	vertices.push_back(glm::vec3(width / 2, -width / 2, 0.f));
	vertices.push_back(glm::vec3(width / 2, width / 2, 0.f));
	vertices.push_back(glm::vec3(-width / 2, width / 2, 0.f));

	vao.bindVertexArray();
	vbo.bindBuffer(GL_ARRAY_BUFFER);

	VertexBuffer::loadData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0]);

	vao.enableAttrib(0);
	vao.setVertexAttribPointer(0, 3, GL_FLOAT, 0, 0);
}

void Crank::render(ShaderProgram &program) {
	program.setUniform("vColor", glm::vec3(0.5f, 0.5f, 0.5f));

	int index = 0;
	vao.bindVertexArray();
	glDrawArrays(GL_TRIANGLE_FAN, index, circlePoints / 2 + 2);
	index += circlePoints / 2 + 2;
	glDrawArrays(GL_TRIANGLE_STRIP, index, 4);
	index += 4;
	glDrawArrays(GL_TRIANGLE_FAN, index, circlePoints / 2 + 2);

	program.setUniform("vColor", glm::vec3(0.8f, 0.8f, 0.8f));
	index += circlePoints / 2 + 2;
	glDrawArrays(GL_TRIANGLE_FAN, index, circlePoints + 2);
}

void Crank::renderHandleOnly(ShaderProgram &program, const glm::vec3 &color) {
	program.setUniform("vColor", color);

	vao.bindVertexArray();
	int startingIndex = circlePoints + 8;
	glDrawArrays(GL_TRIANGLE_FAN, startingIndex, circlePoints + 2);
}

void Crank::renderControlSqaure(ShaderProgram &program, const glm::vec3 &color) {
	program.setUniform("vColor", color);

	vao.bindVertexArray();
	int startingIndex = 2 * circlePoints + 10;
	glDrawArrays(GL_TRIANGLE_STRIP, startingIndex, 4);
}

void Crank::addAngle(float add) {
	fAngle += add;
	int rotations = static_cast<int>(fAngle / (2 * static_cast<float>(M_PI)));
	if (rotations != 0) {
		if (rotationMade)
			rotationMade(rotations);
	}
	
	fAngle -= rotations * 2 * static_cast<float>(M_PI);
}
