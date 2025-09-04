#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cmath>
#include "glm/gtc/random.hpp"
#include "glm/glm.hpp"

// SI constants
namespace SI {
    const double G = 6.674e-11;
    const double C = 299792458.0;
    const double sunMass = 1.98847e30;
    const double AU = 1.495978707e11;
};  // namespace SI

// world scale
namespace World {
    const double METERS_PER_UNIT = 1e9;
    inline double toWorld(double meters) { return meters / METERS_PER_UNIT; }
    inline double toMeters(double units) { return units * METERS_PER_UNIT; }
    const double G_WORLD = SI::G / (std::pow(World::METERS_PER_UNIT, 3));
    const double C_WORLD = SI::C / World::METERS_PER_UNIT;
};  // namespace World

// simulation funsies
namespace Funsies {
    const int NUM_LIGHT_RAYS = 5000;
};  // namespace Funsies

namespace LightConfig {
    inline glm::vec3 getLeftLightStartPos() {
        return glm::linearRand(glm::vec3(-100.0f, -75.0f, -10.0f),
                               glm::vec3(-75.0f, 750.0f, 10.0f));
    }

    inline glm::vec3 getOriginLightStartPos() {
        return glm::linearRand(glm::vec3(-50.0f), glm::vec3(50.0f));
    }

    inline glm::vec3 getLeftLightStartDir() {
        return glm::normalize(
            glm::linearRand(glm::vec3(-1.0, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    }

    inline glm::vec3 getOriginLightStartDir() {
        return glm::normalize(glm::linearRand(glm::vec3(-5.0f), glm::vec3(5.0f)));
    }
}  // namespace LightConfig

namespace BlackholeConfig {
    // const int BLACK_HOLE_RADIUS = World::toMeters(2);    // meters
    const double BLACK_HOLE_RADIUS = World::toWorld(World::METERS_PER_UNIT * 10);  // meters
    const float BLACK_HOLE_MASS = SI::sunMass * 100000000;                               // kg
    const glm::vec3 BLACK_HOLE_ORIGIN_POS = glm::vec3(0.0f);
    const glm::vec3 BLACK_HOLE_LEFT_POS = glm::vec3(10.0f, 0.0f, 0.0f);
}  // namespace BlackholeConfig

namespace CameraConfig {
    const float FAR_DIST = 1000.0f;
    const float CLOSE_DIST = 0.1f;
}

#endif