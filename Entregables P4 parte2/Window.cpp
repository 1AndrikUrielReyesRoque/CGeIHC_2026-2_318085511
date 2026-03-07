#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++) { keys[i] = 0; }
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	pataFI_1 = pataFI_2 = 0.0f;
	pataFD_1 = pataFD_2 = 0.0f;
	pataTI_1 = pataTI_2 = 0.0f;
	pataTD_1 = pataTD_2 = 0.0f;
	cola = 0.0f;
	orejaI = orejaD = 0.0f;

	for (size_t i = 0; i < 1024; i++) { keys[i] = 0; }
}
int Window::Initialise()
{
	if (!glfwInit()) { return 1; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Practica 4 - Gato Robot", NULL, NULL);
	if (!mainWindow) { glfwTerminate(); return 1; }
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);
	createCallbacks();
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { glfwDestroyWindow(mainWindow); glfwTerminate(); return 1; }
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange() { GLfloat theChange = xChange; xChange = 0.0f; return theChange; }
GLfloat Window::getYChange() { GLfloat theChange = yChange; yChange = 0.0f; return theChange; }

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, GL_TRUE); }

	// Controles del Gato Robot
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		// Pata Frontal Izquierda
		if (key == GLFW_KEY_T) theWindow->pataFI_1 += 5.0f;
		if (key == GLFW_KEY_Y) theWindow->pataFI_2 += 5.0f;
		// Pata Frontal Derecha
		if (key == GLFW_KEY_G) theWindow->pataFD_1 += 5.0f;
		if (key == GLFW_KEY_H) theWindow->pataFD_2 += 5.0f;
		// Pata Trasera Izquierda
		if (key == GLFW_KEY_U) theWindow->pataTI_1 += 5.0f;
		if (key == GLFW_KEY_I) theWindow->pataTI_2 += 5.0f;
		// Pata Trasera Derecha
		if (key == GLFW_KEY_J) theWindow->pataTD_1 += 5.0f;
		if (key == GLFW_KEY_K) theWindow->pataTD_2 += 5.0f;
		// Extras
		if (key == GLFW_KEY_C) theWindow->cola += 5.0f;
		if (key == GLFW_KEY_V) theWindow->orejaI += 5.0f;
		if (key == GLFW_KEY_B) theWindow->orejaD += 5.0f;
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) theWindow->keys[key] = true;
		else if (action == GLFW_RELEASE) theWindow->keys[key] = false;
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (theWindow->mouseFirstMoved) { theWindow->lastX = xPos; theWindow->lastY = yPos; theWindow->mouseFirstMoved = false; }
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}
Window::~Window() { glfwDestroyWindow(mainWindow); glfwTerminate(); }