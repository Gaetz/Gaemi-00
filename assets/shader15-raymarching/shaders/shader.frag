#version 410

in vec2 fragTexCoord;

uniform vec2 resolution;
uniform float time;

const int NUM_STEPS = 256;
const float MAX_DIST = 1000.0;

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

// Computes the overall SDF. Return the distance to the closest point on the surface.
MaterialData map(vec3 pos) {
    // 1. Basic scene with no data (use float as a return type)
    //    float dist = sdfPlane(pos - vec3(0.0, -2.0, 0.0));
    //    dist = min(dist, sdfBox(pos - vec3(-2.0, -0.85, 5.0), vec3(1.0)));
    //    dist = min(dist, sdfBox(pos - vec3(2.0, -0.85, 5.0), vec3(1.0)));
    //    return dist;

    // 2. Scene with material data
    MaterialData result = MaterialData(GREY, sdfPlane(pos - vec3(0.0, -2.0, 0.0)));
    float dist;

    // First box
    dist = sdfBox(pos - vec3(-2.0, -0.85, 5.0), vec3(1.0));
    result.colour = dist < result.dist ? RED : result.colour;
    result.dist = min(dist, result.dist);

    // Second box
    dist = sdfBox(pos - vec3(2.0, -0.85, 5.0), vec3(1.0));
    result.colour = dist < result.dist ? BLUE : result.colour;
    result.dist = min(dist, result.dist);

    // Second box
    dist = sdfBox(pos - vec3(2.0, 1.0, 50.0 + sin(time) * 25.0), vec3(2.0));
    result.colour = dist < result.dist ? GREEN : result.colour;
    result.dist = min(dist, result.dist);

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

float computeShadow(vec3 pos, vec3 lightDir) {
    float d = 0.01;
    for (int i = 0; i < 64; i++) {
        float distToScene = map(pos + lightDir * d).dist;
        if (distToScene < 0.001) {
            return 0.0;
        }
        d += distToScene;
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
    vec3 pos;
    MaterialData material = MaterialData(vec3(0.0), 0.0);

    vec3 skyColour = vec3(0.55, 0.6, 1.0);

    for (int i = 0; i < NUM_STEPS; i++) {
        pos = cameraOrigin + cameraDir * material.dist;
        MaterialData result = map(pos);

        // Case 1: distToScene < 0, intersection with the scene
        if (result.dist < 0.001) {
            break;
        }
        material.dist += result.dist;
        material.colour = result.colour;

        // Case 2 : dist too big, out of the scene entirely
        if (material.dist >= MAX_DIST) {
            return skyColour;
        }

        // Case 3: neither intersecting nor travelling too far, just continue looping
    }

    vec3 lightColour = WHITE;
    vec3 lightDir = normalize(vec3(1.0, 2.0, -1.0));
    vec3 normal = computeNormal(pos);
    float shadowed = computeShadow(pos, lightDir);
    vec3 lighting = computeLighting(pos, normal, lightColour, lightDir);
    lighting *= shadowed;

    float ao = computeAmbientOcclusion(pos, normal);
    vec3 colour = material.colour * lighting * ao;

    float fogFactor = 1.0 - exp(-pos.z * 0.01);
    colour = mix(colour, skyColour, fogFactor);

    return colour;
}


out vec4 finalColor;


void main() {
    vec2 pixelCoords = vec2((fragTexCoord.x - 0.5) * resolution.x, -(fragTexCoord.y - 0.5) * resolution.y);
    vec3 colour = vec3(0.0);

    vec3 rayDir = normalize(vec3(pixelCoords * 2.0 / resolution.y, 1.0));
    vec3 rayOrigin = vec3(0.0, 0.0, 0.0);

    colour = rayMarch(rayOrigin, rayDir);

    finalColor = vec4(colour, 1.0);
}
