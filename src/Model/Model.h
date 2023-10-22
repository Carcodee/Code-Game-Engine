#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Shaders/Classes/ShaderClass.h"
#include <iostream>
#include <vector>
#include "../src/Mesh/Mesh.h"
#include <future>
#include <mutex>
#include <memory>
#include "../ClassesOpengl/CodeObject/ComponentBase/ComponentBase.h"

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

class Model: public ComponentBase
{

public:
    Model();

    Model& operator=(const Model& other) {
        this->meshes = other.meshes;
		this->textures_loaded = other.textures_loaded;
		this->directory = other.directory;
		this->path = other.path;
		this->gammaCorrection = other.gammaCorrection;
		this->isPBR = other.isPBR;
		this->isLoaded = other.isLoaded;
        this->material = other.material;
        return *this;
    };
    Model(const Model& other)
    {
        this->meshes = other.meshes;
        this->directory = other.directory;
        this->textures_loaded = other.textures_loaded;
        this->isLoaded = other.isLoaded;
        this->gammaCorrection = other.gammaCorrection;
        this->path = other.path;
        this->isPBR = other.isPBR;
        this->material = other.material;

    }
    //TODO: add move constructor
    

    void StartModel(std::string const& path ,const bool& isPBR, std::shared_ptr<Material> material, bool gamma = false) {
       myFuture = loadSceneAsync(path,directory,isLoaded,modelMutex);
       this->isPBR=isPBR;
       this->path = path;
       this->gammaCorrection = gamma;
       this->material = material;
        //loadModel(path);

    }
    void Draw(ShaderClass& shader);
    bool isLoaded;
    void ExtractMaterials(const char* path);
    void SetMaterial(std::shared_ptr<Material> mat);

private:
    std::mutex modelMutex;
    // model data
    std::shared_ptr<Material> material;
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
    std::future<const aiScene*> loadSceneAsync(std::string path, std::string& directory, bool& isLoaded, std::mutex& mutex);
    void ExtractTextures(std::string typeName, std::vector<Texture>& textures, const char* path);

};
