#version 450

layout(binding = 3) uniform samplerCube skyboxTexture;

layout(location = 0) in vec3 fragPosition;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(skyboxTexture, fragPosition);
}