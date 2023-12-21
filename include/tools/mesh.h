#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#inlcude <tools/shader.h>

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture{
    unsigned int id;
    string type;
    string path;
};

class Mesh{
    public:
    // 网格数据
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
    // 函数
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures -> textures;

            setupMesh();
        }
        void Draw(Shader shader);
    private:
    // 渲染数据
        unsigned int VAO, VBO, EBO;
    // 函数
        void setupMesh()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        }
};

#endif