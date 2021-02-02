#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

//out VS_OUT {
 //   vec3 color;
//} vs_out;

out vec3  fColor;
void main()
{
   // vs_out.color = aColor;
    fColor=aColor;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_PointSize = 10.0;

}

