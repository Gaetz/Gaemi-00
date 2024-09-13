#version 410

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;
uniform float time;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

float inverseLerp(float current, float min, float max) {
    return (current - min) / (max - min);
}

float remap(float current, float inMin, float inMax, float outMin, float outMax) {
    float t = inverseLerp(current, inMin, inMax);
    return mix(outMin, outMax, t);
}

mat3 rotateY(float radians) {
    float s = sin(radians);
    float c = cos(radians);

    return mat3(
    c, 0, -s,
    0, 1, 0,
    s, 0, c
    );
}

float easeOutBounce(float x) {
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (x < 1.0 / d1) {
        return n1 * x * x;
    } else if (x < 2.0 / d1) {
        return n1 * (x -= 1.5 / d1) * x + 0.75;
    } else if (x < 2.5 / d1) {
        return n1 * (x -= 2.25 / d1) * x + 0.9375;
    } else {
        return n1 * (x -= 2.625 / d1) * x + 0.984375;
    }
}

float easeInBounce(float x) {
    return 1.0 - easeOutBounce(1.0 - x);
}

float easeInOutBounce(float x) {
    if (x < 0.5) {
        return 0.5 * easeInBounce(x * 2.0);
    } else {
        return 0.5 * easeOutBounce(x * 2.0 - 1.0) + 0.5;
    }
}

void main()
{
    vec3 localPosition = vertexPosition;

    // Calculate fragment position based on model transformations
    fragPosition = localPosition;

    // 1. Scale + Rotation + Translation
    //    localPosition.xy *= remap(sin(time * 2.0), -1.0, 1.0, 0.6, 1.4);
    //    localPosition = rotateY(time) * localPosition;
    //    localPosition.x += sin(time);
    // 2. Ease in
    //    localPosition *= easeOutBounce(clamp(time - 1.0, 0.0, 1.0));
    // 3. Ripples
    float t = sin(localPosition.y * 20.0 + time * 5.0);
    t = remap(t, -1.0, 1.0, 0.0, 0.2);
    localPosition += vertexNormal * t;

    // Send vertex attributes to fragment shader
    fragPosition = (matModel*vec4(localPosition, 1.0)).xyz;
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragNormal = normalize((matNormal*vec4(vertexNormal, 1.0)).xyz);

    // Calculate final vertex position
    gl_Position = mvp*vec4(localPosition, 1.0);

    fragColor = mix(
        vec4(0.0, 0.0, 0.5, 1.0),
        vec4(0.1, 0.5, 0.8, 1.0),
        smoothstep(0.0, 0.2, t)
    );
}