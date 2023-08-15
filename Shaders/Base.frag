#version 330 core
out vec4 FragColor;
//ourColor is not implemented in order to control the color value from editor (i could also with this but is easier with an uniform)
in vec3 ourColor;
in vec2 TrueTextCoord;
flat in int instanceID;



in vec3 Normal;  
in vec3 FragPos;

struct Material {
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 



struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];



struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};



uniform sampler2D containerText;
uniform sampler2D specularMap;

uniform vec3 Color;
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;


  
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result= CalculateDirLight(dirLight, norm, viewDir);


    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);    


	FragColor =vec4(result, 1.0);


}



vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir){

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TrueTextCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TrueTextCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TrueTextCoord));
    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TrueTextCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TrueTextCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TrueTextCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}

