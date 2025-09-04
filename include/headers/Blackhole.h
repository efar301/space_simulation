#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "Mesh.h"
#include "VBO.h"
#include "Constants.h"

void generateVertices(float radius, std::vector<Vertex>& outputVertices, std::vector<GLuint>& outputIndices, const glm::vec3& color = glm::vec3(1.0f));

struct Blackhole {

    glm::vec3 position;
    float mass;
    float radius;

    Mesh mesh;

    void draw(class Shader& shader, class Camera& camera);
    Blackhole(glm::vec3 position, float mass, float radius, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
};


#endif