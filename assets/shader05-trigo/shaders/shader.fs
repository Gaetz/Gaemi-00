#version 330

in vec2 fragTexCoord;

uniform float time;

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

    colour = vec3(time);


    finalColor = vec4(colour, 1.0);
}


