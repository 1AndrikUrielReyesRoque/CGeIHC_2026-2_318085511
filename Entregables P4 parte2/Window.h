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
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	// Variables del Gato Robot
	GLfloat getPataFI_1() { return pataFI_1; }
	GLfloat getPataFI_2() { return pataFI_2; }
	GLfloat getPataFD_1() { return pataFD_1; }
	GLfloat getPataFD_2() { return pataFD_2; }
	GLfloat getPataTI_1() { return pataTI_1; }
	GLfloat getPataTI_2() { return pataTI_2; }
	GLfloat getPataTD_1() { return pataTD_1; }
	GLfloat getPataTD_2() { return pataTD_2; }
	GLfloat getCola() { return cola; }
	GLfloat getOrejaI() { return orejaI; }
	GLfloat getOrejaD() { return orejaD; }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;

	// Ángulos de articulaciones
	GLfloat pataFI_1, pataFI_2; // Frontal Izquierda
	GLfloat pataFD_1, pataFD_2; // Frontal Derecha
	GLfloat pataTI_1, pataTI_2; // Trasera Izquierda
	GLfloat pataTD_1, pataTD_2; // Trasera Derecha
	GLfloat cola;               // Cola
	GLfloat orejaI, orejaD;     // Orejas

	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};