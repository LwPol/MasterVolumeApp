#include "stdafx.h"
#include "mouseControl.h"
#include "bmpLoader.h"

std::unique_ptr<MouseControl> MouseControl::mouseCtrlObj;

MouseControl::MouseControl() :
	openedHand(nullptr, &glfwDestroyCursor),
	closedHand(nullptr, &glfwDestroyCursor)
{
	unsigned char *pixels = GetBMPPixels("hand.bmp", 32, 32);
	if (!pixels)
		return;
	GLFWimage image;
	image.width = 32;
	image.height = 32;
	image.pixels = pixels;

	openedHand = decltype(openedHand)(glfwCreateCursor(&image, 16, 16), &glfwDestroyCursor);
	delete[] pixels;

	pixels = GetBMPPixels("closedHand.bmp", 32, 32);
	if (!pixels)
		return;
	image.pixels = pixels;

	closedHand = decltype(closedHand)(glfwCreateCursor(&image, 16, 16), &glfwDestroyCursor);
	delete[] pixels;
}

void MouseControl::createMouseCtrl() {
	mouseCtrlObj = std::unique_ptr<MouseControl>(new MouseControl);
}

void MouseControl::mouseButtonsHandler(GLFWwindow *window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			glfwSetCursor(window, closedHand.get());
			if (getObjectUnderCursor(window) == CursorOverObject::CRANK_HANDLE) {
				glfwGetCursorPos(window, &lastCursorPosX, &lastCursorPosY);
				int windowHeight;
				glfwGetWindowSize(window, nullptr, &windowHeight);
				lastCursorPosY = windowHeight - lastCursorPosY;
				crank->hold();
			}
		}
		else if (action == GLFW_RELEASE) {
			glfwSetCursor(window, openedHand.get());
			crank->release();
		}
	}
}

void MouseControl::mouseButtons(GLFWwindow *window, int button, int action, int mods) {
	if (mouseCtrlObj)
		mouseCtrlObj->mouseButtonsHandler(window, button, action, mods);
}

void MouseControl::mouseMoveHandler(GLFWwindow *window, double xPos, double yPos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (crank->isHeld()) {
			if (getObjectUnderCursor(window) == CursorOverObject::CRANK_CONTROL_SQUARE) {
				crank->release();
				return;
			}

			int wndWidth, wndHeight;
			glfwGetWindowSize(window, &wndWidth, &wndHeight);

			yPos = wndHeight - yPos;
			double xCenter = static_cast<double>(wndWidth) / 2;
			double yCenter = static_cast<double>(wndHeight) / 2;
			if (static_cast<int>(xCenter) == static_cast<int>(xPos) && static_cast<int>(yCenter) == static_cast<int>(yPos))
				return;
			glm::vec2 centerToPrev(static_cast<float>(lastCursorPosX - xCenter), static_cast<float>(lastCursorPosY - yCenter));
			glm::vec2 centerToCurrent(static_cast<float>(xPos - xCenter), static_cast<float>(yPos - yCenter));
			float cos = glm::dot(glm::normalize(centerToPrev), glm::normalize(centerToCurrent));
			if (cos >= -1.f && cos <= 1.f) {
				float angle = acos(cos);

				if (glm::cross(glm::vec3(centerToPrev, 0.f), glm::vec3(centerToCurrent, 0.f)).z > 0.f)
					angle *= -1.f;
				crank->addAngle(angle);
			}

			lastCursorPosX = xPos;
			lastCursorPosY = yPos;
		}
	}
}

void MouseControl::mouseMove(GLFWwindow *window, double xPos, double yPos) {
	if (mouseCtrlObj)
		mouseCtrlObj->mouseMoveHandler(window, xPos, yPos);
}

void MouseControl::setOpenedHandCursor(GLFWwindow *window) {
	if (mouseCtrlObj)
		glfwSetCursor(window, mouseCtrlObj->openedHand.get());
}

MouseControl::CursorOverObject MouseControl::getObjectUnderCursor(GLFWwindow *window) {
	CursorOverObject ret = CursorOverObject::NONE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	crank->renderHandleOnly(spMain, glm::vec3(0.f, 0.f, 0.f));

	unsigned char color[3];
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	int wndHeight;
	glfwGetWindowSize(window, nullptr, &wndHeight);
	glReadPixels(static_cast<int>(cursorX), wndHeight - static_cast<int>(cursorY), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
	if ((color[0] | color[1] | color[2]) == 0)
		ret = CursorOverObject::CRANK_HANDLE;
	else {
		crank->renderControlSqaure(spMain, glm::vec3(0.f));
		glReadPixels(static_cast<int>(cursorX), wndHeight - static_cast<int>(cursorY), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
		if ((color[0] | color[1] | color[2]) == 0)
			ret = CursorOverObject::CRANK_CONTROL_SQUARE;
	}

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT);
	return ret;
}
