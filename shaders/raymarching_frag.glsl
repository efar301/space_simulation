#version 400 core
in vec2 vNDC;
out vec4 FragColor;

// camera uniforms
uniform vec3 camPos;
uniform vec3 camFwd;
uniform vec3 camRight;
uniform vec3 camUp;
uniform float fovY; // degrees
uniform float aspect; // width/height

// black hole uniforms
uniform float bhRadius;
uniform vec3 bhPos;

// controls
uniform int maxSteps;
uniform float maxDist;
uniform float surfEps;

vec3 getRayDirection() {
    float tanFovY = tan(radians(fovY) * 0.5f);
    vec3 rayDirView = normalize(vec3(vNDC.x * aspect * tanFovY, vNDC.y * tanFovY, -1.0));

    vec3 fwd = normalize(camFwd);
    vec3 right = normalize(camRight);
    vec3 up = normalize(camUp);
    mat3 cameraToWorld = mat3(right, up, -fwd);

    vec3 rayDirection = normalize(cameraToWorld * rayDirView);
    return rayDirection;
}

float distFromBH(in vec3 pointPos, in vec3 bhPos, in float bhRadius) {
    return length(pointPos - bhPos) - bhRadius;
}

float mapTheWorld(in vec3 position) {
    float bh = distFromBH(position, bhPos, bhRadius);

    return bh;
}

vec3 calculateNormal(in vec3 position) {
    const vec3 smallStep = vec3(0.001f, 0.0f, 0.0f);

    float gradientX = mapTheWorld(position + smallStep.xyy) - mapTheWorld(position - smallStep.xyy);
    float gradientY = mapTheWorld(position + smallStep.yxy) - mapTheWorld(position - smallStep.yxy);
    float gradientZ = mapTheWorld(position + smallStep.yyx) - mapTheWorld(position - smallStep.yyx);

    vec3 normal = vec3(gradientX, gradientY, gradientZ);
    return normalize(normal);
}

// vec3 rayMarch(in vec3 rayOrigin, in vec3 rayDirection) {

//     float totalDistanceTraveled = 0.0f;

//     for (int i = 0; i < maxSteps; ++i) {
//         vec3 currentPosition = rayOrigin + totalDistanceTraveled * rayDirection;

//         float distanceToClosest = distFromBH(currentPosition, bhPos, bhRadius);

//         if (distanceToClosest < surfEps) {
//             vec3 normal = calculateNormal(currentPosition);
//             vec3 light_position = vec3(200.0, 0.0, -200.0);
//             vec3 direction_to_light = normalize(currentPosition - light_position);
//             float diffuse_intensity = max(0.0, dot(normal, direction_to_light));

//             return vec3(1.0f, 0.0f, 0.0f) * diffuse_intensity;
//             // return normal;
//         }
//         if (totalDistanceTraveled > maxDist) {
//             break;
//         }
//         totalDistanceTraveled += distanceToClosest;
//     }
//     return vec3(0.0f); // black
// }


vec3 rayMarch(in vec3 rayOrigin, in vec3 rayDirection) {

    float totalDistanceTraveled = 0.0f;

    for (int i = 0; i < maxSteps; ++i) {
        vec3 currentPosition = rayOrigin + totalDistanceTraveled * rayDirection;

        float distanceToClosest = distFromBH(currentPosition, bhPos, bhRadius);

        if (distanceToClosest < surfEps) {
            vec3 normal = calculateNormal(currentPosition);
            vec3 light_position = vec3(200.0, 0.0, -200.0);
            vec3 direction_to_light = normalize(currentPosition - light_position);
            float diffuse_intensity = max(0.0, dot(normal, direction_to_light));

            return vec3(1.0f, 0.0f, 0.0f) * diffuse_intensity;
            // return normal;
        }
        if (totalDistanceTraveled > maxDist) {
            break;
        }
        totalDistanceTraveled += distanceToClosest;
    }
    return vec3(0.0f); // black
}

void main() {
    // step size for ray
    const float stepSize = 0.1f;

    vec3 rayOrigin = camPos;
    vec3 rayDirection = getRayDirection();

    vec3 outputColor = rayMarch(camPos, rayDirection);

    FragColor = vec4(outputColor, 1.0f);
}