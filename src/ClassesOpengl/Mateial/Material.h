#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../Shaders/Classes/ShaderClass.h"
#include "../CodeObject/ComponentBase/ComponentBase.h"

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct PBRMaterial {
	glm::vec3 albedo;
	float albedoIntensity;
	float metallic;
	float roughness;
	float ao;
};
struct NoPRBMaterial {
	
	glm::vec3 difuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;

};
//changeable, into a proper class
class Material: public ComponentBase
{

public:

	Material();

	Material& operator=(const Material& other) {
		this->pbrMaterial = other.pbrMaterial;
		this->noPBRMaterial = other.noPBRMaterial;
		this->textures = other.textures;
		this->isPBR = other.isPBR;
		this->UseTexture = other.UseTexture;
		return *this;
	};

	//TODO: add move constructor
	Material(const Material& other) {
		this->pbrMaterial = other.pbrMaterial;
		this->noPBRMaterial = other.noPBRMaterial;
		this->textures = other.textures;
		this->isPBR = other.isPBR;
		this->UseTexture = other.UseTexture;
	}
	//to do
	~Material() {
	
	};

	void setConfigurations(bool isPBR, bool useTextures);
	void SetTexture(std::vector <Texture>& texture);
	void ConfigurateTextures(ShaderClass&, int& mIndex);
	void SetDefaultMaterial(ShaderClass& shader, int mIndex);
	void SetMaterial(ShaderClass& shader,int& mIndex);
	void SetMaterialColor(glm::vec3 color);
	void SetMaterialProperties(float albedoIntensity, float roughness, float metallic, float ao);
	std::vector <Texture>& GetTextures();
	bool isPBR;

private:
	

	PBRMaterial pbrMaterial;
	NoPRBMaterial noPBRMaterial;
	std::vector<Texture> textures;

	bool UseTexture;
	



};

