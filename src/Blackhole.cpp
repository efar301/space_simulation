#include "Blackhole.h"
#include <cmath>

void generateVertices(float radius, std::vector<Vertex>& outputVertices,
                      std::vector<GLuint>& outputIndices, const glm::vec3& color) {
    outputVertices.clear();
    outputIndices.clear();

    int stacks = 20;
    int sectors = 20;

    for (int i = 0; i <= stacks; ++i) {
        float v = static_cast<float>(i) / stacks;
        float theta = v * M_PI;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (int j = 0; j <= sectors; ++j) {
            float u = static_cast<float>(j) / sectors;
            float phi = u * 2 * M_PI;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            glm::vec3 normal(sinTheta * cosPhi,   // x
                             cosTheta,            // y
                             sinTheta * sinPhi);  // z
            glm::vec3 position = radius * normal;
            glm::vec2 uv(u, 1.0f - v);

            outputVertices.push_back(Vertex{position, color, glm::normalize(normal), uv});
        }
    }

    const int stride = sectors + 1;
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * stride;
        int k2 = k1 + stride;

        for (int j = 0; j < sectors; ++j) {
            if (i != 0) {
                outputIndices.push_back(k1 + j);
                outputIndices.push_back(k2 + j);
                outputIndices.push_back(k1 + j + 1);
            }
            if (i != stacks - 1) {
                outputIndices.push_back(k1 + j + 1);
                outputIndices.push_back(k2 + j);
                outputIndices.push_back(k2 + j + 1);
            }
        }
    }
}

void Blackhole::draw(class Shader& shader, class Camera& camera) {
    shader.Activate();
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));
    mesh.Draw(shader, camera);
}

Blackhole::Blackhole(glm::vec3 position, float mass, float radius, std::vector<Vertex>& vertices,
                     std::vector<GLuint>& indices)
    : position(position), mass(mass), radius(radius), mesh(vertices, indices) {}