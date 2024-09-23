#version 330

in vec2 fragTexCoord;

uniform vec2 resolution;
uniform float time;

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

float sdfCircle(vec2 p, float radius) {
    return length(p) - radius;
}

float sdfLine(vec2 p, vec2 a, vec2 b) {
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h);
}

float sdfRect(vec2 p, vec2 size) {
    vec2 d = abs(p) - size;
    return length(max(d, vec2(0.0))) + min(max(d.x, d.y), 0.0);
}

mat2 rotate2D(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat2(c, -s, s, c);
}

float opUnion(float d1, float d2) {
    return min(d1, d2);
}

float opSubtraction(float d1, float d2) {
    return max(-d1, d2);
}

float opIntersection(float d1, float d2) {
    return max(d1, d2);
}

float softMax(float a, float b, float k) {
    return log(exp(k * a) + exp(k * b)) / k;
}

float softMin(float a, float b, float k) {
    return -softMax(-a, -b, k);
}

float softMinValue(float a, float b, float k) {
    //float h = remap(a - b, -1.0/k, 1.0/k, 0.0, 1.0);
    // Alternatively
    float h = exp(-b *k ) / (exp(-a * k) + exp(-b * k));
    return h;
}

out vec4 finalColor;

void main() {
    vec2 pixelCoord = (fragTexCoord - 0.5) * resolution;

    vec3 colour = drawBackground();
    colour = drawGrid(colour, grey, 10.0, 1.0);
    colour = drawGrid(colour, black, 100.0, 2.0);

    // 1. Shapes: circle, line, rectangle
    // More on: https://iquilezles.org/articles/distfunctions2d/
    //    float d = sdfCircle(pixelCoord, 100.0);
    //    colour = mix(red, colour, step(0.0, d));
    //    d = sdfLine(pixelCoord, vec2(-100.0, -50.0), vec2(200.0, 75.0));
    //    colour = mix(blue, colour, step(5.0, d));

    // 2. Rotating translated rectangle
    //    vec2 pos = pixelCoord - vec2(100.0, 200.0);
    //    pos *= rotate2D(time * 0.25);
    //    float d = sdfRect(pos, vec2(200.0, 25.0));
    //    colour = mix(yellow, colour, step(0.0, d));

    // 3. Same shapes with darker outline and antialiasing
    //    float d = sdfCircle(pixelCoord, 100.0);
    //    colour = mix(red * 0.5, colour, smoothstep(-1.0, 1.0, d));
    //    colour = mix(red, colour, smoothstep(-5.0, 0.0, d));
    //    d = sdfLine(pixelCoord, vec2(-100.0, -50.0), vec2(200.0, 75.0));
    //    colour = mix(blue * 0.5, colour, smoothstep(4.0, 5.0, d));
    //    vec2 pos = pixelCoord - vec2(100.0, 200.0);
    //    pos *= rotate2D(time * 0.25);
    //    d = sdfRect(pos, vec2(200.0, 25.0));
    //    colour = mix(yellow * 0.5, colour, smoothstep(-1.0, 1.0, d));
    //    colour = mix(yellow, colour, smoothstep(-5.0, 0.0, d));

    // 4. Boolean operations
    float box = sdfRect(rotate2D(time * 0.5) * pixelCoord, vec2(200.0, 100.0));
    float d1 = sdfCircle(pixelCoord - vec2(-200.0, -150.0), 150.0);
    float d2 = sdfCircle(pixelCoord - vec2(200.0, -150.0), 150.0);
    float d3 = sdfCircle(pixelCoord - vec2(0.0, 200.0), 150.0);
    float d = opUnion(opUnion(d1, d2), d3);
    // Also try with opUnion, opSubtraction, opIntersection
    //d = opUnion(box, d);

    // 5. Smooth unions and colors with softMin
    vec3 sdfColour = mix(red, blue, smoothstep(0.0, 1.0, softMinValue(box, d, 0.01)));

    d = softMin(box, d, 0.06);
    colour = mix(sdfColour * 0.5, colour, smoothstep(-1.0, 1.0, d));
    colour = mix(sdfColour, colour, smoothstep(-5.0, 0.0, d));

    finalColor = vec4(colour, 1.0);
}
