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
uniform int heighmap;

uniform Material materials[NR_MATERIAL_SIZE];
uniform DirLight dirLight;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;


uniform float height_scale;


vec2 ParallaxMapping(vec2 textCords, vec3 viewDir);
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material mat,vec2 textCords);
vec3 calculateNormal(vec3 normal,vec2 textCords);
vec3 calculateSpecular(vec2 textCords);

void main()
{    
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 myCoords= ParallaxMapping(TexCoords, viewDir);
    if(myCoords.x > 1.0 || myCoords.y > 1.0 || myCoords.x < 0.0 || myCoords.y < 0.0)
    discard;

    vec3 norm = calculateNormal(Normal,myCoords);

    vec3 result= CalculateDirLight(dirLight, norm, viewDir, materials[meshCount],myCoords);
    vec3 resultD= texture(texture_height1, TexCoords).rgb ;
    FragColor = vec4(result, 1.0f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
  

}


vec3 calculateSpecular(vec2 textCords){
    if(specular==1){
        vec3 specularMap = texture(texture_specular1, textCords).rgb;
        return specularMap;
    }else{
        return vec3 (.5f);
	}


}

vec3 calculateNormal(vec3 normal,vec2 textCords){
	if(calculateNormals == 1){
	    vec3 normalMap =texture(texture_normal1, textCords).rgb;
        normalMap = normalMap * 2.0 - 1.0;
        return normalMap;
	}
	else{
		vec3 normalMap= normalize(Normal);
        return normalMap;
	}
}

vec2 ParallaxMapping(vec2 textCords, vec3 viewDir){

    if  (heighmap==1){
        const float minLayers = 8.0;
        const float maxLayers = 32.0;
        float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
        float layerDepth=1.0 / numLayers;

        float currentLayerDepth=0.0;

        vec2 p= viewDir.xy * height_scale;
        vec2 deltaTextCoords=p/numLayers;

        vec2 currentTexCoords=textCords;
        float currentDephtMapValue=texture(texture_height1,currentTexCoords).r;

        while (currentLayerDepth < currentDephtMapValue)
        {
        currentTexCoords-=deltaTextCoords;

        currentDephtMapValue=texture(texture_height1,currentTexCoords).r;
        currentLayerDepth+=layerDepth;

        }
        vec2 prevTexCoords = currentTexCoords + deltaTextCoords;
        float afterDepth  = currentDephtMapValue - currentLayerDepth;
        float beforeDepth = texture(texture_height1, prevTexCoords).r - currentLayerDepth + layerDepth;

        float weight = afterDepth / (afterDepth - beforeDepth);
        vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

        return finalTexCoords;
    }else{  
        return textCords;
	}
}

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material mat,vec2 textCords){

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
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, textCords));
    vec3 specularMap= calculateSpecular(textCords);
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

