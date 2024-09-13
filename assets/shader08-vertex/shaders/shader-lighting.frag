#version 410

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform float time;
uniform vec3 camPosition;
uniform samplerCube environmentMap;

out vec4 finalColor;

float inverseLerp(float current, float min, float max) {
    return (current - min) / (max - min);
}

float remap(float current, float inMin, float inMax, float outMin, float outMax) {
    float t = inverseLerp(current, inMin, inMax);
    return mix(outMin, outMax, t);
}

vec3 linearToSRGB(vec3 colour) {
    vec3 lt = vec3(lessThanEqual(colour, vec3(0.0031308)));
    vec3 v1 = colour * 12.92;
    vec3 v2 = pow(colour, vec3(0.41666)) * 1.055 - vec3(0.055);
    return mix(v2, v1, lt);
}

vec3 linearToSRGBApprox(vec3 colour) {
    return pow(colour, vec3(1.0 / 2.2));
}

void main() {
    vec3 baseColour = vec3(0.5);
    vec3 lighting = vec3(0.0);
    vec3 ambient  = vec3(0.5);
    vec3 viewDir = normalize(camPosition - fragPosition);

    vec3 normal = normalize(cross(dFdx(fragPosition), dFdy(fragPosition)));

    vec3 skyColour = vec3(0.0, 0.3, 0.6);
    vec3 groundColour = vec3(0.6, 0.3, 0.1);
    float hemiMix = remap(normal.y, -1.0, 1.0, 0.0, 1.0);
    vec3 hemi = mix(groundColour, skyColour, hemiMix);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 lightColour = vec3(1.0, 1.0, 0.9);
    float lambertian = max(0.0, dot(lightDir, normal));
    vec3 diffuse = lambertian * lightColour;
    lighting = ambient * 0.1 + hemi * 0.2 + diffuse * 0.7;

    vec3 r = normalize(reflect(-lightDir, normal));
    float phongValue = pow(max(0.0, dot(viewDir, r)), 32.0);
    vec3 specular = vec3(phongValue);

    vec3 iblCoord = normalize(reflect(-viewDir, normal));
    vec3 ibl = texture(environmentMap, iblCoord).rgb;
    specular += ibl * 0.5;

    float fresnel = 1.0 - max(0.0, dot(viewDir, normal));
    fresnel = pow(fresnel, 2.0);
    specular *= fresnel;

    vec3 colour = baseColour * lighting + specular + fragColor.rgb;
    colour = linearToSRGBApprox(colour); // Improves the lighting
    finalColor = vec4(colour, 1.0);
}

