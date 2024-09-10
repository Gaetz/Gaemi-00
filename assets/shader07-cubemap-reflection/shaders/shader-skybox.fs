#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform samplerCube environmentMap;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Fetch color from texture map
    vec3 colour = texture(environmentMap, fragPosition).rgb;

    // Gamma correction
    colour = colour/(colour + vec3(1.0));
    colour = pow(colour, vec3(1.0/2.2));


    // Calculate final fragment color
    finalColor = vec4(colour, 1.0);
}