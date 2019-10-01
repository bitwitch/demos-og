#version 410

out vec4 outputColor;

uniform float loopDuration;
uniform float elapsedTime;
uniform int fbWidth;
uniform int fbHeight;

uniform sampler1D plasma1;
uniform sampler1D plasma2;

void main()
{
    float currTime = mod(elapsedTime, loopDuration);
    float currLerp = currTime / loopDuration;

    int x1 = int( 0.5*fbWidth + (0.5*fbWidth-1) * cos(elapsedTime/9) );
    int x2 = int( 0.5*fbWidth + (0.5*fbWidth-1) * sin(-elapsedTime/11) );
    int x3 = int( 0.5*fbWidth + (0.5*fbWidth-1) * sin(-elapsedTime/13) );
    int y1 = int( 0.5*fbHeight + (0.5*fbHeight-1) * sin(elapsedTime/12) );
    int y2 = int( 0.5*fbHeight + (0.5*fbHeight-1) * cos(-elapsedTime/7) );
    int y3 = int( 0.5*fbHeight + (0.5*fbHeight-1) * cos(-elapsedTime/10) );

    int i_plasma1 = y1 * fbWidth + x1;
    int i_plasma2 = y2 * fbWidth + x2;
    int i_plasma3 = y3 * fbWidth + x3;

    //float color_val = texture(plasma1, i_plasma1).r + texture(plasma2, i_plasma2).r; 
    //float color_val = texture(plasma1, 100).r + texture(plasma2, 125).r; 
    float color_val = texture(plasma1, 0).r / 255.0f;
    //float color_val = texture(plasma1, 0).r;

    
    outputColor = vec4(0.0f, color_val, 0.0f, 1.0f);
    //outputColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //outputColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}


