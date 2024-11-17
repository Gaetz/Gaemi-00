#version 330

in vec2 fragTexCoord;

uniform vec2 resolution;
uniform float time;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

float inverseLerp(float current, float min, float max) {
    return (current - min) / (max - min);
}

float remap(float current, float inMin, float inMax, float outMin, float outMax) {
    float t = inverseLerp(current, inMin, inMax);
    return mix(outMin, outMax, t);
}

// Random function by Inigo Quilez
float easyRandom(vec2 p) {
    p = 50.0 * fract(p * 0.3183099 + vec2(0.71, 0.113));
    return -1.0 + 2.0 * fract(p.x * p.y * (p.x + p.y));
}

// Random function by Inigo Quilez
vec3 easyRandom3D(vec3 p) {
    p = vec3( dot(p,vec3(127.1,311.7,74.7)),
    dot(p,vec3(269.5,183.3,246.1)),
    dot(p,vec3(113.5,271.9,124.6)));

    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

vec4 noise(vec2 coords) {
    vec2 size = vec2(1.0);
    vec2 pc = coords * size;
    vec2 base = floor(pc);

    float s1 = easyRandom((base + vec2(0.0, 0.0)) / size);
    float s2 = easyRandom((base + vec2(1.0, 0.0)) / size);
    float s3 = easyRandom((base + vec2(0.0, 1.0)) / size);
    float s4 = easyRandom((base + vec2(1.0, 1.0)) / size);

    vec2 f = smoothstep(0.0, 1.0, fract(pc));

    float px1 = mix(s1, s2, f.x);
    float px2 = mix(s3, s4, f.x);
    float result = mix(px1, px2, f.y);
    return vec4(result);
}

float noise3D(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);

    vec3 u = f * f * (3.0 - 2.0 * f);
    float result = mix(mix(mix(dot(easyRandom3D(i + vec3(0.0, 0.0, 0.0)), f - vec3(0.0, 0.0, 0.0)),
    dot(easyRandom3D(i + vec3(1.0, 0.0, 0.0)), f - vec3(1.0, 0.0, 0.0)), u.x),
    mix(dot(easyRandom3D(i + vec3(0.0, 1.0, 0.0)), f - vec3(0.0, 1.0, 0.0)),
    dot(easyRandom3D(i + vec3(1.0, 1.0, 0.0)), f - vec3(1.0, 1.0, 0.0)), u.x), u.y),
    mix(mix(dot(easyRandom3D(i + vec3(0.0, 0.0, 1.0)), f - vec3(0.0, 0.0, 1.0)),
    dot(easyRandom3D(i + vec3(1.0, 0.0, 1.0)), f - vec3(1.0, 0.0, 1.0)), u.x),
    mix(dot(easyRandom3D(i + vec3(0.0, 1.0, 1.0)), f - vec3(0.0, 1.0, 1.0)),
    dot(easyRandom3D(i + vec3(1.0, 1.0, 1.0)), f - vec3(1.0, 1.0, 1.0)), u.x), u.y), u.z);
    return result;
}

// Fractional Brownian Motion. Increase octaves for more detail.
float fbm(vec3 p, int octaves, float persistence, float lacunarity) {
    float amplitude = 0.5;
    float frequency = 1.0;
    float total = 0.0;
    float normalization = 0.0;

    for (int i = 0; i < octaves; i++) {
        float noiseValue = noise3D(p * frequency);
        total += amplitude * noiseValue;
        normalization += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;
    return total;
}

float sdfCircle(vec2 p, float radius) {
    return length(p) - radius;
}

out vec4 finalColor;

void main() {
    vec2 pixelCoords = vec2((fragTexCoord.x - 0.5) * resolution.x, -(fragTexCoord.y - 0.5) * resolution.y);

    // Sample noise for the fire effect irregularities
    float noiseSample = fbm(vec3(pixelCoords, 0.0) * 0.005, 4, 0.5, 2.0);

    float size = smoothstep(0.0, 15.0, time) * (50.0 + length(resolution) * 0.5);
    // Regular circle
    //float d = sdfCircle(pixelCoords, size);
    // Noisy rimed circle. Try 50.0 then 200.0 as a multiplier.
    float d = sdfCircle(pixelCoords + 50.0 * noiseSample, size);

    // Image distortion
    vec2 distortion = noiseSample / resolution;
    vec2 uvDistortion = distortion * 20.0 * smoothstep(80.0, 20.0, d);

    vec3 sample1 = texture(texture1, fragTexCoord + uvDistortion).rgb;
    vec3 sample2 = texture(texture2, fragTexCoord).rgb;
    vec3 colour = sample1;

    // Dark burning effect
    float burnAmout = 1.0 - exp(-d*d*0.001);
    colour = mix (vec3(0), colour, burnAmout);
    // Orange rim
    vec3 rimColor = vec3(1.0, 0.5, 0.2);
    float orangeAmount = smoothstep(0.0, 10.0, d);
    orangeAmount = pow(orangeAmount, 0.5);
    colour = mix(rimColor, colour, orangeAmount);

    colour = mix(sample2, colour, smoothstep(0.0, 1.0, d));

    // Glowing fire effect
    float glowAmount = smoothstep(0.0, 32.0, abs(d));
    glowAmount = 1.0 - pow(glowAmount, 0.125);
    colour += glowAmount * vec3(1.0, 0.2, 0.05);

    finalColor = vec4(colour, 1.0);
}
