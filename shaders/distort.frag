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

    float offX = (sin(x*0.05+elapsed_time*0.10309) 
                + sin(x*y*0.002+elapsed_time*0.0823) 
                + sin((x+y)*0.01-elapsed_time*0.0911) 
                + sin((y-x)*0.014+elapsed_time*0.145) 
                + sin((x+4*y)*0.014-elapsed_time*0.0723)) / 5;

    float offY = (cos(x*0.05+elapsed_time*0.0909) 
                + cos(x*y*0.002+elapsed_time*0.1093) 
                + cos((x+y)*0.01-elapsed_time*0.0821) 
                + cos((y-x)*0.014+elapsed_time*0.095) 
                + cos((x+4*y)*0.014-elapsed_time*0.1003)) / 5;




    //tex_coord.x += offX; 
    //tex_coord.y += offY; 

    // wave
    tex_coord.x += sin(tex_coord.y * 4*2*3.14159 + elapsed_time) / 100;

    fragColor = texture(image, tex_coord);
}

