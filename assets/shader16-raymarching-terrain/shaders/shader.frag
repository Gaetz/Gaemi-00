#version 410

in vec2 fragTexCoord;

uniform vec2 resolution;
uniform float time;

const int NUM_STEPS = 256;
const float MAX_DIST = 1000.0;
const float MIN_DIST = 0.00001;

const vec3 RED = vec3(1.0, 0.0, 0.0);
const vec3 GREEN = vec3(0.0, 1.0, 0.0);
const vec3 BLUE = vec3(0.0, 0.0, 1.0);
const vec3 GREY = vec3(0.5, 0.5, 0.5);
const vec3 WHITE = vec3(1.0, 1.0, 1.0);


float inverseLerp(float current, float min, float max) {
    return (current - min) / (max - min);
}

float remap(float current, float inMin, float inMax, float outMin, float outMax) {
    float t = inverseLerp(current, inMin, inMax);
    return mix(outMin, outMax, t);
}

float saturate(float value) {
    return clamp(value, 0.0, 1.0);
}

float sdfSphere(vec3 pos, float radius) {
    return length(pos) - radius;
}

float sdfBox(vec3 pos, vec3 size) {
    vec3 d = abs(pos) - size;
    return length(max(d, 0.0)) + min(max(d.x, max(d.y, d.z)), 0.0);
}

float sdfTorus(vec3 pos, vec2 t) {
    vec2 q = vec2(length(pos.xz) - t.x, pos.y);
    return length(q) - t.y;
}

float sdfPlane(vec3 pos) {
    return pos.y;
}

struct MaterialData {
    vec3 colour;
    float dist;
};

float easyRandom(vec2 p) {
    p = 50.0 * fract(p * 0.3183099 + vec2(0.71, 0.113));
    return -1.0 + 2.0 * fract(p.x * p.y * (p.x + p.y));
}

float noise(vec2 coords) {
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
    return result;
}

float fbm(vec2 p, int octaves, float persistence, float lacunarity) {
    float amplitude = 0.5;
    float total = 0.0;

    for (int i = 0; i < octaves; i++) {
        float noiseValue = noise(p);
        total += amplitude * noiseValue;
        amplitude *= persistence;
        p *= lacunarity;
    }

    return total;
}

MaterialData opUnion(MaterialData a, MaterialData b) {
    return a.dist < b.dist ? a : b;
}

// Computes the overall SDF. Return the distance to the closest point on the surface.
MaterialData map(vec3 pos) {
    float curNoiseDample = fbm(pos.xz / 2.0, 1, 0.5, 2.0);
    curNoiseDample = abs(curNoiseDample);
    curNoiseDample *= 1.5;
    curNoiseDample += 0.1 * fbm(pos.xz * 4.0, 6, 0.5, 2.0);

    float WATER_LEVEL = 0.45;

    vec3 landColour = vec3(0.498, 0.435, 0.396);
    landColour = mix(landColour, landColour * 0.25, smoothstep(WATER_LEVEL -0.1, WATER_LEVEL, curNoiseDample));
    MaterialData result = MaterialData(landColour, pos.y + curNoiseDample);

    vec3 shallowColour = vec3(0.25, 0.25, 0.75);
    vec3 deepColour = vec3(0.025, 0.025, 0.15);
    vec3 waterColour = mix(shallowColour, deepColour, smoothstep(WATER_LEVEL, WATER_LEVEL + 0.1, curNoiseDample));
    waterColour = mix(waterColour, WHITE, smoothstep(WATER_LEVEL + 0.0125, WATER_LEVEL, curNoiseDample));
    MaterialData waterMaterial = MaterialData(waterColour, pos.y + WATER_LEVEL);

    result = opUnion(result, waterMaterial);

    return result;
}


// Computes the normal of the surface at a given point by approximating the gradient (derivative) of the SDF.
vec3 computeNormal(vec3 pos) {
    const float EPSILON = 0.0001;
    vec3 n = vec3(
    map(pos + vec3(EPSILON, 0.0, 0.0)).dist - map(pos - vec3(EPSILON, 0.0, 0.0)).dist,
    map(pos + vec3(0.0, EPSILON, 0.0)).dist - map(pos - vec3(0.0, EPSILON, 0.0)).dist,
    map(pos + vec3(0.0, 0.0, EPSILON)).dist - map(pos - vec3(0.0, 0.0, EPSILON)).dist
    );
    return normalize(n);
}

vec3 computeLighting(vec3 pos, vec3 normal, vec3 lightColour, vec3 lightDir) {
    float dp = saturate(dot(normal, lightDir));
    return lightColour * dp;
}

