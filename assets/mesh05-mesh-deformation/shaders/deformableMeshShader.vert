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
uniform vec3 hitPosition;
uniform vec3 hitNormal;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out vec4 modelPosition;
out vec4 modelNormal;

const float effectRadius = 5.0;

void main()
{
    fragPosition = (matModel*vec4(vertexPosition, 1.0)).xyz;
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragNormal = normalize((matNormal*vec4(vertexNormal, 1.0)).xyz);
    modelPosition = vec4(vertexPosition, 1.0);
    modelNormal = vec4(vertexNormal, 1.0);


    // Compute deformation
    vec3 trueHitPosition = (matModel * vec4(hitPosition.x, hitPosition.y, -hitPosition.z, 1.0)).xyz;
    vec3 vertexDeformation = vec3(0.0);
    float distance = length(trueHitPosition - vec3(vertexPosition.xy, -vertexPosition.z));
    if (distance < effectRadius)
    {
        float strength = 1.0 - distance/effectRadius;
        vertexDeformation = hitNormal * strength * -3.0;
    }

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition + vertexDeformation, 1.0);
}