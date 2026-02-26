#include <stdio.h>
#include <string.h>
#include <vector>
#include <glew.h>
#include <glfw3.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

Window mainWindow;
std::vector<MeshColor*> listaLetras;

// Creamos 3 shaders en lugar de 1
Shader shaderRojo, shaderVerde, shaderAzul;

// Vertex shader de tu P1 (Sin matrices, usa tus coordenadas directas)
static const char* vShader = " #version 330\n layout (location = 0) in vec3 pos; void main() { gl_Position = vec4(pos, 1.0); }";

// Fragment shaders de colores fijos para cada letra
static const char* fShaderRojo = " #version 330\n out vec4 color; void main() { color = vec4(1.0f, 0.0f, 0.0f, 1.0f); }";
static const char* fShaderVerde = " #version 330\n out vec4 color; void main() { color = vec4(0.0f, 1.0f, 0.0f, 1.0f); }";
static const char* fShaderAzul = " #version 330\n out vec4 color; void main() { color = vec4(0.0f, 0.5f, 1.0f, 1.0f); }";

void CrearNombre() {
    // TUS MISMOS VÉRTICES INTACTOS
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
    listaLetras.push_back(objA); // Índice 0

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
    listaLetras.push_back(objU); // Índice 1

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
    listaLetras.push_back(objR); // Índice 2
}

int main() {
    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    CrearNombre();

    // Cargamos los 3 shaders (Pinturas) en lugar de uno solo
    shaderRojo.CreatefromString(vShader, fShaderRojo);
    shaderVerde.CreatefromString(vShader, fShaderVerde);
    shaderAzul.CreatefromString(vShader, fShaderAzul);

    while (!mainWindow.getShouldClose()) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fondo oscurito para ver bien
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. DIBUJAR LA 'A' DE ROJO
        shaderRojo.useShader();
        listaLetras[0]->RenderMeshColor();

        // 2. DIBUJAR LA 'U' DE VERDE
        shaderVerde.useShader();
        listaLetras[1]->RenderMeshColor();

        // 3. DIBUJAR LA 'R' DE AZUL
        shaderAzul.useShader();
        listaLetras[2]->RenderMeshColor();

        mainWindow.swapBuffers();
    }
    return 0;
}