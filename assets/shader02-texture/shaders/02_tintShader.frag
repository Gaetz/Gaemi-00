#version 330

in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform vec4 tint;

out vec4 finalColor;

void main() {
    finalColor = texture(texture0, fragTexCoord) * tint;
}