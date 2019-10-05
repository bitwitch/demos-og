#version 410

in vec2 texture_coord;

uniform float elapsedTime;
uniform int buffer_width;
uniform int buffer_height;

uniform sampler2D image;

out vec4 fragColor;

void main() {
    fragColor = texture(image, texture_coord);
}

