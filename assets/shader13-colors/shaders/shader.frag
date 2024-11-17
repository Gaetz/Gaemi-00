#version 330

in vec2 fragTexCoord;

uniform vec2 resolution;
uniform float time;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

float inverseLerp(float current, float min, float max) {
    return (current - min) / (max - min);
}

float remap(float current, float inMin, float inMax, float outMin, float outMax) {
    float t = inverseLerp(current, inMin, inMax);
    return mix(outMin, outMax, t);
}

vec3 saturate(vec3 colour) {
    return clamp(colour, 0.0, 1.0);
}

float saturate(float value) {
    return clamp(value, 0.0, 1.0);
}

float colourDistance(vec3 c0, vec3 c1) {
    float rm = (c0.x + c1.x) * 0.5 * 256.0;
    vec3 d = (c0 - c1) * 256.0;

    float r = (2.0 + rm / 256.0) * d.x * d.x;
    float g = 4.0 * d.y * d.y;
    float b = (2.0 + (255.0 - rm) / 256.0) * d.z * d.z;
    return sqrt(r + g + b) / 256.0;
}

out vec4 finalColor;

void main() {
    // Two copies of the image in the same texture
    vec2 coords = fract(fragTexCoord * vec2(2.0, 1.0));
    // Center the copies
    coords.x = remap(coords.x, 0.0, 1.0, 0.25, 0.75);

    vec3 colour = texture(texture1, coords).rgb;

    // Manipulate second copy
    if (fragTexCoord.x > 0.5) {
        // Tinting
        //        vec3 tint = vec3(1.0, 0.5, 0.5);
        //        colour *= tint;

        // Brightness
        //        float brightnessAmount = 0.1;
        //        colour += brightnessAmount;

        // Saturation
        //        float luminance = dot(colour, vec3(0.2126, 0.7152, 0.0722));
        //        float saturationAmount = 2.0;
        //        colour = mix(vec3(luminance), colour, saturationAmount);

        // Contrast - Make black darker and white brighter
        //        float contrastAmount = 2.0;
        //        float midpoint = 0.5;
        // Anything that would push the values to the extremes
        //        colour = saturate((colour - midpoint) * contrastAmount + midpoint);
        //        colour = smoothstep(0.0, 1.0, colour);
        //        vec3 sg = sign(colour - midpoint);
        //        colour = sg * pow(abs(colour - midpoint) * 2.0, vec3(1.0 / contrastAmount)) * 0.5 + midpoint;

        // Color grading - The Matrix
        //        colour = pow(colour, vec3(1.5, 0.8, 1.5));

        // Colour boost with color distance
        //        float luminance = dot(colour, vec3(0.2126, 0.7152, 0.0722));
        //        vec3 boostedColour = vec3(0.72, 0.25, 0.25);
        //        float colourWeight = 1.0 - distance(colour, boostedColour); // Or use the colourDistance function
        //        colourWeight = smoothstep(0.2, 1.0, colourWeight); // Anything similar to constrast
        //        colour = mix(vec3(luminance), colour, colourWeight);

        // Colour boost with color direction dot product
        //        float luminance = dot(colour, vec3(0.2126, 0.7152, 0.0722));
        //        vec3 boostedColour = vec3(0.72, 0.25, 0.25);
        //        float colourWeight = dot(normalize(colour), normalize(boostedColour));
        //        colourWeight = pow(colourWeight, 32.0);
        //        colour = mix(vec3(luminance), colour, colourWeight);

        // Vignette with loaded image
        //        vec2 vignetteCoords = fract(fragTexCoord * vec2(2.0, 1.0));
        //        vec3 vignetteAmount = texture(texture2, vignetteCoords).rgb;
        //        colour *= vignetteAmount;

        // Procudural vignette
        //        vec2 vignetteCoords = fract(fragTexCoord * vec2(2.0, 1.0));
        //        float v0 = smoothstep(0.5, 0.2, abs(vignetteCoords.x - 0.5));
        //        float v1 = smoothstep(0.5, 0.2, abs(vignetteCoords.y - 0.5));
        //        float vignetteAmount = v0 * v1;
        //        vignetteAmount = pow(vignetteAmount, 0.25);
        //        vignetteAmount = remap(vignetteAmount, 0.0, 1.0, 0.5, 1.0);
        //        colour *= vignetteAmount;

        // Pixelation
        //        vec2 dims = vec2(128.0, 128.0);
        //        vec2 texUV = floor(coords * dims) / dims;
        //        vec3 pixelatedColour = texture(texture1, texUV).rgb;
        //        colour = pixelatedColour;

        // Distortion effect
        //        vec2 dims = vec2(128.0, 128.0);
        //        float distortionAmount = 0.7;
        //        vec2 texUV = floor(coords * dims) / dims;
        //        vec3 pixelatedColour = texture(texture1, texUV).rgb;
        //        vec2 pushedCoords = coords;
        //        float pushedSign = sign(pushedCoords.y - 0.5);
        //        pushedCoords.y = pushedSign * pow(abs(pushedCoords.y - 0.5) * 2.0, distortionAmount) * 0.5 + 0.5;
        //        colour = texture(texture1, pushedCoords).rgb;

        // Ripple effect
        vec2 dims = vec2(128.0, 128.0);
        vec2 texUV = floor(coords * dims) / dims;
        vec3 pixelatedColour = texture(texture1, texUV).rgb;
        float distToCenter = length(coords - 0.5);
        float d = sin(distToCenter * 50.0 - time * 2.0);
        vec2 dir = normalize(coords - 0.5);
        vec2 rippleCoords = coords + dir * d * 0.1;
        colour = texture(texture1, rippleCoords).rgb;
    }

    finalColor = vec4(colour, 1.0);
}
