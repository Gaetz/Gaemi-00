#version 330

in vec2 fragTexCoord;

uniform vec2 resolution;

vec3 red = vec3(1.0, 0.0, 0.0);
vec3 blue = vec3(0.0, 0.0, 1.0);
vec3 white = vec3(1.0, 1.0, 1.0);
vec3 black = vec3(0.0, 0.0, 0.0);
vec3 grey = vec3(0.5, 0.5, 0.5);
vec3 yellow = vec3(1.0, 1.0, 0.0);

float inverseLerp(float current, float min, float max) {
    return (current - min) / (max - min);
}

float remap(float current, float inMin, float inMax, float outMin, float outMax) {
    float t = inverseLerp(current, inMin, inMax);
    return mix(outMin, outMax, t);
}

vec3 drawBackground() {
    float distFromCenter = length(abs(fragTexCoord - 0.5));

    float vignette = 1.0 - distFromCenter;
    vignette = smoothstep(0.0, 0.7, vignette);
    vignette = remap(vignette, 0.0, 1.0, 0.3, 1.0);

    return vec3(vignette);
}

vec3 drawGrid(vec3 colour, vec3 lineColour, float cellSpacing, float lineWidth) {
    vec2 center = fragTexCoord - 0.5;
    vec2 cells = abs(fract(center * resolution / cellSpacing) - 0.5);
    float distToEdge = (0.5 - max(cells.x, cells.y)) * cellSpacing;
    float lines = smoothstep(0.0, lineWidth, distToEdge);

    colour = mix(lineColour, colour, lines);
    return colour;
}


out vec4 finalColor;

void main() {
    vec2 pixelCoord = (fragTexCoord - 0.5) * resolution;

    vec3 colour = drawBackground();
    colour = drawGrid(colour, grey, 10.0, 1.0);
    colour = drawGrid(colour, black, 100.0, 2.0);

    finalColor = vec4(colour, 1.0);
}
