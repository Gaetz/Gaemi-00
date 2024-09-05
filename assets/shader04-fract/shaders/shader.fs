#version 330

in vec2 fragTexCoord;

uniform vec2 resolution;

vec3 red = vec3(1.0, 0.0, 0.0);
vec3 blue = vec3(0.0, 0.0, 1.0);
vec3 white = vec3(1.0, 1.0, 1.0);
vec3 black = vec3(0.0, 0.0, 0.0);
vec3 yellow = vec3(1.0, 1.0, 0.0);

out vec4 finalColor;

void main() {
    vec3 colour = vec3(0.75);

    // 1. Color gradient
//     colour = vec3(fragTexCoord, 0.0);

    // 2. Multiply color gradient
//     colour = vec3(fragTexCoord, 0.0) * 10.0;

    // 3. Use fract to get the fractional part and repeat the pattern
//     colour = fract(vec3(fragTexCoord, 0.0) * 10.0);

    // 4. Now we want to draw cells with the inside of it being black and the border being our base color
//     vec2 cell = fract(fragTexCoord * 10.0);
//     cell = abs(cell - 0.5);
//     float distToCell = max(cell.x, cell.y);
//     colour = vec3(distToCell);

    // 5. Inverse the colours
//     vec2 cell = fract(fragTexCoord * 10.0);
//     cell = abs(cell - 0.5);
//     float distToCell = 1.0 - 2.0 * max(cell.x, cell.y);
//     colour = vec3(distToCell);

    // 6. Draw black lines on the edges with smoothstep
//     vec2 cell = fract(fragTexCoord * 10.0);
//     cell = abs(cell - 0.5);
//     float distToCell = 1.0 - 2.0 * max(cell.x, cell.y);
//     float cellLine = smoothstep(0.0, 0.05, distToCell);
//     colour = vec3(cellLine);

    // 7. We use the same trick as in previous lesson to draw lines with the base color
    // and we take into account the resolution. We also center the grid.
//     vec2 center = fragTexCoord - 0.5;
//     vec2 cell = fract(center * resolution / 60.0);
//     cell = abs(cell - 0.5);
//     float distToCell = 1.0 - 2.0 * max(cell.x, cell.y);
//     float cellLine = smoothstep(0.0, 0.05, distToCell);
//     colour = vec3(cellLine);

    // 8. We add axis and functions line
    vec2 center = fragTexCoord - 0.5;
    vec2 cell = fract(center * resolution / 60.0);
    cell = abs(cell - 0.5);
    float distToCell = 1.0 - 2.0 * max(cell.x, cell.y);

    float cellLine = smoothstep(0.0, 0.05, distToCell);
    float xAxis = smoothstep(0.0, 0.002, abs(fragTexCoord.x - 0.5));
    float yAxis = smoothstep(0.0, 0.002, abs(fragTexCoord.y - 0.5));

    vec2 pos = center * resolution / 60.0;
    float value0 = pos.x;
    float value1 = floor(pos.x);
    float functionLine0 = smoothstep(0.0, 0.075, abs(-pos.y - value0));
    float functionLine1 = smoothstep(0.0, 0.075, abs(-pos.y - value1));

    colour = mix(black, colour, cellLine);
    colour = mix(blue, colour, xAxis);
    colour = mix(blue, colour, yAxis);
    colour = mix(yellow, colour, functionLine0);
    colour = mix(red, colour, functionLine1);


    finalColor = vec4(colour, 1.0);
}

// Exercices:
// Try using the following functions: fract, ceil, round, floor and mod.
// Here ar two new functions to try:

// inverseLerp(current, min, max) {
//     return (current - min) / (max - min);
// }

// remap(current, inMin, inMax, outMin, outMax) {
//     float = inverseLerp(current, inMin, inMax);
//     return mix(outMin, outMax, t);
// }