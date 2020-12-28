#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform  mat4 matrix;

out vec3 vcolor;
void main(){
    vec4 temp=vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    gl_Position = matrix*temp;
    vcolor=aColor;
}