MaterialData rayCast(vec3 cameraOrigin, vec3 cameraDir, int numSteps, float startDist, float maxDist) {
    MaterialData material = MaterialData(vec3(0.0), startDist);
    MaterialData defaultMaterial = MaterialData(vec3(0.0), -1.0);

    // Raymarching code
    for (int i = 0; i < NUM_STEPS; i++) {
        vec3 pos = cameraOrigin + cameraDir * material.dist;
        MaterialData result = map(pos);

        // Case 1: Intersection with the scene
        if (abs(result.dist) < MIN_DIST * material.dist) {
            break;
        }
        material.dist += result.dist;
        material.colour = result.colour;

        // Case 2 : dist too big, out of the scene entirely
        if (material.dist >= maxDist) {
            return defaultMaterial;
        }

        // Case 3: neither intersecting nor travelling too far, just continue looping
    }
    return material;
}

float computeShadow(vec3 pos, vec3 lightDir) {
    MaterialData result = rayCast(pos, lightDir, 64, 0.01, 10.0);
    if (result.dist >= 0) {
        return 0.0;
    }
    return 1.0;
}

float computeAmbientOcclusion(vec3 pos, vec3 normal) {
    float ao = 0.0;
    float stepSize = 0.1;

    for (float i = 0.0; i < 5.0; i++) {
        float distFactor = 1.0 / pow(2.0, i);
        ao += distFactor * (i * stepSize - map(pos + normal * i * stepSize).dist);
    }
    return 1.0 - ao;
}

// Performs sphere tracing for the scene. Return the colour of the pixel at sphere intersection.
//vec3 rayMarchSimple(vec3 cameraOrigin, vec3 cameraDir) {
//    vec3 pos;
//    float dist = 0.0;
//
//    for (int i = 0; i < NUM_STEPS; i++) {
//        pos = cameraOrigin + cameraDir * dist;
//        float distToScene = map(pos);
//
//        // Case 1: distToScene < 0, intersection with the scene
//        if (distToScene < 0.001) {
//            break;
//        }
//        dist += distToScene;
//
//        // Case 2 : disttoo big, out of the scene entirely
//        if (dist >= MAX_DIST) {
//            return vec3(0.0);
//        }
//
//        // Case 3: neither intersecting nor travelling too far, just continue looping
//    }
//
//    return vec3(1.0);
//}

vec3 rayMarch(vec3 cameraOrigin, vec3 cameraDir) {

    MaterialData material = rayCast(cameraOrigin, cameraDir, NUM_STEPS, 1.0, MAX_DIST);
    /* First sky colour
    vec3 skyColour = vec3(0.55, 0.6, 1.0);
    skyColour = mix(skyColour, skyColour * 0.25, smoothstep(0.0, 0.5, cameraDir.y));
    */
    /* Second sky colour */
    vec3 lightDir = normalize(vec3(-0.5, 0.2, -0.6));
    vec3 lightColour = WHITE;
    float skyT = exp(saturate(cameraDir.y) * -40.0);
    float sunFactor = pow(saturate(dot(lightDir, cameraDir)), 8.0);
    vec3 skyColour = mix(vec3(0.025, 0.065, 0.5), vec3(0.4, 0.5, 1.0), skyT);
    vec3 fogColour = mix(skyColour, vec3(1.0, 0.9, 0.65), sunFactor);
    if (material.dist < 0.0) {
        return fogColour;
    }

    vec3 pos = cameraOrigin + cameraDir * material.dist;

    vec3 normal = computeNormal(pos);
    float shadowed = computeShadow(pos, lightDir);
    vec3 lighting = computeLighting(pos, normal, lightColour, lightDir);
    lighting *= shadowed;

    float ao = computeAmbientOcclusion(pos, normal);
    vec3 colour = material.colour * lighting * ao;

    float fogDist = distance(cameraOrigin, pos);
    float inscatter = 1.0 - exp(-fogDist * fogDist * mix(0.0005, 0.001, sunFactor));
    float extinction = exp(-fogDist * fogDist * 0.01);
    colour = colour * extinction + fogColour * inscatter;

    return colour;
}

mat3 makeCameraMatrix(vec3 cameraOrigin, vec3 cameraLookAt, vec3 cameraUp) {
    vec3 z = normalize(cameraLookAt - cameraOrigin);
    vec3 x = normalize(cross(z, cameraUp));
    vec3 y = cross(x, z);
    return mat3(x, y, z);
}

out vec4 finalColor;

void main() {
    vec2 pixelCoords = vec2((fragTexCoord.x - 0.5) * resolution.x, -(fragTexCoord.y - 0.5) * resolution.y);
    vec3 colour = vec3(0.0);

    float t = time * 0.2;
    vec3 rayDir = normalize(vec3(pixelCoords * 2.0 / resolution.y, 1.0));
    vec3 rayOrigin = vec3(3.0, 0.75, -3.0) * vec3(cos(t), 1.0, sin(t));
    vec3 rayLookAt = vec3(0.0, 0.0, 0.0);
    mat3 camera = makeCameraMatrix(rayOrigin, rayLookAt, vec3(0.0, 1.0, 0.0));

    colour = rayMarch(rayOrigin, camera * rayDir);

    finalColor = vec4(colour, 1.0);
}
