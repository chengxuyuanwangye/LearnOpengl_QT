#version 330 core
in vec3 vcolor;
out vec4 FragColor;

void main(){
    FragColor = vec4(vcolor.r, vcolor.g, vcolor.b, 1.0f);
}
