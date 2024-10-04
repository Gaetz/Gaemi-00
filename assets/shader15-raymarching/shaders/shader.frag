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

float saturate(float value) {
    return clamp(value, 0.0, 1.0);
}

float sdfSphere(vec3 pos, float radius) {
    return length(pos) - radius;
}

// Computes the overall SDF. Return the distance to the closest point on the surface.
float map(vec3 pos) {
    float dist = sdfSphere(pos - vec3(0.0, 0.0, 5.0), 1.0);
    return dist;
}

const int NUM_STEPS = 256;
const float MAX_DIST = 1000.0;

// Performs sphere tracing for the scene. Return the colour of the pixel at sphere intersection.
vec3 rayMarch(vec3 cameraOrigin, vec3 cameraDir) {
    vec3 pos;
    float dist = 0.0;
    for (int i = 0; i < NUM_STEPS; i++) {
        pos = cameraOrigin + cameraDir * dist;
        float distToScene = map(pos);

        // Case 1: distToScene < 0, intersection with the scene
        if (distToScene < 0.001) {
            break;
        }
        dist += distToScene;

        // Case 2 : disttoo big, out of the scene entirely
        if (dist >= MAX_DIST) {
            return vec3(0.0);
        }

        // Case 3: neither intersecting nor travelling too far, just continue looping
    }

    return vec3(1.0);
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
