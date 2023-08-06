#version 330 core
out vec4 FragColor;
//ourColor is not implemented in order to control the color value from editor (i could also with this but is easier with an uniform)
in vec3 ourColor;
in vec2 TrueTextCoord;
flat in int instanceID;



in vec3 Normal;  
in vec3 FragPos;


struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 Color;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material[100];
uniform Light light; 


  

void main()
{

	vec3 ambient = light.ambient;


	//diffuse calculation
	//normalize normal vector
	vec3 norm = normalize(Normal);
	//we got the direction of the shader and the light
	vec3 lightDir = normalize(lightPos - FragPos);
	//the result of the dor vector of the light and the normal will determine how the light will affect
	float diff = max(dot(norm, lightDir), 0.1); 
	//multiply diff(a value between -1,1) with the light
	vec3 diffuse =  light.diffuse * (diff * material[instanceID].diffuse);
	
	//Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material[instanceID].shininess);
	vec3 specular = light.specular * (spec * material[instanceID].specular); 


	vec3 result = ambient + diffuse + specular;
	FragColor = mix(texture(texture1, TrueTextCoord), texture(texture2, TrueTextCoord), 0.5) * vec4(result, 1.0);
}