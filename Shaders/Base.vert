#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 TextCoord;

out vec3 ourColor;
out vec2 TrueTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position=projection * view * model *vec4(position,1.0);
  ourColor= colour;
  TrueTextCoord= vec2(TextCoord.x, TextCoord.y);

}