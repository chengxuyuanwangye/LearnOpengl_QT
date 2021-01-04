#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 objectcolor;
uniform vec3 lightcolor;

void main(){
    FragColor = vec4(lightcolor*objectcolor,1.0);
}
