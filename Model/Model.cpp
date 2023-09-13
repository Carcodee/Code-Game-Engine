#include "Model.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)

Model::Model()
{
    isLoaded = false;
    gammaCorrection = false;
}

void Model::Draw(ShaderClass& shader)
{
    if (!isLoaded)
    {
        if (myFuture.valid())
        {
            const aiScene* scene = myFuture.get();
            processNode(scene->mRootNode, scene);
            isLoaded = true;
        }

        return;
    }
    else
    {
        shader.setInt("matSize", meshes.size());
        for (unsigned int i = 0; i < meshes.size(); i++) {

            meshes[i].Draw(shader, i);
        }
    }

}

void Model::loadModel(std::string path)
{

    import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));


    processNode(scene->mRootNode, scene);


}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);

    }


}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);

    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::string errorTexture = "errorTexture::UNABLE TO READ THE TEXTURE= ";


    if (!isPBR)
    {
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        (diffuseMaps.size() == 0) ? std::cout << errorTexture + GET_VARIABLE_NAME(diffuseMaps) << "\n" : std::cout << "diffuse ok" << "\n";
        //2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        (specularMaps.size() == 0) ? std::cout << errorTexture + GET_VARIABLE_NAME(specularMaps) << "\n" : std::cout << "specular ok" << "\n";
        //3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        (normalMaps.size() == 0) ? std::cout << errorTexture + GET_VARIABLE_NAME(normalMaps) << "\n" : std::cout << "normalmap ok" << "\n";
        //4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        (heightMaps.size() == 0) ? std::cout << errorTexture + GET_VARIABLE_NAME(heightMaps) << "\n" : std::cout << "heightmap ok" << "\n";
    }
    else
    {

        //1. roughtness maps
        LoadPBRTextures("texture_diffuse", textures);
        //2. Normal maps
        LoadPBRTextures("texture_normal", textures);
        //3. roughtness maps
        LoadPBRTextures("texture_roughness", textures);
        //4. metallic maps
        LoadPBRTextures("texture_metallic", textures);
        //5. ao maps
        LoadPBRTextures("texture_ao", textures);
    }

    //insertmaterials in fragment shader  with a method in the mesh
    Material materials = loadMaterial(material);

    //std::cout << materials.Diffuse.x<< " ," << materials.Diffuse.y << " ," << materials.Diffuse.z << "\n";

    return Mesh(vertices, indices, textures, materials);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory, false);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

void Model::LoadPBRTextures(std::string typeName,std::vector<Texture>& textures)
{
    Texture myPBRText;
    std::string prefixSize="texture_";
    std::string textName = typeName.substr(prefixSize.size());

    std::string myPath = "PBRTextures/" + textName + ".jpg";
    myPBRText.id = TextureFromFile(myPath.c_str(), this->directory, false);
    if (myPBRText.id==0)
    {
        std::cout << "Failed to load texture " + typeName << "\n";
    }
    myPBRText.type = typeName;
    myPBRText.path = path;
    textures.push_back(myPBRText);
    textures_loaded.push_back(myPBRText); // add to loaded textures
    std::cout << "PBR texture loaded: " + typeName << "\n";
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;


}

Material Model::loadMaterial(aiMaterial* mat)
{

    glm::vec3 randDiff((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
    Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(randDiff.r, randDiff.b, randDiff.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(randDiff.r, randDiff.b, randDiff.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.Shininess = shininess;

    return material;
}

std::future<const aiScene*> Model::loadSceneAsync(std::string path, std::string& directory, bool& isLoaded, std::mutex& mutex)
{


    auto sceneLoader = ([path, &directory, &isLoaded, &mutex, this]()->const aiScene* {
        std::lock_guard<std::mutex> lock(mutex);
        const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return nullptr;
        }
        directory = path.substr(0, path.find_last_of('/'));
        return scene;
        });

    std::future <const aiScene*> sceneFuture = std::async(std::launch::async,sceneLoader);

    return sceneFuture;
    //if (sceneFuture.valid())
    //{
    //    std::cout << "Done" << "\n";
    //    isLoaded = true;
    //    const aiScene* scene = import.GetOrphanedScene();
    //    this->processNode(scene->mRootNode, scene);
    //}


}



