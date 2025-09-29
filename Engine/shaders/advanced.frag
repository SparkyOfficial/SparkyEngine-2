#version 450

// Lighting uniforms
#define MAX_LIGHTS 100

struct Light {
    vec4 position;     // w component indicates light type (1.0 for point, 0.0 for directional)
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

layout(binding = 1) uniform LightingBuffer {
    Light lights[MAX_LIGHTS];
    int numLights;
} lightData;

layout(binding = 2) uniform MaterialBuffer {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    int hasTexture;
} material;

// Textures
layout(binding = 3) uniform sampler2D texSampler;

// Input from vertex shader
layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

// Function to calculate directional light
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction.xyz);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Combine results
    vec3 ambient = light.ambient.xyz * material.ambient.xyz;
    vec3 diffuse = light.diffuse.xyz * diff * material.diffuse.xyz;
    vec3 specular = light.specular.xyz * spec * material.specular.xyz;
    
    return ambient + diffuse + specular;
}

// Function to calculate point light
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Combine results
    vec3 ambient = light.ambient.xyz * material.ambient.xyz;
    vec3 diffuse = light.diffuse.xyz * diff * material.diffuse.xyz;
    vec3 specular = light.specular.xyz * spec * material.specular.xyz;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

// Function to calculate spot light
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction.xyz)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Combine results
    vec3 ambient = light.ambient.xyz * material.ambient.xyz;
    vec3 diffuse = light.diffuse.xyz * diff * material.diffuse.xyz;
    vec3 specular = light.specular.xyz * spec * material.specular.xyz;
    
    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ambient + diffuse + specular;
}

void main() {
    // Properties
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(-fragPosition);
    
    // Sample texture
    vec4 texColor = texture(texSampler, fragTexCoord);
    
    // If we have a texture, use it; otherwise use material color
    vec3 result = vec3(0.0);
    if (material.hasTexture == 1) {
        result = texColor.rgb;
    } else {
        result = material.diffuse.rgb;
    }
    
    // Process all lights
    for(int i = 0; i < min(lightData.numLights, MAX_LIGHTS); i++) {
        Light light = lightData.lights[i];
        
        // Check light type based on position.w
        if (light.position.w == 0.0) {
            // Directional light
            result += CalcDirLight(light, norm, viewDir);
        } else {
            // Point light or spot light
            if (light.direction.xyz == vec3(0.0)) {
                // Point light
                result += CalcPointLight(light, norm, fragPosition, viewDir);
            } else {
                // Spot light
                result += CalcSpotLight(light, norm, fragPosition, viewDir);
            }
        }
    }
    
    outColor = vec4(result, texColor.a);
}