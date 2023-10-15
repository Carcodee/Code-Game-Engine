#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    

    vec3 mySkybox = texture(skybox, TexCoords).rgb;
    
    mySkybox = mySkybox / (mySkybox + vec3(1.0));
    mySkybox = pow(mySkybox, vec3(1.0/2.2)); 
    FragColor = vec4(mySkybox, 1.0f);
}