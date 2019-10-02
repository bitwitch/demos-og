#version 410

in vec4 gl_FragCoord;

uniform float elapsedTime;
uniform int buffer_width;
uniform int buffer_height;
uniform samplerBuffer lookup;

out vec4 fragColor;

void main() {
    int y = int( gl_FragCoord.y + 0.5 ); 
    int x = int( gl_FragCoord.x + 0.5 ); 

    int index = y * buffer_width + x;

    float y_color = float(y) / float(buffer_height);

    float color = texelFetch(lookup, index).r;
    fragColor = vec4(0.0, color, 0.0, 1.0); 
    //fragColor = vec4(0.0, 1.0, 0.0, 1.0); 
}
