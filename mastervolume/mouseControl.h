#pragma once

#include "stdafx.h"
#include "crank.h"
#include "shaders.h"

class MouseControl {
	public:
		enum class CursorOverObject {
			NONE,
			CRANK_HANDLE,
			CRANK_CONTROL_SQUARE
		};
	private:
		static std::unique_ptr<MouseControl> mouseCtrlObj;

		std::unique_ptr<GLFWcursor, decltype(&glfwDestroyCursor)> openedHand, closedHand;

		Crank *crank;

		double lastCursorPosX, lastCursorPosY;

		MouseControl();

		CursorOverObject getObjectUnderCursor(GLFWwindow *window);
	public:
		static void createMouseCtrl();

		void mouseButtonsHandler(GLFWwindow *window, int button, int action, int mods);
		static void mouseButtons(GLFWwindow *window, int button, int action, int mods);

		void mouseMoveHandler(GLFWwindow *window, double xPos, double yPos);
		static void mouseMove(GLFWwindow *window, double xPos, double yPos);

		static void setOpenedHandCursor(GLFWwindow *window);

		void setCrank(Crank *crank) { this->crank = crank; }

		static MouseControl* getMouseCtrl() { return mouseCtrlObj.get(); }
};