#version 330 core
out vec4 FragColor;

//in vec3 Normal;
//in vec3 Position;
in VS_OUT
{
    in vec3 Normal;
    in vec3 Position;
} fs_in;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
  //  float ratio = 1.00 / 1.52;
  //  vec3 I = normalize(fs_in.Position - cameraPos);
 //   vec3 R = refract(I, normalize(fs_in.Normal), ratio);
 //   FragColor = vec4(texture(skybox, R).rgb, 1.0);
 FragColor = vec4(0.0, 1.0, 0.0, 1.0);

  /*  if(gl_FragCoord.x < 400)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);*/
}
