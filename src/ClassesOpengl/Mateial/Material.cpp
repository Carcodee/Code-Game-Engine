#include "Material.h"


Material::Material()
{
    std::cout << "Material created" << std::endl;
    pbrMaterial.albedo = glm::vec3(1.0f, 0.0f, 0.0f);
    pbrMaterial.albedoIntensity = 1.0f;
    pbrMaterial.metallic = 1.0f;
    pbrMaterial.roughness = 1.0f;
    pbrMaterial.ao = 1.0f;

    noPBRMaterial.difuse = glm::vec3(0.5f, 0.0f, 0.0f);
    noPBRMaterial.specular = glm::vec3(0.5f, 0.0f, 0.0f);
    noPBRMaterial.ambient = glm::vec3(0.5f, 0.0f, 0.0f);
    noPBRMaterial.shininess = 32.0f;

    isPBR = true;
    UseTexture = false;
}

void Material::setConfigurations(bool isPBR, bool useTextures)
{
    this->isPBR = isPBR;
    this->UseTexture = useTextures;
}

void Material::SetTexture(std::vector<Texture>& texture)
{
	this->textures= texture;

}


void Material::SetMaterial(ShaderClass& shader, int& mIndex)
{
    if (UseTexture)
    {
        ConfigurateTextures(shader, mIndex);
    }
    else
    {
        SetDefaultMaterial(shader, mIndex);
    }
}

void Material::SetMaterialColor(glm::vec3 color)
{
	pbrMaterial.albedo = color;
}

void Material::SetMaterialProperties(float albedoIntensity, float roughness, float metallic, float ao)
{
    pbrMaterial.albedoIntensity = albedoIntensity;
	pbrMaterial.metallic = metallic;
	pbrMaterial.roughness = roughness;
	pbrMaterial.ao = ao;
}

std::vector<Texture>& Material::GetTextures()
{
    return textures;
}



void Material::ConfigurateTextures(ShaderClass& shader, int& mIndex)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    unsigned int roguhtnessNr = 1;
    unsigned int metallicNr = 1;
    unsigned int aoNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE1 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string
        else if (name == "texture_metallic")
            number = std::to_string(metallicNr++); // transfer unsigned int to string
        else if (name == "texture_ao")
            number = std::to_string(aoNr++); // transfer unsigned int to string
        else if (name == "texture_roughness")
            number = std::to_string(roguhtnessNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i+1);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    int heightmapping = (heightNr > 1) ? 1 : 0;
    int normalMapping = (normalNr > 1) ? 1 : 0;
    //Albedo or Diffuse
    int diffuseMapping = (diffuseNr > 1) ? 1 : 0;

    //PBR
    int roughtnessmapping = (roguhtnessNr > 1) ? 1 : 0;
    int metallicmapping = (metallicNr > 1) ? 1 : 0;
    int aomapping = (aoNr > 1) ? 1 : 0;

    //Non PBR
    int specularMapping = (specularNr > 1) ? 1 : 0;
    shader.use();
    shader.setInt("texturesOn", 1);
    shader.setInt("meshCount", mIndex);
    shader.setInt("heighmap", heightmapping);
    shader.setInt("normalMapping", normalMapping);
    shader.setInt("diffuseMapping", diffuseMapping);

    if (isPBR)
    {
        shader.setInt("PBRon", 1);
        shader.setInt("roughnessMap", roughtnessmapping);
        shader.setInt("metallicMap", metallicmapping);
        shader.setInt("aoMap", aomapping);
        shader.setFloat("albedoR", pbrMaterial.albedo.x);
        shader.setFloat("albedoG", pbrMaterial.albedo.y);
        shader.setFloat("albedoB", pbrMaterial.albedo.z);

        shader.setFloat("albedoM", pbrMaterial.albedoIntensity);
        shader.setFloat("metallicM", pbrMaterial.metallic);
        shader.setFloat("roughnessM", pbrMaterial.roughness);
        shader.setFloat("aoM", pbrMaterial.ao);

    }else 
    {
        shader.setInt("PBRon", 0);
        shader.setInt("specularMapping", specularMapping);
        shader.setVec3("materials[" + std::to_string(mIndex) + "].diffuse", noPBRMaterial.difuse);
        shader.setVec3("materials[" + std::to_string(mIndex) + "].specular", noPBRMaterial.specular);
        shader.setVec3("materials[" + std::to_string(mIndex) + "].ambient", noPBRMaterial.ambient);
        shader.setFloat("materials[" + std::to_string(mIndex) + "].shininess", noPBRMaterial.shininess);
    }



}

void Material::SetDefaultMaterial(ShaderClass& shader, int mIndex)
{

    shader.use();
    shader.setInt("texturesOn", 0);
    shader.setInt("PBRon", 1);

    shader.setInt("meshCount", mIndex);
    shader.setInt("diffuseMapping", 0);
    shader.setInt("normalMapping", 0);
    shader.setInt("specular", 0);
    shader.setInt("heighmap", 0);
    shader.setInt("roughnessMap", 0);
    shader.setInt("metallicMap", 0);
    shader.setInt("aoMap", 0);
    shader.setFloat("albedoR", pbrMaterial.albedo.x);
    shader.setFloat("albedoG", pbrMaterial.albedo.y);
    shader.setFloat("albedoB", pbrMaterial.albedo.z);
    shader.setFloat("metallicM", pbrMaterial.metallic);
    shader.setFloat("roughnessM", pbrMaterial.roughness);
    shader.setFloat("aoM", pbrMaterial.ao);

}


