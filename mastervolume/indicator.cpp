#include "stdafx.h"
#include "indicator.h"

Indicator::Indicator(float width, float height, float indicatorWidth, float indicatorHeight, float x, float y) :
	width(width), height(height),
	indicatorWidth(indicatorWidth), indicatorHeight(indicatorHeight),
	xIndicator(x), yIndicator(y)
{
	std::vector<glm::vec3> vertices;

	// background rectangle
	vertices.push_back(glm::vec3(width, 0.f, 0.0f));
	vertices.push_back(glm::vec3(0.f, 0.f, 0.0f));
	vertices.push_back(glm::vec3(width, height, 0.0f));
	vertices.push_back(glm::vec3(0.f, height, 0.0f));

	// indicator background
	vertices.push_back(glm::vec3(xIndicator + indicatorWidth, yIndicator, 0.1f));
	vertices.push_back(glm::vec3(xIndicator, yIndicator, 0.1f));
	vertices.push_back(glm::vec3(xIndicator + indicatorWidth, yIndicator + indicatorHeight, 0.1f));
	vertices.push_back(glm::vec3(xIndicator, yIndicator + indicatorHeight, 0.1f));

	// indicator square
	vertices.push_back(glm::vec3(xIndicator + indicatorWidth, yIndicator, 0.4f));
	vertices.push_back(glm::vec3(xIndicator, yIndicator, 0.4f));
	vertices.push_back(glm::vec3(xIndicator + indicatorWidth, yIndicator + indicatorWidth, 0.4f));
	vertices.push_back(glm::vec3(xIndicator, yIndicator + indicatorWidth, 0.4f));

	vao.bindVertexArray();
	vbo.bindBuffer(GL_ARRAY_BUFFER);
	VertexBuffer::loadData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0]);
	
	vao.enableAttrib(0);
	vao.setVertexAttribPointer(0, 3, GL_FLOAT, 0, 0);

	// dtTable init
	std::vector<Shader> shaders(2);
	shaders[0] = Shader("shaders\\font.vert", GL_VERTEX_SHADER);
	shaders[1] = Shader("shaders\\font.frag", GL_FRAGMENT_SHADER);
	dtTable.linkFontShader(shaders);
}

void Indicator::render(ShaderProgram &program, const glm::mat4 &mVP) {
	vao.bindVertexArray();

	glm::mat4 mTopTranslate = glm::translate(mVP, glm::vec3(0.65f, -height / 2, 0.f));
	program.setUniform("mMVP", mTopTranslate);

	program.setUniform("vColor", glm::vec3(0.f));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	program.setUniform("vColor", glm::vec3(0.5f, 0.5f, 0.5f));
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	glm::mat4 mGreenInd = glm::translate(glm::mat4(1.f), glm::vec3(0.f, yIndicator, 0.f));
	mGreenInd = glm::scale(mGreenInd, glm::vec3(1.f, static_cast<float>(value) / maxValue, 1.f));
	mGreenInd = glm::translate(mGreenInd, glm::vec3(0.f, -yIndicator, 0.05f));
	program.setUniform("mMVP", mTopTranslate * mGreenInd);
	program.setUniform("vColor", glm::vec3(0.f, 0.8f, 0.f));
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	program.setUniform("mMVP", glm::translate(mTopTranslate,
		glm::vec3(0.f, static_cast<float>(value) / maxValue * (indicatorHeight - indicatorWidth), 0.f)));
	program.setUniform("vColor", glm::vec3(1.f, 1.f, 1.f));
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

	// render string with value
	constexpr float fontSize = 0.06f;
	int digits;
	if (value < 10)
		digits = 1;
	else if (value < 100)
		digits = 2;
	else
		digits = 3;
	float xPos = (width - digits * fontSize) / 2;

	dtTable.printNumber(0.65f + xPos, -height / 2 + 0.04f, fontSize, value);
	spMain.useProgram();
}
