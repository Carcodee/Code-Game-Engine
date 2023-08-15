#version 330 core
out vec4 FragColor;



in vec3 FragPos;


struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



uniform vec3 objectColor;
uniform Light light;


  

void main()
{
    
	FragColor =vec4(objectColor * light.diffuse, 1.0f);
}