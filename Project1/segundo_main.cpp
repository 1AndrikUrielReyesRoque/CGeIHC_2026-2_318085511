#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

// Shaders con formato limpio
static const char* vShader = "                  \n\
#version 330                                    \n\
layout (location = 0) in vec3 pos;              \n\
void main()                                     \n\
{                                               \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n\
}";

static const char* fShader = "                  \n\
#version 330                                    \n\
out vec4 color;                                 \n\
void main()                                     \n\
{                                               \n\
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);        \n\
}";

void CrearFiguras() {
    GLfloat vertices[] = {
        // --- ROMBO 2 triángulos ---
        // Triángulo Superior
        0.0f,  0.8f, 0.0f,   
       -0.3f,  0.5f, 0.0f,   
        0.3f,  0.5f, 0.0f,  
        // Triángulo Inferior
        0.0f,  0.2f, 0.0f,   
       -0.3f,  0.5f, 0.0f,   
        0.3f,  0.5f, 0.0f,   

        // --- TRAPECIO ISÓSCELES 4 triángulo
        // 1. Triángulo Lateral Izquierdo
       -0.6f, -0.8f, 0.0f,   
       -0.3f, -0.8f, 0.0f,   
       -0.3f, -0.3f, 0.0f,   

       // 2. Rectángulo Central mitad 1
      -0.3f, -0.8f, 0.0f,   // Abajo Izquierda
       0.3f, -0.8f, 0.0f,   // Abajo Derecha
      -0.3f, -0.3f, 0.0f,   // Arriba Izquierda

      // 3. Rectángulo Central mitad2
      0.3f, -0.8f, 0.0f,   
      0.3f, -0.3f, 0.0f,   
     -0.3f, -0.3f, 0.0f,  

     // 4. Triángulo Lateral Derecho
     0.6f, -0.8f, 0.0f,   
     0.3f, -0.8f, 0.0f,   
     0.3f, -0.3f, 0.0f    
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Funciones de compilación (sin cambios, solo limpieza de logs)
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1] = { shaderCode };
    GLint codeLength[1] = { (GLint)strlen(shaderCode) };
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    glLinkProgram(shader);
}

int main() {
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1: Figuras y Colores", NULL, NULL);
    if (!mainWindow) { glfwTerminate(); return 1; }

    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, BufferWidth, BufferHeight);

    CrearFiguras();
    CompileShaders();

    // Variables de tiempo para el ciclo RGB
    double lastTime = glfwGetTime();
    int colorIndex = 0;

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        // 1. Lógica del cambio de color (cada 1.0 segundo)
        double currentTime = glfwGetTime(); //usar funcion de la libreria
        if (currentTime - lastTime >= 1.0) {
            colorIndex = (colorIndex + 1) % 3;
            lastTime = currentTime;
        }
        //RGB
        if (colorIndex == 0)      glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Rojo
        else if (colorIndex == 1) glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // Verde
        else                      glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Azul

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(VAO);

        // un total de 18 vertices, 3 por cada triangulo
        glDrawArrays(GL_TRIANGLES, 0, 18);

        glBindVertexArray(0);
        glfwSwapBuffers(mainWindow);
    }
    return 0;
}