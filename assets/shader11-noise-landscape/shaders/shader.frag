#version 330

in vec2 fragTexCoord;

uniform vec2 resolution;
uniform float time;

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

float ridgedFBM(vec3 p, int octaves, float persistence, float lacunarity) {
    float amplitude = 0.5;
    float frequency = 1.0;
    float total = 0.0;
    float normalization = 0.0;

    for (int i = 0; i < octaves; i++) {
        float noiseValue = noise3D(p * frequency);
        noiseValue = 1.0 - abs(noiseValue); // Ridge

        total += amplitude * noiseValue;
        normalization += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;
    total *= total; // Square to increase contrast
    return total;
}

float turbulenceFBM(vec3 p, int octaves, float persistence, float lacunarity) {
    float amplitude = 0.5;
    float frequency = 1.0;
    float total = 0.0;
    float normalization = 0.0;

    for (int i = 0; i < octaves; i++) {
        float noiseValue = noise3D(p * frequency);
        noiseValue = abs(noiseValue); // Turbulence

        total += amplitude * noiseValue;
        normalization += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;
    return total;
}

// Voronoi noise
float cellular(vec3 coords) {
    vec2 gridBasePosition = floor(coords.xy);
    vec2 gridOffset = fract(coords.xy);

    float closest = 1.0;
    for(float y = -2.0; y <= 2.0; y++) {
        for(float x = -2.0; x <= 2.0; x++) {
            vec2 neighbourCellPosition = vec2(x, y);
            vec2 cellWorldPosition = gridBasePosition + neighbourCellPosition;
            vec2 cellOffset = vec2(
            noise3D(vec3(cellWorldPosition, coords.z) + vec3(243.432, 324.235, 0.0)),
            noise3D(vec3(cellWorldPosition, coords.z))
            );
            float distanceToNeighboor = length(neighbourCellPosition + cellOffset - gridOffset);
            closest = min(closest, distanceToNeighboor);
        }
    }

    return closest;
}

float stepped(float noiseSample) {
    float steppedSample = floor(noiseSample * 10.0) / 10.0;
    float remainder = fract(noiseSample * 10.0);
    steppedSample = (steppedSample - remainder) * 0.5 + 0.5;
    return steppedSample;
}

float domainWarpingFBM(vec3 coords) {
    vec3 offset = vec3(
    fbm(coords, 4, 0.5, 2.0),
    fbm(coords + vec3(43.235, 23.112, 0.0), 4, 0.5, 2.0),
    0.0
    );
    float noiseSample = fbm(coords + offset, 1, 0.5, 2.0);

    vec3 offset2 = vec3(
    fbm(coords + 4.0 * offset + vec3(5.325, 1.421, 3.235), 4, 0.5, 2.0),
    fbm(coords + 4.0 * offset +  vec3(4.32, 0.532, 6.324), 4, 0.5, 2.0),
    0.0
    );
    noiseSample = fbm(coords + 4.0 * offset2, 1, 0.5, 2.0);

    return noiseSample;
}

out vec4 finalColor;

vec3 generateSky() {
    vec3 colour0 = vec3(0.4, 0.6, 0.9);
    vec3 colour1 = vec3(0.1, 0.15, 0.4);
    return mix(colour0, colour1, smoothstep(0.875, 1.0, 1.0 - fragTexCoord.y));
}

vec3 drawMountains(vec3 background, vec3 mountainColour, vec2 pixelCoords, float depth) {
    // Mountain with an horizontal offset proportional to the depth
    float y = fbm(vec3(depth + pixelCoords.x / 256.0, 1.434, 3.643), 6, 0.5, 2.0) * 256.0;

    // Fog
    vec3 fogColour = vec3(0.4, 0.6, 0.9);
    float fogFactor = smoothstep(0.0, 8000.0, depth) * 0.5;

    // Fog at the bottom of the mountains
    float heightFactor = smoothstep(56.0, -512.0, pixelCoords.y);
    heightFactor *= heightFactor;
    fogFactor = mix(heightFactor, fogFactor, fogFactor);

    mountainColour = mix(mountainColour, fogColour, fogFactor);

    float sdfMountain = pixelCoords.y - y;

    // Blur (depth of field)
    float blur = 1.0
    // Blur background
    + smoothstep(600.0, 8000.0, depth) * 128.0;
    // Blur foreground
    + smoothstep(200.0, -1200.0, depth) * 128.0;

    vec3 colour = mix(mountainColour, background, smoothstep(0.0, blur, sdfMountain));
    return colour;
}

void main() {
    vec2 pixelCoords = vec2((fragTexCoord.x - 0.5) * resolution.x, -(fragTexCoord.y - 0.5) * resolution.y);
    vec3 colour = generateSky();
    vec2 timeOffset = vec2(time * 50.0, 0.0);

    vec2 mountainCoords = (pixelCoords - vec2(0.0, 300)) * 8.0 + timeOffset;
    colour = drawMountains(colour, vec3(0.5), mountainCoords, 6000.0);

    mountainCoords = (pixelCoords - vec2(0.0, 280)) * 4.0 + timeOffset;
    colour = drawMountains(colour, vec3(0.45), mountainCoords, 3200.0);

    mountainCoords = (pixelCoords - vec2(0.0, 240)) * 2.0 + timeOffset;
    colour = drawMountains(colour, vec3(0.4), mountainCoords, 1600.0);

    mountainCoords = (pixelCoords - vec2(0.0, 180)) * 1.0 + timeOffset;
    colour = drawMountains(colour, vec3(0.35), mountainCoords, 800.0);

    mountainCoords = (pixelCoords - vec2(0.0, 100)) * 0.5 + timeOffset;
    colour = drawMountains(colour, vec3(0.3), mountainCoords, 400.0);

    mountainCoords = (pixelCoords - vec2(0.0, -70)) * 0.25 + timeOffset;
    colour = drawMountains(colour, vec3(0.25), mountainCoords, 200.0);

    mountainCoords = (pixelCoords - vec2(0.0, -300)) * 0.125 + timeOffset;
    colour = drawMountains(colour, vec3(0.2), mountainCoords, -50.0);

    finalColor = vec4(colour, 1.0);
}
