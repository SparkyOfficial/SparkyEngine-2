#version 450 core

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform sampler2D texSampler;

void main() {
    // For now, we'll just use the vertex color
    // In a more advanced implementation, we would sample from textures
    outColor = vec4(fragColor, 1.0);
    
    // If we had textures:
    // outColor = texture(texSampler, fragTexCoord) * vec4(fragColor, 1.0);
}