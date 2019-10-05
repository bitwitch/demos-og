#version 410

in vec2 texture_coord;

uniform float elapsed_time;
uniform int buffer_width;
uniform int buffer_height;

uniform sampler2D image;

out vec4 fragColor;

void main() {
    vec2 tex_coord = texture_coord;
    float x = tex_coord.x * buffer_width;
    float y = tex_coord.y * buffer_height;

    float offX = (sin(x*0.05*elapsed_time) 
                + sin(x*y*0.0005*elapsed_time) 
                + sin((x+y)*0.01*elapsed_time) 
                + sin((y-x)*0.014*elapsed_time) 
                + sin((x+4*y)*0.014*elapsed_time)
                + 2*sin( distance(vec2(buffer_width - 200, buffer_height*0.75*0.125), 
                                  vec2(x, y*0.125))*0.025*elapsed_time)) / 7 * 0.01;
     float offY = (cos(x*0.032*elapsed_time) 
                 + cos(x*y*0.00056*elapsed_time) 
                 + 2*cos((x+y)*0.0073*elapsed_time) 
                 + cos((y-x)*0.018*elapsed_time) 
                 + 2*cos((x+8*y)*0.017*elapsed_time)
                 + cos( distance(vec2(buffer_width, buffer_height*0.111), 
                                 vec2(x, y*0.111))*0.02*elapsed_time)) / 8 * 0.1;

    tex_coord.x += offX; 
    tex_coord.y += offY; 

    // wave
    //tex_coord.x += sin(tex_coord.y * 4*2*3.14159 + elapsed_time) / 100;

    fragColor = texture(image, tex_coord);
}

