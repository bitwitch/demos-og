#version 410

in vec4 gl_FragCoord;

uniform float elapsed_time;
uniform int buffer_width;
uniform int buffer_height;

uniform samplerBuffer palette;

out vec4 fragColor;

void main() {
    int y = int(gl_FragCoord.y + 0.5);
    int x = int(gl_FragCoord.x + 0.5);

    int off_x1 = int(0.5*buffer_width  * cos(elapsed_time*0.10309));
    int off_y1 = int(0.5*buffer_height * sin(elapsed_time*0.08130));
    int off_x2 = int(0.5*buffer_width  * sin(elapsed_time*0.07299));

    float center_dist = distance(vec2(0.5*buffer_width, 0.5*buffer_height), 
                                 vec2(x+off_x1, y+off_y1));
    
    int plasma1 = int(128 + 127 * sin(center_dist * 0.0666));
    int plasma2 = int(128 + 127 * sin((x+off_x2) * 0.0625));

    int color_val = int(mod(plasma1+plasma2, 255));

    float r = texelFetch(palette, color_val*3).r;
    float g = texelFetch(palette, color_val*3+1).r;
    float b = texelFetch(palette, color_val*3+2).r;

    fragColor = vec4(r, g, b, 1.0); 
}

