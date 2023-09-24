#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Shaders/ShaderClass.h"
#include "stb-master/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "../ClassesOpengl/Mateial/Material.h"



#define MAX_BONE_INFLUENCE 4
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class Mesh
{
public:

    // mesh data
    Material mat;
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material& mat);
    void Draw(ShaderClass& shader, int mIndex);
private:
    //  render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();

};

