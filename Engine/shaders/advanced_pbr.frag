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
    float roughness;
    float metalness;
    int hasNormalMap;
    int hasRoughnessMap;
    int hasMetalnessMap;
    int padding[1]; // For alignment
} material;

// Textures
layout(binding = 3) uniform sampler2D texSampler;
layout(binding = 4) uniform sampler2D normalMap;
layout(binding = 5) uniform sampler2D roughnessMap;
layout(binding = 6) uniform sampler2D metalnessMap;
layout(binding = 7) uniform sampler2D shadowMap; // For shadow mapping
layout(binding = 8) uniform samplerCube irradianceMap; // For IBL
layout(binding = 9) uniform samplerCube prefilterMap; // For IBL
layout(binding = 10) uniform sampler2D brdfLUT; // For IBL

// Input from vertex shader
layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragBitangent;
layout(location = 5) in vec4 fragPosLightSpace; // For shadow mapping

layout(location = 0) out vec4 outColor;

// Constants
const float PI = 3.14159265359;

// Function to calculate shadow
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // Calculate bias to prevent shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    // PCF (Percentage Closer Filtering) for softer shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum
    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }
    
    return shadow;
}

// Function to calculate the Cook-Torrance BRDF
vec3 cookTorranceBRDF(vec3 lightDir, vec3 viewDir, vec3 normal, vec3 albedo, float roughness, float metallic) {
    // Calculate the half vector
    vec3 halfVector = normalize(lightDir + viewDir);
    
    // Calculate the dot products
    float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotH = max(dot(normal, halfVector), 0.0);
    float VdotH = max(dot(viewDir, halfVector), 0.0);
    
    // Calculate the Fresnel term (F)
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
    
    // Calculate the geometric term (G)
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    float G1L = NdotL / (NdotL * (1.0 - k) + k);
    float G1V = NdotV / (NdotV * (1.0 - k) + k);
    float G = G1L * G1V;
    
    // Calculate the roughness term (D)
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float denom = NdotH * NdotH * (alpha2 - 1.0) + 1.0;
    float D = alpha2 / (PI * denom * denom);
    
    // Combine all terms
    vec3 specular = (D * G * F) / (4.0 * NdotL * NdotV + 0.0001);
    
    // Calculate the diffuse term
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    
    // Combine diffuse and specular
    vec3 diffuse = kD * albedo / PI;
    
    return (diffuse + specular) * NdotL;
}

// Function to calculate directional light with PBR
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metallic) {
    vec3 lightDir = normalize(-light.direction.xyz);
    float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);
    vec3 radiance = cookTorranceBRDF(lightDir, viewDir, normal, albedo, roughness, metallic);
    return radiance * (1.0 - shadow);
}

// Function to calculate point light with PBR
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float roughness, float metallic) {
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    
    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);
    vec3 radiance = cookTorranceBRDF(lightDir, viewDir, normal, albedo, roughness, metallic);
    return radiance * attenuation * (1.0 - shadow);
}

// Function to calculate spot light with PBR
vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float roughness, float metallic) {
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    
    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));
    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction.xyz)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);
    vec3 radiance = cookTorranceBRDF(lightDir, viewDir, normal, albedo, roughness, metallic);
    return radiance * attenuation * intensity * (1.0 - shadow);
}

// Function to sample IBL
vec3 sampleIBL(vec3 albedo, vec3 normal, vec3 viewDir, float roughness, float metallic) {
    // Sample irradiance map for diffuse IBL
    vec3 irradiance = texture(irradianceMap, normal).rgb;
    
    // Calculate diffuse contribution
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 kS = F0 + (1.0 - F0) * pow(1.0 - max(dot(viewDir, normal), 0.0), 5.0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec3 diffuse = irradiance * albedo;
    
    // Sample pre-filtered environment map for specular IBL
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 reflection = reflect(-viewDir, normal);
    vec3 prefilteredColor = textureLod(prefilterMap, reflection, roughness * MAX_REFLECTION_LOD).rgb;
    
    // Sample BRDF LUT
    vec2 brdf = texture(brdfLUT, vec2(max(dot(normal, viewDir), 0.0), roughness)).rg;
    
    // Combine for specular
    vec3 specular = prefilteredColor * (F0 * brdf.x + brdf.y);
    
    // Combine diffuse and specular IBL
    return kD * diffuse + specular;
}

void main() {
    // Sample textures
    vec4 texColor = texture(texSampler, fragTexCoord);
    vec3 normalMapSample = texture(normalMap, fragTexCoord).rgb;
    float roughnessSample = texture(roughnessMap, fragTexCoord).r;
    float metalnessSample = texture(metalnessMap, fragTexCoord).r;
    
    // Create TBN matrix for normal mapping
    vec3 T = normalize(fragTangent);
    vec3 B = normalize(fragBitangent);
    vec3 N = normalize(fragNormal);
    mat3 TBN = mat3(T, B, N);
    
    // Apply normal mapping
    vec3 normal;
    if (material.hasNormalMap == 1) {
        normalMapSample = normalMapSample * 2.0 - 1.0; // Transform from [0,1] to [-1,1]
        normal = normalize(TBN * normalMapSample);
    } else {
        normal = N;
    }
    
    // Get material properties
    vec3 albedo = texColor.rgb;
    if (material.hasTexture != 1) {
        albedo = material.diffuse.rgb;
    }
    
    float roughness = material.roughness;
    if (material.hasRoughnessMap == 1 && roughnessSample > 0.0) {
        roughness = roughnessSample;
    }
    
    float metallic = material.metalness;
    if (material.hasMetalnessMap == 1 && metalnessSample > 0.0) {
        metallic = metalnessSample;
    }
    
    // Properties
    vec3 viewDir = normalize(-fragPosition);
    
    // Start with ambient lighting
    vec3 result = material.ambient.rgb * albedo;
    
    // Add IBL contribution
    result += sampleIBL(albedo, normal, viewDir, roughness, metallic);
    
    // Process all lights
    for(int i = 0; i < min(lightData.numLights, MAX_LIGHTS); i++) {
        Light light = lightData.lights[i];
        
        // Check light type based on position.w
        if (light.position.w == 0.0) {
            // Directional light
            result += CalcDirLight(light, normal, viewDir, albedo, roughness, metallic);
        } else {
            // Point light or spot light
            if (light.direction.xyz == vec3(0.0)) {
                // Point light
                result += CalcPointLight(light, normal, fragPosition, viewDir, albedo, roughness, metallic);
            } else {
                // Spot light
                result += CalcSpotLight(light, normal, fragPosition, viewDir, albedo, roughness, metallic);
            }
        }
    }
    
    outColor = vec4(result, texColor.a);
}