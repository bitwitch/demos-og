#version 410

in vec4 gl_FragCoord;

uniform float elapsedTime;
uniform int buffer_width;
uniform int buffer_height;

uniform samplerBuffer palette;

// no need to calculate these per pixel
//uniform int offX;
//uniform int offY;

out vec4 fragColor;

void main() {
    int y = int( gl_FragCoord.y + 0.5 ); 
    int x = int( gl_FragCoord.x + 0.5 ); 

    int off_x1 = int(0.5*buffer_width   * cos(elapsedTime*0.11111));
    int off_y1 = int(0.5*buffer_height  * sin(elapsedTime*0.08333));
    int off_x2 = int(0.5*buffer_width   * sin(-elapsedTime*0.090909));
    int off_y2 = int(0.5*buffer_height  * cos(-elapsedTime*0.14285));
    int off_x3 = int(0.5*buffer_width   * sin(elapsedTime*0.07299));
    int off_y4 = int(0.5*buffer_height  * sin(-elapsedTime*0.1));

    float center_dist = distance(vec2(0.5*buffer_width, 0.5*buffer_height), 
                                 vec2(x+off_x1, y+off_y1));
    
    int plasma1 = int(64 + 63 * sin(center_dist * 0.0666));
    //int plasma2 = int(64 + 63 * sin(float(x+off_x2)/(37+15*cos(float(y+off_y2)/74)))
    //                          * cos(float(y+off_y2)/(31+11*sin(float(x+off_x2)/57))));

    int plasma3 = int(64 + 63 * sin((x+off_x3)*0.0625));
    int plasma4 = int(64 + 63 * sin((y+off_y4)*0.125));

    int color_val = int(mod(plasma1+plasma3+plasma4, 255));

    float r = texelFetch(palette, color_val*3).r;
    float g = texelFetch(palette, color_val*3+1).r;
    float b = texelFetch(palette, color_val*3+2).r;

    fragColor = vec4(r, g, b, 1.0); 
}
