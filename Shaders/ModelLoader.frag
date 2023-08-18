#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;





struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};




uniform DirLight dirLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir);


void main()
{    

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result= CalculateDirLight(dirLight, norm, viewDir);

    FragColor = vec4(result , 1.0f);
}



vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDir){

    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 255.0f);
    // combine results
    vec3 ambient  = light.ambient  * vec3(0.5f, 0.5f, 0.5f);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}