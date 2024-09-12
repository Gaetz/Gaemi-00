#version 330

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

void main()
{
    vec3 localPosition = vertexPosition;

    // Calculate fragment position based on model transformations
    fragPosition = localPosition;

    // 1. Scale + Rotation + Translation
    localPosition.xy *= remap(sin(time * 2.0), -1.0, 1.0, 0.6, 1.4);
    localPosition = rotateY(time) * localPosition;
    localPosition.x += sin(time);

    // Send vertex attributes to fragment shader
    fragPosition = (matModel*vec4(localPosition, 1.0)).xyz;
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragNormal = normalize((matNormal*vec4(vertexNormal, 1.0)).xyz);

    // Calculate final vertex position
    gl_Position = mvp*vec4(localPosition, 1.0);
}