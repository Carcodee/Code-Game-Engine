#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 TangentLightPos;

flat in int calculateNormals;

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
uniform int specular;

uniform Material materials[NR_MATERIAL_SIZE];
uniform DirLight dirLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;


vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material mat);
vec3 calculateNormal(vec3 normal);
vec3 calculateSpecular();

void main()
{    

    vec3 norm = calculateNormal(Normal);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    vec3 result= CalculateDirLight(dirLight, norm, viewDir, materials[meshCount]);
    FragColor = vec4(result, 1.0f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
  

}


vec3 calculateSpecular(){
    if(specular==1){
        vec3 specularMap = texture(texture_specular1, TexCoords).rgb;
        return specularMap;
    }else{
        return vec3(.5f);
	}


}

vec3 calculateNormal(vec3 normal){
	if(calculateNormals == 1){
	    vec3 normalMap =texture(texture_normal1, TexCoords).rgb;
        normalMap = normalMap * 2.0 - 1.0;
        return normalMap;
	}
	else{
		vec3 normalMap= normalize(Normal);
        return normalMap;
	}
}

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material mat){

    light.direction = TangentLightPos;
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
    vec3 specularMap= calculateSpecular();
    vec3 specular = light.specular * spec * specularMap;
    return (ambient + diffuse + specular);
    }
    if(texturesOn==0){
    vec3 ambient  = light.ambient  * vec3(0.2f);
    vec3 diffuse  = light.diffuse  * diff * vec3(mat.diffuse);
    vec3 specular = light.specular * spec * vec3(mat.specular);
    return (ambient + diffuse + specular);
    }

}