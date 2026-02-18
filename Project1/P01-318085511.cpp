#include <stdio.h>
#include <string.h>
#include <vector>
#include <time.h> // Para el random

#include <glew.h>
#include <glfw3.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

Window mainWindow;
std::vector<MeshColor*> listaLetras; // Contenedor para tus iniciales
Shader shader1;

// Shaders simples (como texto) para que no necesites archivos externos por ahora
static const char* vShader = " #version 330\n layout (location = 0) in vec3 pos; void main() { gl_Position = vec4(pos, 1.0); }";
static const char* fShader = " #version 330\n out vec4 color; void main() { color = vec4(1.0f, 1.0f, 1.0f, 1.0f); }";

void CrearNombre() {
    GLfloat verticesA[] = {
        -0.80f, -0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.70f, -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.75f, -0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.80f, -0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.75f, -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.70f, -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.60f, -0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.70f, -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.65f, -0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.60f, -0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.65f, -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.70f, -0.50f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.75f, -0.75f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.65f, -0.75f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.65f, -0.70f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.75f, -0.75f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.75f, -0.70f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.65f, -0.70f, 0.00f,  1.0f, 1.0f, 1.0f
    };
    MeshColor* objA = new MeshColor();
    objA->CreateMeshColor(verticesA, 108);
    listaLetras.push_back(objA);

    GLfloat verticesU[] = {
        -0.15f,  0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.15f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.05f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.15f,  0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.05f,  0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.05f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.15f,  0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.15f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.05f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.15f,  0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.05f,  0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.05f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.15f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.15f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.15f, -0.30f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.15f, -0.20f, 0.00f,  1.0f, 1.0f, 1.0f,
        -0.15f, -0.30f, 0.00f,  1.0f, 1.0f, 1.0f,
         0.15f, -0.30f, 0.00f,  1.0f, 1.0f, 1.0f
    };
    MeshColor* objU = new MeshColor();
    objU->CreateMeshColor(verticesU, 108);
    listaLetras.push_back(objU);

    GLfloat verticesR[] = {
        0.60f,  0.40f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.60f,  0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.40f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.60f,  0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.40f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.82f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.90f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.82f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.82f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.77f,  0.82f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.82f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.77f,  0.82f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.77f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.73f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.73f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.73f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.40f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.75f,  0.40f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.68f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.78f,  0.65f, 0.00f,  1.0f, 1.0f, 1.0f,
        0.85f,  0.40f, 0.00f,  1.0f, 1.0f, 1.0f
    };
    MeshColor* objR = new MeshColor();
    objR->CreateMeshColor(verticesR, 180);
    listaLetras.push_back(objR);
}

int main() {
    srand((unsigned)time(NULL)); // Semilla para el random

    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    CrearNombre();
    shader1.CreatefromString(vShader, fShader);

    double lastTime = glfwGetTime();
    float r = 0.0f, g = 0.0f, b = 0.0f;

    while (!mainWindow.getShouldClose()) {
        glfwPollEvents();

        // Lógica de color aleatorio cada 2 segundos
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 2.0) {
            r = (float)rand() / (float)RAND_MAX;
            g = (float)rand() / (float)RAND_MAX;
            b = (float)rand() / (float)RAND_MAX;
            lastTime = currentTime;
        }

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader1.useShader();
        // Dibujamos todas las letras que estén en la lista
        for (size_t i = 0; i < listaLetras.size(); i++) {
            listaLetras[i]->RenderMeshColor();
        }

        mainWindow.swapBuffers();
    }
    return 0;
}