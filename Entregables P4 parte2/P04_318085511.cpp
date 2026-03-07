/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"

using std::vector;
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo() {
	unsigned int cubo_indices[] = { 0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3 };
	GLfloat cubo_vertices[] = { -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f };
	Mesh* cubo = new Mesh(); cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36); meshList.push_back(cubo);
}

void CrearPiramideTriangular() {
	unsigned int indices[] = { 0,1,2, 1,3,2, 3,0,2, 1,0,3 };
	GLfloat vertices[] = { -0.5f, -0.5f,0.0f, 0.5f,-0.5f,0.0f, 0.0f,0.5f, -0.25f, 0.0f,-0.5f,-0.5f };
	Mesh* obj1 = new Mesh(); obj1->CreateMesh(vertices, indices, 12, 12); meshList.push_back(obj1);
}

void CrearCilindro(int res, float R) {
	int n, i; GLfloat dt = 2 * PI / res, x, z, y = -0.5f; vector<GLfloat> vertices; vector<unsigned int> indices;
	for (n = 0; n <= (res); n++) {
		if (n != res) { x = R * cos((n)*dt); z = R * sin((n)*dt); }
		else { x = R * cos((0) * dt); z = R * sin((0) * dt); }
		for (i = 0; i < 6; i++) {
			switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(y); break; case 2: vertices.push_back(z); break; case 3: vertices.push_back(x); break; case 4: vertices.push_back(0.5); break; case 5: vertices.push_back(z); break; }
		}
	}
	for (n = 0; n <= (res); n++) { x = R * cos((n)*dt); z = R * sin((n)*dt); for (i = 0; i < 3; i++) { switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(-0.5f); break; case 2: vertices.push_back(z); break; } } }
	for (n = 0; n <= (res); n++) { x = R * cos((n)*dt); z = R * sin((n)*dt); for (i = 0; i < 3; i++) { switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(0.5); break; case 2: vertices.push_back(z); break; } } }
																														  for (i = 0; i < vertices.size(); i++) indices.push_back(i);
																														  Mesh* cilindro = new Mesh(); cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size()); meshList.push_back(cilindro);
}

void CrearCono(int res, float R) {
	int n, i; GLfloat dt = 2 * PI / res, x, z, y = -0.5f; vector<GLfloat> vertices; vector<unsigned int> indices;
	vertices.push_back(0.0); vertices.push_back(0.5); vertices.push_back(0.0);
	for (n = 0; n <= (res); n++) { x = R * cos((n)*dt); z = R * sin((n)*dt); for (i = 0; i < 3; i++) { switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(y); break; case 2: vertices.push_back(z); break; } } }
																														  vertices.push_back(R * cos(0) * dt); vertices.push_back(-0.5); vertices.push_back(R * sin(0) * dt);
																														  for (i = 0; i < res + 2; i++) indices.push_back(i);
																														  Mesh* cono = new Mesh(); cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2); meshList.push_back(cono);
}

void CreateShaders() { Shader* shader1 = new Shader(); shader1->CreateFromFiles(vShader, fShader); shaderList.push_back(*shader1); }

int main()
{
	mainWindow = Window(1000, 800);
	mainWindow.Initialise();

	CrearCubo();               // meshList[0] - Torso, Cola
	CrearPiramideTriangular(); // meshList[1] - Orejas
	CrearCilindro(30, 1.0f);   // meshList[2] - Muslos y pantorrillas
	CrearCono(25, 1.0f);       // meshList[3]
	CreateShaders();

	camera = Camera(glm::vec3(4.5f, 1.0f, 2.5f), glm::vec3(0.0f, 1.0f, 0.0f), -115.0f, -5.0f, 0.2f, 0.2f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	sp.init(); sp.load();
	glm::mat4 model(1.0); glm::mat4 modelaux(1.0); glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	lastTime = glfwGetTime();

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); deltaTime = now - lastTime; deltaTime += (now - lastTime) / limitFPS; lastTime = now;
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime); camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation(); uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation(); uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		modelaux2 = model;

		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.3f, 0.3f, 0.3f); glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.8f, 0.5f, 0.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 0.35f));
		model = glm::rotate(model, glm::radians(mainWindow.getOrejaI()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.8f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.2f, 0.2f); glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, -0.35f));
		model = glm::rotate(model, glm::radians(-mainWindow.getOrejaD()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.8f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.5f, 0.2f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f + mainWindow.getCola()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 1.6f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.3f, 0.3f, 0.3f); glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, 0.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataFI_1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataFI_2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.2f, 0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, -0.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataFD_1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataFD_2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.2f, 0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataTI_1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataTI_2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.2f, 0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		model = modelaux2;
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, -0.6f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataTD_1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -1.2f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getPataTD_2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render();

		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.2f, 0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.4f, 0.4f, 0.4f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[2]->RenderMeshGeometry();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}