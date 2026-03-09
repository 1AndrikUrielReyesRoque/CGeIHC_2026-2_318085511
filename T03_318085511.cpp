/*
Tarea 3: Programa de Dibujo de Circunferencia por Bresenham
Implementación en OpenGL Moderno (Core Profile)
*/

#include <iostream>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Window.h"
#include "Shader.h"

using namespace std;

Window mainWindow;
vector<Shader> shaderList;

// Shaders básicos
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

// Variables globales para los vértices
GLuint VAO, VBO;
int numVerticesCirculo = 0;
int numVerticesEjes = 0;

void CreateShaders() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

// Algoritmo de Bresenham para generar los puntos de la circunferencia
void generarCircunferenciaBresenham(int xc, int yc, int r, vector<GLfloat>& vertices) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    // Lambda para agregar los 8 puntos simétricos
    auto agregarSimetria = [&](int cx, int cy, int x, int y) {
        // Cuadrante 1
        vertices.push_back(cx + x); vertices.push_back(cy + y); vertices.push_back(0.0f);
        vertices.push_back(cx + y); vertices.push_back(cy + x); vertices.push_back(0.0f);
        // Cuadrante 2
        vertices.push_back(cx - x); vertices.push_back(cy + y); vertices.push_back(0.0f);
        vertices.push_back(cx - y); vertices.push_back(cy + x); vertices.push_back(0.0f);
        // Cuadrante 3
        vertices.push_back(cx - x); vertices.push_back(cy - y); vertices.push_back(0.0f);
        vertices.push_back(cx - y); vertices.push_back(cy - x); vertices.push_back(0.0f);
        // Cuadrante 4
        vertices.push_back(cx + x); vertices.push_back(cy - y); vertices.push_back(0.0f);
        vertices.push_back(cx + y); vertices.push_back(cy - x); vertices.push_back(0.0f);
        };

    agregarSimetria(xc, yc, x, y);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else {
            d = d + 4 * x + 6;
        }
        agregarSimetria(xc, yc, x, y);
    }
}

// Genera los ejes X y Y que cruzan por el centro
void generarEjes(int xc, int yc, int r, vector<GLfloat>& vertices) {
    float offset = r * 1.5f; // Que los ejes sobresalgan un poco

    // Eje X
    vertices.push_back(xc - offset); vertices.push_back(yc); vertices.push_back(0.0f);
    vertices.push_back(xc + offset); vertices.push_back(yc); vertices.push_back(0.0f);

    // Eje Y
    vertices.push_back(xc); vertices.push_back(yc - offset); vertices.push_back(0.0f);
    vertices.push_back(xc); vertices.push_back(yc + offset); vertices.push_back(0.0f);
}

int main() {
    int cx, cy, radio;

    // 1. REQUERIMIENTO: Consola para entrada de datos y validación
    cout << "=========================================\n";
    cout << "   DIBUJO DE CIRCUNFERENCIA (BRESENHAM)  \n";
    cout << "=========================================\n\n";

    cout << "Ingrese la coordenada X del centro: ";
    cin >> cx;
    cout << "Ingrese la coordenada Y del centro: ";
    cin >> cy;

    do {
        cout << "Ingrese el RADIO (debe ser un numero entero mayor a 0): ";
        cin >> radio;
        if (radio <= 0) {
            cout << "ERROR: El radio debe ser estrictamente mayor que 0. Intente de nuevo.\n";
        }
    } while (radio <= 0);

    // 2. GENERACIÓN DE VÉRTICES
    vector<GLfloat> vertices;
    generarCircunferenciaBresenham(cx, cy, radio, vertices);
    numVerticesCirculo = vertices.size() / 3;

    generarEjes(cx, cy, radio, vertices);
    numVerticesEjes = 4; // 2 para X, 2 para Y

    // 3. INICIALIZACIÓN DE VENTANA (OpenGL Moderno)
    mainWindow = Window(800, 800);
    mainWindow.Initialise();
    CreateShaders();
    glDisable(GL_DEPTH_TEST);

    // Configuración de VAO y VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 4. ADAPTACIÓN DEL VIEWPORT (Proyección Ortogonal)
    // Se ajusta la cámara para que el radio cubra exactamente 3/4 de la ventana (1.0f / 0.75f = 1.33f)
    float margen = radio * 1.333f;
    glm::mat4 projection = glm::ortho(cx - margen, cx + margen, cy - margen, cy + margen, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f); // Vista neutra para 2D
    glm::mat4 model = glm::mat4(1.0f);

    glPointSize(3.0f); // Puntos más grandes para que la circunferencia se vea sólida

    // Loop de renderizado
    while (!mainWindow.getShouldClose()) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].useShader();
        glUniformMatrix4fv(shaderList[0].getModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderList[0].getProjectLocation(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shaderList[0].getViewLocation(), 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // Dibujar ejes (Color Rojo)
        glm::vec3 colorEjes = glm::vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(shaderList[0].getColorLocation(), 1, glm::value_ptr(colorEjes));
        glDrawArrays(GL_LINES, numVerticesCirculo, numVerticesEjes);

        // Dibujar circunferencia (Color Cian)
        glm::vec3 colorCirculo = glm::vec3(0.0f, 1.0f, 1.0f);
        glUniform3fv(shaderList[0].getColorLocation(), 1, glm::value_ptr(colorCirculo));
        glDrawArrays(GL_POINTS, 0, numVerticesCirculo);

        glBindVertexArray(0);
        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}