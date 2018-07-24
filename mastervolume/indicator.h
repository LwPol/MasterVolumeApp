#pragma once

#include "stdafx.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "shaders.h"
#include "digitsTable.h"

class Indicator {
	private:
		VertexArray vao;
		VertexBuffer vbo;

		DigitsTable dtTable;

		float width, height;
		float indicatorWidth, indicatorHeight;
		float xIndicator, yIndicator;

		int value = 0;
		static constexpr int maxValue = 100;
	public:
		Indicator(float width, float height, float indicatorWidth, float indicatorHeight, float x, float y);

		void render(ShaderProgram &program, const glm::mat4 &mVP);

		void getSize(float *width, float *height) {
			if (width)
				*width = this->width;
			if (height)
				*height = this->height;
		}
		void setSize(float width, float height) { this->width = width; this->height = height; }

		void getIndicatorSize(float *width, float *height) {
			if (width)
				*width = indicatorWidth;
			if (height)
				*height = indicatorHeight;
		}
		void setIndicatorSize(float width, float height) { indicatorWidth = width; indicatorHeight = height; }

		void getIndicatorPos(float *x, float *y) {
			if (x)
				*x = xIndicator;
			if (y)
				*y = yIndicator;
		}
		void setIndicatorPos(float x, float y) { xIndicator = x; yIndicator = y; }

		int getValue() const { return value; }
		void setValue(int val) {
			if (val < 0)
				val = 0;
			else if (val > maxValue)
				val = maxValue;
			value = val;
		}
};