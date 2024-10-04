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

// Random function by Inigo Quilez
float easyRandom(vec2 p) {
    p = 50.0 * fract(p * 0.3183099 + vec2(0.71, 0.113));
    return -1.0 + 2.0 * fract(p.x * p.y * (p.x + p.y));
}

// Random function by Inigo Quilez
vec3 easyRandom3D(vec3 p) {
    p = vec3( dot(p,vec3(127.1,311.7,74.7)),
    dot(p,vec3(269.5,183.3,246.1)),
    dot(p,vec3(113.5,271.9,124.6)));

    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

vec4 noise(vec2 coords) {
    vec2 size = vec2(1.0);
    vec2 pc = coords * size;
    vec2 base = floor(pc);

    float s1 = easyRandom((base + vec2(0.0, 0.0)) / size);
    float s2 = easyRandom((base + vec2(1.0, 0.0)) / size);
    float s3 = easyRandom((base + vec2(0.0, 1.0)) / size);
    float s4 = easyRandom((base + vec2(1.0, 1.0)) / size);

    vec2 f = smoothstep(0.0, 1.0, fract(pc));

    float px1 = mix(s1, s2, f.x);
    float px2 = mix(s3, s4, f.x);
    float result = mix(px1, px2, f.y);
    return vec4(result);
}

float noise3D(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);

    vec3 u = f * f * (3.0 - 2.0 * f);
    float result = mix(mix(mix(dot(easyRandom3D(i + vec3(0.0, 0.0, 0.0)), f - vec3(0.0, 0.0, 0.0)),
                               dot(easyRandom3D(i + vec3(1.0, 0.0, 0.0)), f - vec3(1.0, 0.0, 0.0)), u.x),
                           mix(dot(easyRandom3D(i + vec3(0.0, 1.0, 0.0)), f - vec3(0.0, 1.0, 0.0)),
                               dot(easyRandom3D(i + vec3(1.0, 1.0, 0.0)), f - vec3(1.0, 1.0, 0.0)), u.x), u.y),
                       mix(mix(dot(easyRandom3D(i + vec3(0.0, 0.0, 1.0)), f - vec3(0.0, 0.0, 1.0)),
                               dot(easyRandom3D(i + vec3(1.0, 0.0, 1.0)), f - vec3(1.0, 0.0, 1.0)), u.x),
                           mix(dot(easyRandom3D(i + vec3(0.0, 1.0, 1.0)), f - vec3(0.0, 1.0, 1.0)),
                               dot(easyRandom3D(i + vec3(1.0, 1.0, 1.0)), f - vec3(1.0, 1.0, 1.0)), u.x), u.y), u.z);
    return result;
}

// Simplex noise by Stefan Gustavson

vec3 mod289(vec3 x) {
    return x - floor(x / 289.0) * 289.0;
}

vec4 mod289(vec4 x) {
    return x - floor(x / 289.0) * 289.0;
}

vec4 permute(vec4 x) {
    return mod289((x * 34.0 + 1.0) * x);
}

vec4 taylorInvSqrt(vec4 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}

