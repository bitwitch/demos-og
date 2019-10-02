#version 410

layout (location = 0) in vec4 v_coord;

void main() {
  gl_Position = v_coord;
}

