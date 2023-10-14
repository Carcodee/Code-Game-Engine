#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent; 

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 noTangentMapViewPos;

out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 TangentLightPos;

flat out int calculateNormals;
flat out int PBR;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 mousePos;
uniform int normalMapping;
uniform int PBRon;

void main()
{
    
    Normal=aNormal;
    noTangentMapViewPos=viewPos;
    TexCoords = aTexCoords;    
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    
    calculateNormals = normalMapping;
    PBR = PBRon;
    calculateNormals=0;
    if(calculateNormals==1){
        vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
        vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
        vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
        mat3 TBN = mat3(T, B, N);
        TBN = transpose(mat3(T,B,N));
        TangentViewPos  = TBN * viewPos;
        TangentLightPos = TBN * lightPos;
        TangentFragPos  = TBN * vec3(model * vec4(aPos, 1.0));
    }else{
        TangentViewPos  = viewPos;
		TangentLightPos = lightPos;
		TangentFragPos  = vec3(model * vec4(aPos, 1.0));
    }

}