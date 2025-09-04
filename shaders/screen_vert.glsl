#version 400 core

layout(location = 0) in vec3 aPos;

out vec2 vNDC;

void main() {
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    vNDC = aPos.xy;
}