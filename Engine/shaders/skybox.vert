#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragPosition;

// UBO with view and proj matrices
layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

// Push constant for model matrix
layout(push_constant) uniform PushConstants {
    mat4 model;
} push;

void main() {
    // Remove translation from view matrix for skybox
    mat4 viewNoTranslation = mat4(mat3(ubo.view));
    
    vec4 position = ubo.proj * viewNoTranslation * push.model * vec4(inPosition, 1.0);
    gl_Position = position.xyww; // Use w for z and w to ensure skybox is always at far plane
    fragPosition = inPosition;
}