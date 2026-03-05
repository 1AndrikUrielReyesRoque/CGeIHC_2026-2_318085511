//práctica 3: Modelado Geométrico y Cámara Sintética.
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
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks



glm::vec3 v0(0.0f, 0.6f, 0.0f);
glm::vec3 v1(-0.5f, -0.3f, 0.5f);
glm::vec3 v2(0.5f, -0.3f, 0.5f);
glm::vec3 v3(0.0f, -0.3f, -0.6f);

float autoRotateAngle = 0.0f;

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CrearCarasTetraedro()
{
	// Cara frontal
	GLfloat cara1[] = {
		 0.0f,  0.6f,  0.0f,
		-0.5f, -0.3f,  0.5f,
		 0.5f, -0.3f,  0.5f
	};
	unsigned int ind1[] = { 0,1,2 };

	Mesh* c1 = new Mesh();
	c1->CreateMesh(cara1, ind1, 9, 3);
	meshList.push_back(c1);

	// Cara derecha
	GLfloat cara2[] = {
		 0.0f,  0.6f,  0.0f,
		 0.5f, -0.3f,  0.5f,
		 0.0f, -0.3f, -0.6f
	};
	unsigned int ind2[] = { 0,1,2 };

	Mesh* c2 = new Mesh();
	c2->CreateMesh(cara2, ind2, 9, 3);
	meshList.push_back(c2);


	// Cara izquierda
	GLfloat cara3[] = {
		 0.0f,  0.6f,  0.0f,
		 0.0f, -0.3f, -0.6f,
		-0.5f, -0.3f,  0.5f
	};
	unsigned int ind3[] = { 0,1,2 };

	Mesh* c3 = new Mesh();
	c3->CreateMesh(cara3, ind3, 9, 3);
	meshList.push_back(c3);


	// Base
	GLfloat cara4[] = {
		-0.5f, -0.3f,  0.5f,
		 0.0f, -0.3f, -0.6f,
		 0.5f, -0.3f,  0.5f
	};
	unsigned int ind4[] = { 0,1,2 };

	Mesh* c4 = new Mesh();
	c4->CreateMesh(cara4, ind4, 9, 3);
	meshList.push_back(c4);


}

void CrearMarcosTetraedro(std::vector<Mesh*>& meshList)
{
	float t = 0.07f;
	float offset = 0.0001f;

	// Esta función interna recibe los tres vértices de una cara y construye el borde negro
	auto CrearMarcoCara = [&](glm::vec3 A, glm::vec3 B, glm::vec3 C)
		{
			glm::vec3 centro = (A + B + C) / 3.0f;

			// Movemos cada vértice un poco hacia el centro para dejar espacio al borde
			glm::vec3 A2 = A + (centro - A) * t;
			glm::vec3 B2 = B + (centro - B) * t;
			glm::vec3 C2 = C + (centro - C) * t;

			// Elevamos ligeramente sobre la superficie para evitar que el color tape el marco
			glm::vec3 normal = glm::normalize(glm::cross(B - A, C - A));
			A += normal * offset;
			B += normal * offset;
			C += normal * offset;
			A2 += normal * offset;
			B2 += normal * offset;
			C2 += normal * offset;

			// Dos triángulos por cada lado de la cara forman la franja negra del borde
			GLfloat marco[] = {
				A.x,  A.y,  A.z,   B.x,  B.y,  B.z,   B2.x, B2.y, B2.z,
				A.x,  A.y,  A.z,   B2.x, B2.y, B2.z,   A2.x, A2.y, A2.z,
				B.x,  B.y,  B.z,   C.x,  C.y,  C.z,   C2.x, C2.y, C2.z,
				B.x,  B.y,  B.z,   C2.x, C2.y, C2.z,   B2.x, B2.y, B2.z,
				C.x,  C.y,  C.z,   A.x,  A.y,  A.z,   A2.x, A2.y, A2.z,
				C.x,  C.y,  C.z,   A2.x, A2.y, A2.z,   C2.x, C2.y, C2.z
			};

			unsigned int indices[] = {
				0,1,2,  3,4,5,
				6,7,8,  9,10,11,
				12,13,14,  15,16,17
			};

			Mesh* m = new Mesh();
			m->CreateMesh(marco, indices, 54, 18);
			meshList.push_back(m);
		};

	CrearMarcoCara(v0, v1, v2); // frontal
	CrearMarcoCara(v0, v2, v3); // derecha
	CrearMarcoCara(v0, v3, v1); // izquierda
	CrearMarcoCara(v1, v3, v2); // base
}

