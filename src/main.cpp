#include <filesystem>
#include <random>
#include <glm/gtc/random.hpp>
namespace fs = std::filesystem;

#include "Constants.h"
#include "Mesh.h"
#include "Blackhole.h"
#include "LightRay.h"
#include "VAO.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
Vertex vertices[] = {  //               COORDINATES           /            COLORS          / NORMALS
                       //               /       TEXTURE COORDINATES    //
    Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
           glm::vec2(1.0f, 0.0f)}};

// Indices for vertices order
GLuint indices[] = {0, 1, 2, 0, 2, 3};

Vertex lightVertices[] = {  //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)}, Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)}, Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},  Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},  Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

std::vector<Vertex> lightVerts = {
    Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)}, Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)}, Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},  Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},  Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

GLuint lightIndices[] = {0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2,
                         2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

std::vector<GLuint> lightInds{0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2,
                              2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

std::vector<Vertex> bhVertices;
std::vector<GLuint> bhIndices;

std::vector<Vertex> rayTriangle = {
    {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(3.0f, -1.0f, 0.0f), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec2(2.0f, 0.0f)},
    {glm::vec3(-1.0f, 3.0f, 0.0f), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec2(0.0f, 2.0f)},
};

void updateRays(Camera& camera, Shader& lightRayShader, Blackhole& bh) {
    lightRayShader.Activate();
    // camera info
    glUniform1f(glGetUniformLocation(lightRayShader.ID, "fovY"), camera.fovY);
    glUniform3fv(glGetUniformLocation(lightRayShader.ID, "camPos"), 1,
                 glm::value_ptr(camera.Position));
    glUniform3fv(glGetUniformLocation(lightRayShader.ID, "camFwd"), 1,
                 glm::value_ptr(camera.Orientation));
    glUniform3fv(glGetUniformLocation(lightRayShader.ID, "camRight"), 1,
                 glm::value_ptr(glm::cross(camera.Orientation, camera.Up)));
    glUniform3fv(glGetUniformLocation(lightRayShader.ID, "camUp"), 1, glm::value_ptr(camera.Up));
    glUniform1f(glGetUniformLocation(lightRayShader.ID, "aspect"), float(width) / float(height));
    glUniform1i(glGetUniformLocation(lightRayShader.ID, "maxSteps"), 300);
    glUniform1f(glGetUniformLocation(lightRayShader.ID, "maxDist"), CameraConfig::FAR_DIST);
    glUniform1f(glGetUniformLocation(lightRayShader.ID, "surfEps"), 0.001f);

    // bh info
    glUniform1f(glGetUniformLocation(lightRayShader.ID, "bhRadius"), bh.radius);
    glUniform3fv(glGetUniformLocation(lightRayShader.ID, "bhPos"), 1, glm::value_ptr(bh.position));
}

int main() {
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
    GLFWwindow* window = glfwCreateWindow(width, height, "Engine", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    gladLoadGL();
    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800

    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();

    std::string shaderPath = "/shaders/";
    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram(
        (parentDir + shaderPath + "vert.glsl").c_str(),
        (parentDir + shaderPath + "frag.glsl").c_str());  // Store mesh data in vectors for the mesh
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    // std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    // Create floor mesh
    // Mesh floor(verts, ind, tex);
    Mesh floor(verts, ind);

    // Shader for light cube
    Shader lightShader((parentDir + shaderPath + "light_vert.glsl").c_str(),
                       (parentDir + shaderPath + "light_frag.glsl").c_str());
    // Store mesh data in vectors for the mesh
    std::vector<Vertex> lightVerts(lightVertices,
                                   lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector<GLuint> lightInd(lightIndices,
                                 lightIndices + sizeof(lightIndices) / sizeof(GLuint));
    // Create light mesh
    // Mesh light(lightVerts, lightInd, tex);

    VAO rayVAO;
    rayVAO.Bind();
    VBO rayVBO(rayTriangle);

    rayVAO.LinkAttrib(rayVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);  // aPos
    rayVAO.LinkAttrib(rayVBO, 1, 3, GL_FLOAT, sizeof(Vertex),
                      (void*)(3 * sizeof(float)));  // aNormal
    rayVAO.LinkAttrib(rayVBO, 2, 3, GL_FLOAT, sizeof(Vertex),
                      (void*)(6 * sizeof(float)));  // aColor
    rayVAO.LinkAttrib(rayVBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));  // aTex
    rayVAO.Unbind();
    rayVBO.Unbind();

    Shader lightRayShader((parentDir + shaderPath + "screen_vert.glsl").c_str(),
                          (parentDir + shaderPath + "raymarching_frag.glsl").c_str());

    Mesh light(lightVerts, lightInd);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);

    generateVertices(BlackholeConfig::BLACK_HOLE_RADIUS, bhVertices, bhIndices);
    Blackhole bh(BlackholeConfig::BLACK_HOLE_ORIGIN_POS, BlackholeConfig::BLACK_HOLE_MASS,
                 BlackholeConfig::BLACK_HOLE_RADIUS, bhVertices, bhIndices);

    std::vector<LightRay> lightRays;
    for (int i = 0; i < Funsies::NUM_LIGHT_RAYS; ++i) {
        glm::vec3 lrPosition = LightConfig::getOriginLightStartPos();
        glm::vec3 lrDirection = LightConfig::getOriginLightStartDir();

        LightRay ray(lrPosition, lrDirection, World::C_WORLD, lightVerts, lightInds);
        lightRays.push_back(ray);
    }

    // glm::vec3 lrPosition = glm::linearRand(glm::vec3(-5.0f), glm::vec3(5.0f));
    // glm::vec3 lrDirection = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));

    // LightRay lr(lrPosition, lrDirection, 0.5f, lightVerts, lightInds);

    // Enables the Depth Buffer
    // glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);

    // Creates camera object
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 200.0f), 45.0f);

    double lastTime = glfwGetTime();
    double fpsTimer = lastTime;
    int frameCount = 0;

    // Main while loop
    while (!glfwWindowShouldClose(window)) {
        double currTime = glfwGetTime();
        double dt = currTime - lastTime;
        lastTime = currTime;
        frameCount++;
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // lr.step(bh, dt, 1);
        for (int i = 0; i < Funsies::NUM_LIGHT_RAYS; ++i) {
            lightRays[i].step(bh, dt, 1);
        }

        if (currTime - fpsTimer >= 0) {
            double fps = frameCount / (currTime - fpsTimer);
            double ms = 1000.0 / std::max(0.0001, fps);
            std::string newTitle =
                "Engine " + std::to_string(fps) + " FPS / " + std::to_string(ms) + " ms";
            glfwSetWindowTitle(window, newTitle.c_str());
            fpsTimer = currTime;
            frameCount = 0;
        }
        // Handles camera inputs
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(camera.fovY, CameraConfig::CLOSE_DIST, CameraConfig::FAR_DIST);



        glDisable(GL_DEPTH_TEST);
        rayVAO.Bind();
        updateRays(camera, lightRayShader, bh);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glEnable(GL_DEPTH_TEST);

        // bh.draw(shaderProgram, camera);

        // Draws different meshes
        // floor.Draw(shaderProgram, camera);
        // light.Draw(lightShader, camera);

        // for (int i = 0; i < Funsies::NUM_LIGHT_RAYS; ++i) {
        //     lightRays[i].draw(lightShader, camera);
        // }

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    shaderProgram.Delete();
    lightShader.Delete();
    lightRayShader.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}