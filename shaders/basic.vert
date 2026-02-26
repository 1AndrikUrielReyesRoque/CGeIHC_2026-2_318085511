#version 330

// Atributo de posición (Location 0)
layout (location = 0) in vec3 pos;

// Matrices que ya tienes en tu clase Shader.cpp
uniform mat4 model;
uniform mat4 projection;

void main() {
    // Multiplicamos para obtener la posición final en el mundo 3D
    gl_Position = projection * model * vec4(pos, 1.0);
}