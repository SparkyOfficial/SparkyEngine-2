#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;

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
    vec4 worldPosition = push.model * vec4(inPosition, 1.0);
    fragPosition = worldPosition.xyz;
    
    // Transform normal to world space
    fragNormal = mat3(transpose(inverse(push.model))) * inNormal;
    
    fragTexCoord = inTexCoord;
    
    gl_Position = ubo.proj * ubo.view * worldPosition;
}