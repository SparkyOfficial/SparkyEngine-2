#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec3 fragBitangent;
layout(location = 5) out vec4 fragPosLightSpace; // For shadow mapping

// UBO with view and proj matrices
layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    mat4 lightSpaceMatrix; // For shadow mapping
} ubo;

// Push constant for model matrix
layout(push_constant) uniform PushConstants {
    mat4 model;
} push;

void main() {
    vec4 worldPosition = push.model * vec4(inPosition, 1.0);
    fragPosition = worldPosition.xyz;
    
    // Transform normal, tangent, and bitangent to world space
    fragNormal = normalize(mat3(transpose(inverse(push.model))) * inNormal);
    fragTangent = normalize(mat3(push.model) * inTangent);
    fragBitangent = normalize(mat3(push.model) * inBitangent);
    
    fragTexCoord = inTexCoord;
    
    // Calculate position in light space for shadow mapping
    fragPosLightSpace = ubo.lightSpaceMatrix * worldPosition;
    
    gl_Position = ubo.proj * ubo.view * worldPosition;
}