vec4 simplexNoise(vec3 v) {
    const vec2 C = vec2(1.0 / 6.0, 1.0 / 3.0);

    // First corner
    vec3 i  = floor(v + dot(v, vec3(C.y)));
    vec3 x0 = v   - i + dot(i, vec3(C.x));

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min(g.xyz, l.zxy);
    vec3 i2 = max(g.xyz, l.zxy);

    vec3 x1 = x0 - i1 + C.x;
    vec3 x2 = x0 - i2 + C.y;
    vec3 x3 = x0 - 0.5;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    vec4 p =
    permute(permute(permute(i.z + vec4(0.0, i1.z, i2.z, 1.0))
                    + i.y + vec4(0.0, i1.y, i2.y, 1.0))
            + i.x + vec4(0.0, i1.x, i2.x, 1.0));

    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    vec4 j = p - 49.0 * floor(p / 49.0);  // mod(p,7*7)

    vec4 x_ = floor(j / 7.0);
    vec4 y_ = floor(j - 7.0 * x_);

    vec4 x = (x_ * 2.0 + 0.5) / 7.0 - 1.0;
    vec4 y = (y_ * 2.0 + 0.5) / 7.0 - 1.0;

    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4(x.xy, y.xy);
    vec4 b1 = vec4(x.zw, y.zw);

    vec4 s0 = floor(b0) * 2.0 + 1.0;
    vec4 s1 = floor(b1) * 2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

    vec3 g0 = vec3(a0.xy, h.x);
    vec3 g1 = vec3(a0.zw, h.y);
    vec3 g2 = vec3(a1.xy, h.z);
    vec3 g3 = vec3(a1.zw, h.w);

    // Normalize gradients
    vec4 norm = taylorInvSqrt(vec4(dot(g0, g0), dot(g1, g1), dot(g2, g2), dot(g3, g3)));
    g0 *= norm.x;
    g1 *= norm.y;
    g2 *= norm.z;
    g3 *= norm.w;

    // Compute noise and gradient at P
    vec4 m = max(0.6 - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    vec4 m2 = m * m;
    vec4 m3 = m2 * m;
    vec4 m4 = m2 * m2;
    vec3 grad =
    -6.0 * m3.x * x0 * dot(x0, g0) + m4.x * g0 +
    -6.0 * m3.y * x1 * dot(x1, g1) + m4.y * g1 +
    -6.0 * m3.z * x2 * dot(x2, g2) + m4.z * g2 +
    -6.0 * m3.w * x3 * dot(x3, g3) + m4.w * g3;
    vec4 px = vec4(dot(x0, g0), dot(x1, g1), dot(x2, g2), dot(x3, g3));
    return 42.0 * vec4(grad, dot(m4, px));
}

float fbm(vec3 p, int octaves, float persistence, float lacunarity, float exponentiation) {
    float amplitude = 0.5;
    float frequency = 1.0;
    float total = 0.0;
    float normalization = 0.0;

    for (int i = 0; i < octaves; ++i) {
        float noiseValue = simplexNoise(p * frequency).w;
        total += noiseValue * amplitude;
        normalization += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;
    total = total * 0.5 + 0.5;
    total = pow(total, exponentiation);

    return total;
}

float map(vec3 pos) {
    return fbm(pos, 6, 0.5, 2.0, 4.0);
}

vec3 calcNormal(vec3 pos, vec3 n) {
    vec2 e = vec2(0.0001, 0.0);
    return normalize(
        n + -500.0 * vec3(
        map(pos + e.xyy) - map(pos - e.xyy),
        map(pos + e.yxy) - map(pos - e.yxy),
        map(pos + e.yyx) - map(pos - e.yyx)
        )
    );
}

mat3 rotateY(float radians) {
    float s = sin(radians);
    float c = cos(radians);
    return mat3(
    c, 0.0, s,
    0.0, 1.0, 0.0,
    -s, 0.0, c);
}

out vec4 finalColor;

vec3 generateGridStars(vec2 pixelCoords, float starRadius, float cellSize, float seed, bool twinkle) {
    vec2 cellCoords = (fract(pixelCoords / cellSize) - 0.5) * cellSize;
    vec2 cellID = floor(pixelCoords / cellSize) + seed / 100.0;
    vec3 cellHashValue = easyRandom3D(vec3(cellID, 0.0));

    float starBrightness = saturate(cellHashValue.z);
    vec2 starPosition = vec2(0.0);
    starPosition += cellHashValue.xy * (cellSize * 0.5 - starRadius * 4.0);
    float distToStar = length(cellCoords - starPosition);
    float glow = exp(-2.0 * distToStar / starRadius);

    if (twinkle) {
        float noiseSample = noise3D(vec3(cellID, time * 1.5));
        float twinkleSize = (remap(noiseSample, -1.0, 1.0, 1.0, 0.1) * starRadius * 6.0);
        vec2 absDist = abs(cellCoords - starPosition);
        float twinkleValue = smoothstep(starRadius * 0.25, 0.0, absDist.y) * smoothstep(twinkleSize, 0.0, absDist.x);
        twinkleValue += smoothstep(starRadius * 0.25, 0.0, absDist.x) * smoothstep(twinkleSize, 0.0, absDist.y);
        glow += twinkleValue;
    }

    return vec3(glow * starBrightness);
}

vec3 generateStars(vec2 pixelCoords) {
    vec3 stars = vec3(0.0);
    float size = 4.0;
    float cellSize = 500.0;
    for (float i = 0.0; i <= 2.0; i++) {
        stars += generateGridStars(pixelCoords, size, cellSize, i, true);
        size *= 0.5;
        cellSize *= 0.35;
    }

    for (float i = 3.0; i < 5.0; i++) {
        stars += generateGridStars(pixelCoords, size, cellSize, i, false);
        size *= 0.5;
        cellSize *= 0.35;
    }

    return stars;
}

float sdfCircle(vec2 p, float radius) {
    return length(p) - radius;
}

vec3 drawPlanet(vec2 pixelCoords, vec3 colour) {
    float d = sdfCircle(pixelCoords, 300.0);

    vec3 planetColour = vec3(0.0, 0.0, 1.0);
    if (d <= 0.0) {
        float x = pixelCoords.x / 300.0;
        float y = pixelCoords.y / 300.0;
        float z = sqrt(1.0 - x * x - y * y);
        mat3 planetRotation = rotateY(time * 0.1);
        vec3 viewNormal = vec3(x, y, z);
        vec3 wsPosition = planetRotation * viewNormal;
        vec3 wsNormal = planetRotation * normalize(wsPosition);
        vec3 wsViewDir = vec3(0.0, 0.0, 1.0);


        vec3 noiseCoord = wsPosition * 2.0;
        float noiseSample = fbm(noiseCoord, 6, 0.5, 2.0, 4.0);
        float moistureMap = fbm(noiseCoord * 0.5 + vec3(20.0), 2, 0.5, 2.0, 1.0);

        // Colour
        vec3 waterColour = mix(
            vec3(0.01, 0.09, 0.55), // Ocean blue
            vec3(0.09, 0.26, 0.57), // Shore blue
            smoothstep(0.02, 0.06, noiseSample)
        );
        vec3 landColour = mix(
            vec3(0.5, 1.0, 0.3), // Shore green
            vec3(0.0, 0.7, 0.1), // Forest green
            smoothstep(0.05, 0.1, noiseSample)
        );
        landColour = mix(
            vec3(1.0, 1.0, 0.5), // Desert color
            landColour,
            smoothstep(0.4, 0.5, moistureMap)
        );
        landColour = mix(landColour, vec3(0.5, 0.6, 0.5), smoothstep(0.1, 0.2, noiseSample)); // Mountains
        landColour = mix(landColour, vec3(0.9, 0.9, 1.0), smoothstep(0.2, 0.3, noiseSample)); // Ice on tops
        landColour = mix(landColour, vec3(0.9, 0.9, 1.0), smoothstep(0.6, 0.9, abs(viewNormal.y))); // Poles

        planetColour = mix(waterColour, landColour, smoothstep(0.05, 0.06, noiseSample));

        // Lighting
        vec3 wsLightDir = planetRotation * normalize(vec3(0.5, 1.0, 0.5));
        vec3 wsSurfaceNormal = calcNormal(noiseCoord, wsNormal);
        // Different specular values for lands and oceans
        vec2 specParams = mix(
            vec2(0.5, 32.0),
            vec2(0.01, 2.0),
            smoothstep(0.05, 0.06, noiseSample)
        );
        // Wrap: light goes a little further, simulate subsurface scattering
        float wrap = 0.05;
        float dp = max(0.0, (dot(wsLightDir, wsSurfaceNormal) + wrap) / (1.0 + wrap));

        // Dark orange light on fringes
        vec3 lightColour = mix(
            vec3(0.25, 0.05, 0.0),
            vec3(0.75),
            smoothstep(0.05, 0.5, dp));

        vec3 ambient = vec3(0.002);
        vec3 diffuse = lightColour * dp;

        vec3 r = normalize(reflect(-wsLightDir, wsSurfaceNormal));
        float phongValue = max(0.0, dot(wsViewDir, r));
        phongValue = pow(phongValue, specParams.y);

        vec3 specular = vec3(phongValue) * specParams.x * diffuse;

        vec3 planetShading = planetColour * (diffuse + ambient) + specular;
        planetColour = planetShading;

        // Fresnel
        float fresnel = smoothstep(1.0, 0.1, viewNormal.z);
        fresnel = pow(fresnel, 8.0) * dp;
        planetColour = mix(planetColour, vec3(0.0, 0.5, 1.0), fresnel);
    }

    colour = mix(colour, planetColour, smoothstep(0.0, -1.0, d));

    // Planet glow
    if (d < 40.0 && d >= -1.0) {
        float x = pixelCoords.x / 440.0;
        float y = pixelCoords.y / 440.0;
        float z = sqrt(1.0 - x * x - y * y);
        vec3 normal = vec3(x, y, z);

        float lighting = dot(normal, normalize(vec3(0.5, 1.0, 0.5)));
        lighting = smoothstep(-0.15, 1.0, lighting);

        vec3 glowColour = vec3(0.05, 0.3, 0.9) *
        exp(-0.01 * d * d) * lighting * 0.75;
        colour += glowColour;
    }

    return colour;
}

void main() {
    vec2 pixelCoords = vec2((fragTexCoord.x - 0.5) * resolution.x, -(fragTexCoord.y - 0.5) * resolution.y);
    vec3 colour = vec3(0.0);

    colour = generateStars(pixelCoords);
    colour = drawPlanet(pixelCoords, colour);

    colour = pow(colour, vec3(1.0 / 2.2));
    finalColor = vec4(colour, 1.0);
}
