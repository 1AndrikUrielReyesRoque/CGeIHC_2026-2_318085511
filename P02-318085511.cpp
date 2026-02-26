#include <stdio.h>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

Window mainWindow;

// Aquí vamos a guardar las piezas que forman la casa
std::vector<Mesh*> houseParts;

// Shaders con distintos colores para cada parte
Shader shaderRojo, shaderVerde, shaderAzul, shaderCafe, shaderVOscuro;

void CrearGeometria3D() {

    // Índices del cubo 
    unsigned int cuboIndices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,3,7, 7,4,0,
        1,2,6, 6,5,1,
        3,2,6, 6,7,3,
        0,1,5, 5,4,0
    };

    // Vértices del cubo 
    GLfloat cuboVertices[] = {
        -0.5f,-0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,   0.5f,-0.5f,-0.5f,   0.5f, 0.5f,-0.5f,  -0.5f, 0.5f,-0.5f
    };

    // Creamos el cubo y lo guardamos
    Mesh* cubo = new Mesh();
    cubo->CreateMesh(cuboVertices, cuboIndices, 24, 36);
    houseParts.push_back(cubo);

    // Ahora la pirámide 
    unsigned int pirIndices[] = {
        0,1,2,
        1,3,2,
        3,4,2,
        4,0,2,
        0,1,3, 3,4,0
    };

    GLfloat pirVertices[] = {
        -0.5f, 0.0f,  0.5f,
         0.5f, 0.0f,  0.5f,
         0.0f, 1.0f,  0.0f,
         0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, -0.5f
    };

    Mesh* piramide = new Mesh();
    piramide->CreateMesh(pirVertices, pirIndices, 15, 18);
    houseParts.push_back(piramide);
}

void CargarShaders() {

    // Cargamos los shaders desde los archivos
    shaderRojo.CreateFromFiles("shaders/basic.vert", "shaders/rojo.frag");
    shaderVerde.CreateFromFiles("shaders/basic.vert", "shaders/verde.frag");
    shaderAzul.CreateFromFiles("shaders/basic.vert", "shaders/azul.frag");
    shaderCafe.CreateFromFiles("shaders/basic.vert", "shaders/cafe.frag");
    shaderVOscuro.CreateFromFiles("shaders/basic.vert", "shaders/verde_oscuro.frag");
}

int main() {

    // Ventana de 800x800
    mainWindow = Window(800, 800);
    mainWindow.Initialise();

    // Creamos figuras y cargamos shaders
    CrearGeometria3D();
    CargarShaders();

    // Proyección en perspectiva para que se vea profundidad
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );

    while (!mainWindow.getShouldClose()) {

        glfwPollEvents();

        // Fondo gris oscuro
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);

        // Rotación general para que la casa no se vea plana
        float anguloRotacion = 25.0f;
        glm::vec3 ejeRotacion = glm::vec3(0.0f, 1.0f, 0.0f);

        // Cuerpo principal de la casa (cubo rojo)
        shaderRojo.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -1.0f, -6.0f));
        model = glm::rotate(model, glm::radians(anguloRotacion), ejeRotacion);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(shaderRojo.getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderRojo.getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        houseParts[0]->RenderMesh();

        // Techo azul (pirámide)
        shaderAzul.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -6.0f));
        model = glm::rotate(model, glm::radians(anguloRotacion), ejeRotacion);
        model = glm::scale(model, glm::vec3(2.2f, 1.5f, 2.2f));
        glUniformMatrix4fv(shaderAzul.getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderAzul.getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        houseParts[1]->RenderMesh();

        // Puerta verde 
        shaderVerde.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -1.25f, -4.9f));
        model = glm::rotate(model, glm::radians(anguloRotacion), ejeRotacion);
        model = glm::scale(model, glm::vec3(0.6f, 1.0f, 0.1f));
        glUniformMatrix4fv(shaderVerde.getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderVerde.getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        houseParts[0]->RenderMesh();

        // Tronco del árbol
        shaderCafe.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, -1.0f, -6.0f));
        model = glm::rotate(model, glm::radians(anguloRotacion), ejeRotacion);
        model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
        glUniformMatrix4fv(shaderCafe.getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderCafe.getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        houseParts[0]->RenderMesh();

        // Parte verde oscuro del árbol 
        shaderVOscuro.useShader();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, -0.25f, -6.0f));
        model = glm::rotate(model, glm::radians(anguloRotacion), ejeRotacion);
        model = glm::scale(model, glm::vec3(1.5f, 2.0f, 1.5f));
        glUniformMatrix4fv(shaderVOscuro.getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderVOscuro.getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        houseParts[1]->RenderMesh();

        mainWindow.swapBuffers();
    }

    return 0;
}