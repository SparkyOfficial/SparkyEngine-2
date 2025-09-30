#version 450

// Material uniforms
layout(binding = 1) uniform MaterialBuffer {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    int hasTexture;
} material;

// Textures
layout(binding = 2) uniform sampler2D texSampler;

// Input from vertex shader
layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    // Normalize the normal vector
    vec3 norm = normalize(fragNormal);
    
    // Simple directional light (sun)
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    
    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    
    // Specular shading
    vec3 viewDir = normalize(-fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Combine results
    vec3 ambient = material.ambient.xyz;
    vec3 diffuse = material.diffuse.xyz * diff;
    vec3 specular = material.specular.xyz * spec;
    
    // Sample texture
    vec4 texColor = texture(texSampler, fragTexCoord);
    
    // If we have a texture, use it; otherwise use material color
    vec3 result;
    if (material.hasTexture == 1) {
        result = texColor.rgb * (ambient + diffuse) + specular;
    } else {
        result = (ambient + diffuse) + specular;
    }
    
    outColor = vec4(result, texColor.a);
}