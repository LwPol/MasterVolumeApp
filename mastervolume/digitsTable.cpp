#include "stdafx.h"
#include "digitsTable.h"
#include "bmpLoader.h"

#include <sstream>

DigitsTable::DigitsTable() {
	glGenTextures(10, texture);
	for (int i = 0; i < 10; ++i) {
		std::stringstream ss;
		ss << "digits\\" << i << ".bmp";
		std::string textureFile;
		ss >> textureFile;

		unsigned char *pixels = GetBMPPixels(textureFile.c_str(), 32, 32, false);
		if (!pixels)
			throw std::exception();
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete[] pixels;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	vao.bindVertexArray();
	vbo.bindBuffer(GL_ARRAY_BUFFER);
	float x = 8.f / 32.f, y = 1.f - 16.f / 32.f;
	float rect[] = {
		1.f, 0.f,
		x, y,

		0.f, 0.f,
		0.f, y,

		1.f, 1.f,
		x, 1.f,

		0.f, 1.f,
		0.f, 1.f
	};
	VertexBuffer::loadData(GL_ARRAY_BUFFER, sizeof(rect), rect);

	vao.enableAttrib(0);
	vao.enableAttrib(1);
	vao.setVertexAttribPointer(0, 2, GL_FLOAT, 4 * sizeof(float), 0);
	vao.setVertexAttribPointer(1, 2, GL_FLOAT, 4 * sizeof(float), 2 * sizeof(float));
}

DigitsTable::~DigitsTable() {
	glDeleteTextures(10, texture);
}

void DigitsTable::printNumber(float x, float y, float size, int value) {
	spRender.useProgram();
	GLuint samp = glGetUniformLocation(spRender.getId(), "samp");
	glUniform1i(samp, 0);

	std::stringstream ss;
	ss << value;
	std::string num;
	ss >> num;

	vao.bindVertexArray();
	glDisable(GL_DEPTH_TEST);
	glm::mat4 mScale = glm::scale(glm::mat4(1.f), glm::vec3(size, size, 1.f));
	for (char &c : num) {
		glBindTexture(GL_TEXTURE_2D, texture[c - '0']);
		glm::mat4 mTranslate = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));
		spRender.setUniform("mTransform", mTranslate * mScale);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += size;
	}
	glEnable(GL_DEPTH_TEST);
}
