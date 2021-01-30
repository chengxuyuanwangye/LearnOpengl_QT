#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//out vec3 Normal;
//out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out VS_OUT
{
    out vec3 Normal;
    out vec3 Position;
} vs_out;

void main()
{
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
