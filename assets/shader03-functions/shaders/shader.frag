#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

void main() {
    vec3 colour = vec3(1.0);

    float value0 = fragTexCoord.x;
    float value1 = smoothstep(0.0, 1.0, fragTexCoord.x);

    float line = smoothstep(0.0, 0.005, abs(fragTexCoord.y - 0.5));
    float linearLine = smoothstep(0.0, 0.005, abs(fragTexCoord.y - mix(0.5, 0.0, value0)));
    float smoothstepLine = smoothstep(0.0, 0.005, abs(fragTexCoord.y - mix(1.0, 0.5, value1)));

    vec3 red = vec3(1.0, 0.0, 0.0);
    vec3 blue = vec3(0.0, 0.0, 1.0);
    vec3 white = vec3(1.0, 1.0, 1.0);

    if (fragTexCoord.y < 0.5) {
        colour = mix(red, blue, value0);
    } else {
        colour = mix(red, blue, value1);
    }
    // Line is black on the line and white elsewhere
    // so we will draw white at the line location and colour elsewhere
    colour = mix(white, colour, line);
    colour = mix(white, colour, linearLine);
    colour = mix(white, colour, smoothstepLine);

    finalColor = vec4(colour, 1.0);
}

// Exercise 1. Three sections.
// Have 3 sections instead of two : step function, mix function and smoothstep function

// Exercise 2. Texture smoothstep.
// Load a texture and run the colours through smoothstep before displaying the texture


// varying vec2 vUvs;
//
// uniform sampler2D diffuse;
//
// void main(void) {
//   vec4 diffuseSample = texture2D(diffuse, vUvs);
//
//   gl_FragColor = smoothstep(vec4(0.0), vec4(1.0), diffuseSample);
// }


// Exercise 3. min, max, clamp.
// Change value1 to min(fragTexCoord.x, 0.25), max(fragTexCoord.x, 0.75), clamp(fragTexCoord.x, 0.25, 0.75).
// How clamp is linked to min and max ?

// Exercise 4. pow, parabolic.
// Change value1 to pow(fragTexCoord.x, 8.0), pow(fragTexCoord.x, 0.5).
// Change value1 to fragTexCoord.x * (1.0 - fragTexCoord.x) * 4.0.

