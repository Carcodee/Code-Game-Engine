#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;





struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
	vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
}; 
uniform int matSize;
uniform int meshCount;
#define NR_MATERIAL_SIZE 5

uniform int texturesOn;
uniform Material materials[NR_MATERIAL_SIZE];
uniform DirLight dirLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material mat);


void main()
{    

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result= CalculateDirLight(dirLight, norm, viewDir, materials[meshCount]);
    FragColor = vec4(result, 1.0f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
  

}



vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material mat){


    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.1);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.1), 225.0f);
    // combine results
    if(texturesOn==1){
    vec3 ambient  = light.ambient  * vec3(0.2f);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
    }
    if(texturesOn==0){
    vec3 ambient  = light.ambient  * vec3(0.2f);
    vec3 diffuse  = light.diffuse  * diff * vec3(mat.diffuse);
    vec3 specular = light.specular * spec * vec3(mat.specular);
    return (ambient + diffuse + specular);
    }

}