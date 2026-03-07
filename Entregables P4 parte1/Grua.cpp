/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
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
Sphere sp = Sphere(1.0, 20, 20);

void CrearCubo()
{
	unsigned int cubo_indices[] = { 0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3 };
	GLfloat cubo_vertices[] = { -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f };
	Mesh* cubo = new Mesh(); cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36); meshList.push_back(cubo);
}

void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = { 0,3,4, 3,2,4, 2,1,4, 1,0,4, 0,1,2, 0,2,4 };
	vector<GLfloat> piramidecuadrangular_vertices = { 0.5f,-0.5f,0.5f, 0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f,-0.5f,0.5f, 0.0f,0.5f,0.0f };
	Mesh* piramide = new Mesh(); piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18); meshList.push_back(piramide);
}

void CrearCilindro(int res, float R) {
	int n, i; GLfloat dt = 2 * PI / res, x, z, y = -0.5f; vector<GLfloat> vertices; vector<unsigned int> indices;
	for (n = 0; n <= (res); n++) {
		if (n != res) { x = R * cos((n)*dt); z = R * sin((n)*dt); }
		else { x = R * cos((0) * dt); z = R * sin((0) * dt); }
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break; case 1: vertices.push_back(y); break; case 2: vertices.push_back(z); break;
			case 3: vertices.push_back(x); break; case 4: vertices.push_back(0.5); break; case 5: vertices.push_back(z); break;
			}
		}
	}
	for (n = 0; n <= (res); n++) { x = R * cos((n)*dt); z = R * sin((n)*dt); for (i = 0; i < 3; i++) { switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(-0.5f); break; case 2: vertices.push_back(z); break; } } }
	for (n = 0; n <= (res); n++) { x = R * cos((n)*dt); z = R * sin((n)*dt); for (i = 0; i < 3; i++) { switch (i) { case 0: vertices.push_back(x); break; case 1: vertices.push_back(0.5); break; case 2: vertices.push_back(z); break; } } }
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);
	Mesh* cilindro = new Mesh(); cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size()); meshList.push_back(cilindro);
}

void CreateShaders() { Shader* shader1 = new Shader(); shader1->CreateFromFiles(vShader, fShader); shaderList.push_back(*shader1); }

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CrearCubo(); CrearCilindro(30, 1.0f); CrearPiramideCuadrangular();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.2f, 0.2f);
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

		// 1. CUERPO (TRACTOR AMARILLO)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -4.0f));
		modelaux2 = model;

		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.9f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		// 2. LLANTAS (Hijas del Cuerpo)
		color = glm::vec3(0.1f, 0.1f, 0.1f); glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		// Llanta 1
		model = modelaux2; model = glm::translate(model, glm::vec3(-1.8f, -0.5f, 2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta1()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); meshList[1]->RenderMeshGeometry();
		// Llanta 2
		model = modelaux2; model = glm::translate(model, glm::vec3(1.8f, -0.5f, 2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta2()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); meshList[1]->RenderMeshGeometry();
		// Llanta 3
		model = modelaux2; model = glm::translate(model, glm::vec3(-1.8f, -0.5f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta3()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); meshList[1]->RenderMeshGeometry();
		// Llanta 4
		model = modelaux2; model = glm::translate(model, glm::vec3(1.8f, -0.5f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta4()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.4f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); meshList[1]->RenderMeshGeometry();

		// 3. BASE PIRAMIDAL
		model = modelaux2; model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.9f, 0.9f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		// 4. BRAZO MORADO (Pivote 1)
		model = modelaux; model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2() + 45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.35f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render(); // Esfera 1
		model = modelaux; model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f)); model = glm::scale(model, glm::vec3(5.0f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.0f, 0.5f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[0]->RenderMesh();

		// 5. BRAZO VERDE (Pivote 2)
		model = modelaux; model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3() - 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.3f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render(); // Esfera 2
		model = modelaux2; model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f)); model = glm::scale(model, glm::vec3(4.0f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.7f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[0]->RenderMesh();

		// 6. BRAZO AZUL (Pivote 3)
		model = modelaux2; model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4() + 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.25f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render(); // Esfera 3
		model = modelaux; model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f)); model = glm::scale(model, glm::vec3(3.0f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.9f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[0]->RenderMesh();

		// 7. CANASTA (NARANJA)
		model = modelaux; model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.2f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); sp.render(); // Esfera 4
		model = modelaux2; model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f)); model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.5f, 0.0f); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[0]->RenderMesh();

		glUseProgram(0); mainWindow.swapBuffers();
	}
	return 0;
}