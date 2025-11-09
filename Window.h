#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getLampMode() { return isOn; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	bool prendido() { return prenderspot; }
	bool AntorchOn() { return antorch; }
	bool RayGunOn() { return raygun; }

	GLfloat forward() { return front; }
	~Window();
public:
	// Para obtener la posición del mouse
	double lastMouseX = 0.0, lastMouseY = 0.0;
	// Para callback de mouse
	static void ManejaMouseClick(GLFWwindow* window, int button, int action, int mods);
private: 
	GLFWwindow *mainWindow;
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion7, articulacion8, articulacion9, articulacion10;
	GLint width, height;
	bool isOn,antorch,raygun;

	bool front;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool prenderspot;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

