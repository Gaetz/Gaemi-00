in vec2 fragTexCoord;

uniform vec4 color1;
uniform vec4 color2;

void main() {
    gl_FragColor = mix(color1, color2, fragTexCoord.x);
}