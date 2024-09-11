#version 330

in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 finalColor;

void main() {
    vec4 diffuse = texture(texture1, fragTexCoord);
    vec4 mask = texture(texture2, fragTexCoord * 3.0);
    finalColor = mix(diffuse, mask, mask.a);
}