// Dibuja una franja negra delgada entre dos puntos sobre la superficie de una cara
void CrearDivisionTrapecio(
	glm::vec3 a, glm::vec3 b,
	glm::vec3 normal,
	float t, float offset,
	std::vector<Mesh*>& meshList)
{
	glm::vec3 dir = glm::normalize(b - a);

	// Acortamos el segmento en ambos extremos para que las líneas no se toquen en las esquinas
	float shrink = t * 1.2f;
	a = a + dir * shrink;
	b = b - dir * shrink;

	glm::vec3 perp = glm::normalize(glm::cross(normal, dir));
	glm::vec3 despl = normal * offset;

	glm::vec3 p1 = a - perp * t + despl;
	glm::vec3 p2 = a + perp * t + despl;
	glm::vec3 p3 = b + perp * t + despl;
	glm::vec3 p4 = b - perp * t + despl;

	GLfloat vertices[] = {
		p1.x, p1.y, p1.z,
		p2.x, p2.y, p2.z,
		p3.x, p3.y, p3.z,
		p4.x, p4.y, p4.z
	};

	unsigned int indices[] = { 0,1,2, 0,2,3 };

	Mesh* division = new Mesh();
	division->CreateMesh(vertices, indices,
		sizeof(vertices) / sizeof(GLfloat),
		sizeof(indices) / sizeof(unsigned int));
	meshList.push_back(division);
}

// Divide cada cara en 9 triángulos trazando líneas paralelas a cada lado
void CrearDivisionesCara(
	glm::vec3 v0, glm::vec3 v1, glm::vec3 v2,
	std::vector<Mesh*>& meshList)
{
	float t = 0.009f;
	float offset = 0.001f;
	glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

	// Líneas paralelas al lado base, una a un tercio y otra a dos tercios de altura
	for (int i = 1; i <= 2; i++) {
		float alpha = i / 3.0f;
		glm::vec3 a1 = glm::mix(v0, v1, alpha);
		glm::vec3 a2 = glm::mix(v0, v2, alpha);
		CrearDivisionTrapecio(a1, a2, normal, t, offset, meshList);
	}

	// Líneas paralelas al lado izquierdo
	for (int i = 1; i <= 2; i++) {
		float alpha = i / 3.0f;
		glm::vec3 b1 = glm::mix(v1, v0, alpha);
		glm::vec3 b2 = glm::mix(v1, v2, alpha);
		CrearDivisionTrapecio(b1, b2, normal, t, offset, meshList);
	}

	// Líneas paralelas al lado derecho
	for (int i = 1; i <= 2; i++) {
		float alpha = i / 3.0f;
		glm::vec3 c1 = glm::mix(v2, v0, alpha);
		glm::vec3 c2 = glm::mix(v2, v1, alpha);
		CrearDivisionTrapecio(c1, c2, normal, t, offset, meshList);
	}
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	// Orden de creación determina el índice en meshList
	CrearCubo();                  // índice 0
	CrearPiramideTriangular();    // índice 1
	CrearCilindro(5, 1.0f);      // índice 2  — resolución 5, radio 1
	CrearCono(25, 2.0f);         // índice 3  — resolución 25, radio 2
	CrearPiramideCuadrangular(); // índice 4
	CrearCarasTetraedro();       // índices 5 al 8
	CrearMarcosTetraedro(meshList);           // índices 9 al 12
	CrearDivisionesCara(v0, v1, v2, meshList); // cara frontal
	CrearDivisionesCara(v0, v2, v3, meshList); // cara derecha
	CrearDivisionesCara(v0, v3, v1, meshList); // cara izquierda
	CrearDivisionesCara(v1, v3, v2, meshList); // base
	CreateShaders();

	// La cámara parte de la posición 0,0,0 mirando hacia -Z con yaw -60 y velocidad 0.3
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		-60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformColor = 0;

	glm::mat4 projection = glm::perspective(
		glm::radians(60.0f),
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
		0.1f, 100.0f);

	sp.init();
	sp.load();

	glm::mat4 model(1.0);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 viewMat = camera.calculateViewMatrix();
		glm::mat4 currentModel, model;
		glm::vec3 color;

		// El ángulo crece con el tiempo para que el Pyraminx gire solo
		autoRotateAngle += deltaTime * 2.0f;

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(2.0f));

		// Rotación doble: en Y para girar alrededor, en X para inclinarse levemente
		model = glm::rotate(model, glm::radians(autoRotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(autoRotateAngle * 0.5f), glm::vec3(1.0f, 0.0f, 0.0f));

		currentModel = viewMat * model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(currentModel));

		// Caras con sus colores: rojo, verde, azul y amarillo
		color = glm::vec3(1, 0, 0); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[5]->RenderMesh();
		color = glm::vec3(0, 1, 0); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[6]->RenderMesh();
		color = glm::vec3(0, 0, 1); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[7]->RenderMesh();
		color = glm::vec3(1, 1, 0); glUniform3fv(uniformColor, 1, glm::value_ptr(color)); meshList[8]->RenderMesh();

		// Marcos exteriores de cada cara en negro
		color = glm::vec3(0, 0, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		for (int i = 9; i <= 12; i++) meshList[i]->RenderMesh();

		// Líneas internas que dividen cada cara en 9 triángulos, también en negro
		for (size_t i = 13; i < meshList.size(); i++) meshList[i]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}