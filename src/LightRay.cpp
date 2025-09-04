#include "LightRay.h"
#include "Blackhole.h"
#include "shaderClass.h"
#include "Constants.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <random>
#include <algorithm>
#include <array>

LightRay::LightRay(glm::vec3& position, glm::vec3& direction, float speed,
                   std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    : position(position), direction(direction), speed(speed), mesh(vertices, indices) {}

    void
    LightRay::draw(class Shader& shader, class Camera& camera) {
    shader.Activate();
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));
    mesh.Draw(shader, camera);
}

void LightRay::step(const struct Blackhole& bh, float dt, float lensingStrength) {
    if (glm::distance(bh.position, position) <= bh.radius) {
        position = LightConfig::getOriginLightStartPos();
        direction = LightConfig::getOriginLightStartDir();

    } else {
        glm::vec3 distToBlackhole = bh.position - position;
        float r2 = glm::dot(distToBlackhole, distToBlackhole);
        float r = std::sqrt(std::max(r2, 1e-8f));
        float denom = std::max(std::pow(r, 3), static_cast<double>(1e-6f));
        glm::vec3 a = static_cast<float>(World::G_WORLD * bh.mass) * distToBlackhole / denom;

        glm::vec3 v = direction * speed;
        v += a * dt;

        if (glm::length2(v) > 0.0f) {
            direction = glm::normalize(v);
        }

        position += direction * speed * dt;
    }
}