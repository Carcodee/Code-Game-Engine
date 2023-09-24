#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 TangentLightPos;
flat in int PBRon;
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
uniform int diffuseMapping;
uniform int roughnessMap;
uniform int metallicMap;
uniform int aoMap;

uniform float albedoR;
uniform float albedoG;
uniform float albedoB;

uniform float roughnessM;
uniform float metallicM;
uniform float aoM;

uniform Material materials[NR_MATERIAL_SIZE];
uniform DirLight dirLight;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_metallic1;
uniform sampler2D texture_ao1;



const float PI = 3.14159265359;
uniform float height_scale;

//-------------------------------------
//NON-PBR
vec2 ParallaxMapping(vec2 textCords, vec3 viewDir);
vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir, Material mat,vec2 textCords);
vec3 calculateNormal(vec3 normal,vec2 textCords);
vec3 calculateSpecular(vec2 textCords);
vec3 setNormalLight();
//-------------------------------------
//PBR
vec3 CalculatePBR();
vec3 CalculatePBRNoTextures();
vec3 getNormalFromMapNoTexture();
vec3 getNormalFromMap();
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
//-------------------------------------

void main()
{    
    vec3 result;

	if(texturesOn==1){
        if(PBRon==1)result=CalculatePBR();
        if(PBRon==0)result=CalculateDirLight(dirLight, Normal, TangentViewPos, materials[meshCount],TexCoords);
    }else{
		result=CalculatePBRNoTextures();
	}
    FragColor = vec4(result, 1.0f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));


}

vec3 CalculatePBR(){
    
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;  

    if(diffuseMapping==1){
     albedo= pow(texture(texture_diffuse1, TexCoords).rgb, vec3(2.2));
    }else{
    albedo=vec3(albedoR,albedoG,albedoB);
    }
    if(metallicMap==1){
    metallic= texture(texture_metallic1, TexCoords).r * metallicM;
    }else{
    metallic=metallicM;
    }
    if(roughnessMap==1){
    roughness= texture(texture_roughness1, TexCoords).r * roughnessM;
    }else
    {
    roughness=roughnessM;
    }
    if(aoMap==1){
    ao= texture(texture_ao1, TexCoords).r*aoM;
    }else
    {
    ao=aoM;
    }

    vec3 N = getNormalFromMap();
    vec3 V = normalize(TangentViewPos - FragPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(dirLight.direction - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(dirLight.direction - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = dirLight.diffuse * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    return color;

}

vec3 CalculatePBRNoTextures(){
    
   vec3 albedo=vec3(albedoR,albedoG,albedoB);
   float metallic=metallicM;
   float roughness=roughnessM;
   float ao=aoM;

    vec3 N = getNormalFromMapNoTexture();
    vec3 V = normalize(TangentViewPos - FragPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

        // calculate per-light radiance
        vec3 L = normalize(TangentLightPos- FragPos);
        vec3 H = normalize(V + L);
        float distance = length(TangentLightPos - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = dirLight.diffuse * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
 
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    return color;

}

vec3 setNormalLight(){

    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 myCoords= ParallaxMapping(TexCoords, viewDir);
    if(myCoords.x > 1.0 || myCoords.y > 1.0 || myCoords.x < 0.0 || myCoords.y < 0.0)
    discard;

    vec3 norm = calculateNormal(Normal,myCoords);

    vec3 result= CalculateDirLight(dirLight, norm, viewDir, materials[meshCount],myCoords);
    vec3 resultD= texture(texture_height1, TexCoords).rgb ;
    return result;

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
    float spec = pow(max(dot(viewDir, reflectDir), 0.1), mat.shininess);
    // combine results
    if(texturesOn==1){
    vec3 ambient  = light.ambient  * vec3(0.2f);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, textCords));
    vec3 specularMap= calculateSpecular(textCords);
    vec3 specular = light.specular * spec * specularMap * metallicM;
    return (ambient + diffuse + specular);
    }
    if(texturesOn==0){
    vec3 ambient  = light.ambient  * vec3(0.2f);
    vec3 diffuse  = light.diffuse  * diff * vec3(mat.diffuse);
    vec3 specular = light.specular * spec * vec3(mat.specular)* metallicM;
    return (ambient + diffuse + specular);
    }

}


//-------------------------------------PBR-------------------------------------
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(texture_normal1, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

//-------------------------------------PBR-------------------------------------
vec3 getNormalFromMapNoTexture()
{
    vec3 tangentNormal =vec3(0,1,0) * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
