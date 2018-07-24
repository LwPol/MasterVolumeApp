// master_volume_app.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "shaders.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "crank.h"
#include "fpsCounter.h"
#include "mouseControl.h"
#include "indicator.h"
#include "digitsTable.h"
#include "volumeManager.h"

#include "bmpLoader.h"

#include <Windows.h>

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

int main() {
	if (FAILED(CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return -1;

	GLFWwindow *window = nullptr;

	if (!glfwInit())
		return -1;

	std::set_terminate([] {glfwTerminate(); std::abort(); });

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(400, 400, "Master Volume App", nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	VolumeManager vmManager;

	MouseControl::createMouseCtrl();

	MouseControl::setOpenedHandCursor(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPosCallback(window, &MouseControl::mouseMove);
	glfwSetMouseButtonCallback(window, &MouseControl::mouseButtons);
	glfwSwapInterval(1);

	std::vector<Shader> shaders(2);
	shaders[0] = Shader("shaders\\main.vert", GL_VERTEX_SHADER);
	shaders[1] = Shader("shaders\\main.frag", GL_FRAGMENT_SHADER);
	spMain = ShaderProgram(shaders);
	spMain.useProgram();

	Crank crank(0.5f, 0.15f, 20);
	MouseControl::getMouseCtrl()->setCrank(&crank);
	auto rotationsHandler = [](int rotations, VolumeManager &vm) {vm.setMasterVolume(vm.getMasterVolume() + rotations); };
	crank.rotationMade = std::bind(rotationsHandler, std::placeholders::_1, std::ref(vmManager));

	Indicator ind(0.2f, 0.7f, 0.05f, 0.5f, 0.075f, 0.15f);
	ind.setValue(100);

	//glm::mat4 mProj = glm::perspective((float)M_PI / 2, 1.f, 0.2f, 50.f);
	glm::mat4 mProj = glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.5f, 1.5f);
	glm::mat4 mView = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 mModel = glm::mat4(1.f);//glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 1.f, 1.f));
	spMain.setUniform("mMVP", mProj * mView * mModel);

	glClearColor(0.f, 0.f, 0.4f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.f);

	//FPSCounter timer;
	do {
		//timer.update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		spMain.useProgram();
		ind.setValue(vmManager.getMasterVolume());
		ind.render(spMain, mProj * mView);

		mModel = glm::rotate(glm::mat4(1.f), crank.getAngle(), glm::vec3(0.f, 0.f, -1.f));
		spMain.setUniform("mMVP", mProj * mView * mModel);
		crank.render(spMain);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	CoUninitialize();
    return 0;
}
