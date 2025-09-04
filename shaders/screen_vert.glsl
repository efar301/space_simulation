#version 400 core

layout(location = 0) in aPos;

out vec2 vNDC;
out vec2 vUv;

void main() {
    gl_position = vec4(aPos.xy, 1.0, 1.0);
    vNDC = aPos.xy;
    vUv = vNDC * 0.5 + 0.5;
}