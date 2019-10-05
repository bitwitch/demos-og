#version 410

layout (location = 0) in vec4 v_coord;
layout (location = 1) in vec2 tex_coord;

out vec2 texture_coord;

void main() {
  gl_Position = v_coord;
  texture_coord = tex_coord;
}

