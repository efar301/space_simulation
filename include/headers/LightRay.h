#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include <glm/glm.hpp>
#include "Mesh.h"
#include "Blackhole.h"

struct LightRay {
    glm::vec3 position;
    glm::vec3 direction; //normalized
    float speed;
    Mesh mesh;

    void step(const struct Blackhole& bh, float dt, float lensingStrength = 1.0f);
    void draw(class Shader& shader, class Camera& camera);
    void drawTrail(class Shader& shader, class Camera& camera);

    LightRay(glm::vec3& position, glm::vec3& direction, float speed, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
};

#endif