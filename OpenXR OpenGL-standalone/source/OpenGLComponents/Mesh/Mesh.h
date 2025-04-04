#pragma once
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    Mesh();

    void Bind() const;

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
    void ComputeTangentBasis();
};