#version 330

in vec2 fragTexCoord;

uniform float time;

vec3 red = vec3(1.0, 0.0, 0.0);
vec3 blue = vec3(0.0, 0.0, 1.0);

float inverseLerp(float current, float min, float max) {
    return (current - min) / (max - min);
}

float remap(float current, float inMin, float inMax, float outMin, float outMax) {
    float t = inverseLerp(current, inMin, inMax);
    return mix(outMin, outMax, t);
}

out vec4 finalColor;

void main() {
    vec3 colour = vec3(0.75);

    // 1. Time adjustment
//     colour = vec3(time);

    // 2. Time with sine
//     float t = sin(time);
//     t = remap(t, -1.0, 1.0, 0.0, 1.0);
//     colour = mix(red, blue, t);

    // 3. Chess pattern with sine and cosine
//     float t0 = sin(fragTexCoord.x * 100.0);
//     float t1 = sin(fragTexCoord.y * 100.0);
//     colour = vec3(t0*t1);

    // 4. Tile pattern with sine and cosine
//     float t0 = sin(fragTexCoord.x * 100.0);
//     float t1 = sin(fragTexCoord.y * 100.0);
//     colour = vec3(max(t0, t1));

    // 5. CRT-screen style scanlines
    float t0 = remap(sin(fragTexCoord.y * 400.0 + time * 10.0), -1.0, 1.0, 0.9, 1.0);
    float t1 = remap(sin(fragTexCoord.y * 50.0 - time * 2.0), -1.0, 1.0, 0.9, 1.0);
    colour = colour * t0 * t1;

    finalColor = vec4(colour, 1.0);
}

// Exercise: use a texture with this scanline effect.