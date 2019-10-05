#version 410

in vec2 texture_coord;

uniform float elapsed_time;
uniform int buffer_width;
uniform int buffer_height;

uniform sampler2D image;
uniform sampler2D last_frame;
uniform samplerBuffer palette;

out vec4 fragColor;

void main() {

    vec2 left = vec2(texture_coord.x-1, texture_coord.y);
    vec2 right = vec2(texture_coord.x+1, texture_coord.y);
    vec2 left_down = vec2(texture_coord.x-1, texture_coord.y-1);
    vec2 down = vec2(texture_coord.x, texture_coord.y-1);
    vec2 right_down = vec2(texture_coord.x+1, texture_coord.y-1);
    vec2 left_down2 = vec2(texture_coord.x-1, texture_coord.y-2);
    vec2 right_down2 = vec2(texture_coord.x+1, texture_coord.y-2);
    vec2 down2 = vec2(texture_coord.x, texture_coord.y-2);

    vec4 color = (texture(last_frame, left) + texture(last_frame, right) +  
                  texture(last_frame, left_down) + texture(last_frame, down) +  
                  texture(last_frame, right_down) + texture(last_frame, left_down2) +  
                  texture(last_frame, down2) + texture(last_frame, right_down2)) 
                  * 0.125; 

    int index = int((1-texture_coord.y) * 256)*3;
    float r = texelFetch(palette, index).r;
    float g = texelFetch(palette, index+1).r;
    float b = texelFetch(palette, index+2).r;

    //fragColor = vec4(r, g, b, 1.0);

    fragColor = texture(last_frame, texture_coord);
    //fragColor = color;

}

