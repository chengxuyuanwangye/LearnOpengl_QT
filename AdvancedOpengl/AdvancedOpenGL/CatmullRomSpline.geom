
#version 330 core
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 100) out;
in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;


vec2 GetCatmullRomPosition(float t, vec2 p0, vec2 p1, vec2 p2, vec2 p3)
{
    vec2 a = 2 * p1;
    vec2 b = p2 - p0;
    vec2 c = 2 * p0 - 5 * p1 + 4 * p2 - p3;
    vec2 d = -p0 + 3 * p1 - 3 * p2 + p3;

    vec2 pos = 0.5f * (a + (b * t) + (c * t * t) + (d * t * t * t));

    return pos;
}

void GetCatmullRomSplinePos(vec2 p0, vec2 p1, vec2 p2, vec2 p3)
{
    float resolution = 0.01f;
    int loops = 100; //loops=1/resolution
    for (int i = 0; i < loops; i++)//<=loops
    {
        vec2 temppos = GetCatmullRomPosition(i * resolution, p0, p1, p2, p3);
        gl_Position= vec4(temppos.x, temppos.y, 0.0, 1.0);
        EmitVertex();
    }

    EndPrimitive();
}


void main() {

 fColor = gs_in[0].color;
 vec4 tempp0=gl_in[0].gl_Position;
 vec4 tempp1=gl_in[1].gl_Position;
 vec4 tempp2=gl_in[2].gl_Position;
 vec4 tempp3=gl_in[3].gl_Position;

GetCatmullRomSplinePos(vec2(tempp0.x,tempp0.y),vec2(tempp1.x,tempp1.y),vec2(tempp2.x,tempp2.y), vec2(tempp3.x,tempp3.y));


}



