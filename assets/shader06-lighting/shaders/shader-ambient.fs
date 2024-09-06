#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

uniform float time;

out vec4 finalColor;

void main() {
    vec3 colour = vec3(1.0, 0.0, 0.0);

    finalColor = vec4(colour, 1.0);
}

