#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Shaders/ShaderClass.h"
#include <iostream>
#include <vector>
#include "../Mesh/Mesh.h"
#include <future>
#include <mutex>
#include <memory>

struct ModelConfigs
{
    float posX, posY, posZ=0.0f;
    float rotX, rotY, rotZ=0.0;
    float scaleX, scaleY, scaleZ=1.0f;
    float albedo=1.0f;
    float roughness=1.0f;
    float metallic=1.0f;
    float ao=1.0f;
    bool isPBR=false;
    int id;
};

class Model
{

public:
    Model();
    Model(const Model& other);

    Model operator=(const Model& other) {
        this->meshes = other.meshes;
		this->textures_loaded = other.textures_loaded;
		this->directory = other.directory;
		this->path = other.path;
		this->gammaCorrection = other.gammaCorrection;
		this->isPBR = other.isPBR;
		this->isLoaded = other.isLoaded;
        return *this;
    };

    //TODO: add move constructor
    

    void StartModel(std::string const& path ,bool& isPBR,bool gamma = false) {
       myFuture = loadSceneAsync(path,directory,isLoaded,modelMutex);
       this->isPBR=isPBR;
       this->path = path;
        //loadModel(path);
    }
    void Draw(ShaderClass& shader);
    bool isLoaded;
    Material material;

private:
    std::mutex modelMutex;
    // model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    std::string path;
    bool gammaCorrection;
    bool isPBR;
    bool useTexture;
    std::future<const aiScene*> myFuture;
    Assimp::Importer import;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    void LoadPBRTextures(std::string typeName,std::vector<Texture>& texture);
    unsigned int TextureFromFile(const char* path, const std::string& directory,bool gamma);
    Material loadMaterial(aiMaterial* mat);
    std::future<const aiScene*> loadSceneAsync(std::string path, std::string& directory, bool& isLoaded, std::mutex& mutex);
    
};
