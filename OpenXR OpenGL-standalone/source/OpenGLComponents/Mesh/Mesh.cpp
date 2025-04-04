#include "Mesh.h"
#include <glad/glad.h>

//Constructor
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

    this->ComputeTangentBasis();
	this->SetupMesh();
}

//Constructor
Mesh::Mesh() {

}

//public
void Mesh::Bind() const {
    glBindVertexArray(this->VAO);
}

//private
void Mesh::SetupMesh() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int),
        &this->indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}

void Mesh::ComputeTangentBasis() {
    for (int i = 0; i < this->indices.size(); i+=3) {
        glm::vec3 v0 = this->vertices[this->indices[i + 0]].Position;
        glm::vec3 v1 = this->vertices[this->indices[i + 1]].Position;
        glm::vec3 v2 = this->vertices[this->indices[i + 2]].Position;

        glm::vec2 uv0 = this->vertices[this->indices[i + 0]].TexCoords;
        glm::vec2 uv1 = this->vertices[this->indices[i + 1]].TexCoords;
        glm::vec2 uv2 = this->vertices[this->indices[i + 2]].TexCoords;

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangent = glm::normalize(tangent);
        bitangent = glm::normalize(bitangent);

        for (int a = 0; a < 3; a++)
        {
            this->vertices[this->indices[i + a]].Tangent = tangent;
            this->vertices[this->indices[i + a]].Bitangent = bitangent;
        }
    }
}