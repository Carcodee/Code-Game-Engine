#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 TextCoord;
layout (location = 3) in vec3 aNormal;
//layout (location = 4) in mat4 instanceMatrix;

out vec3 ourColor;
out vec2 TrueTextCoord;
flat out int instanceID;

out vec3 Normal;
out vec3 FragPos;
uniform mat4 model[100];
uniform mat4 view;
uniform mat4 projection;



void main()
{
  
  Normal = aNormal;
  FragPos = vec3(model[gl_InstanceID] *vec4(position, 1.0));
  Normal = mat3(transpose(inverse(model[gl_InstanceID]))) * aNormal; 
  int glID=int(gl_InstanceID);
  instanceID=glID;
  gl_Position=projection * view * model[gl_InstanceID] *vec4(position,1.0);
  ourColor= colour;
  TrueTextCoord= vec2(TextCoord.x, TextCoord.y);

}