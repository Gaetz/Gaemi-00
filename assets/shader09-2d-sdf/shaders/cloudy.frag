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

float sdfStar5(vec2 p, float r, float rf) {
    const vec2 k1 = vec2(0.809016994375, -0.587785252292);
    const vec2 k2 = vec2(-k1.x,k1.y);
    p.x = abs(p.x);
    p -= 2.0*max(dot(k1,p),0.0)*k1;
    p -= 2.0*max(dot(k2,p),0.0)*k2;
    p.x = abs(p.x);
    p.y -= r;
    vec2 ba = rf*vec2(-k1.y,k1.x) - vec2(0,1);
    float h = clamp( dot(p,ba)/dot(ba,ba), 0.0, r );
    return length(p-ba*h) * sign(p.y*ba.x-p.x*ba.y);
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

float saturate(float t) {
    return clamp(t, 0.0, 1.0);
}

float easeOut(float x, float p) {
    return 1.0 - pow(1.0 - x, p);
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

vec3 drawSimpleBackground() {
    return mix(vec3(0.42, 0.58, 0.75),
    vec3(0.36, 0.46, 0.82),
    smoothstep(0.0, 1.0, pow(fragTexCoord.x * fragTexCoord.y, 0.4)));
}

vec3 drawBackground(float dayTime) {
    // This background will chamge tone over time
    vec3 morning = mix(vec3(0.44, 0.64, 0.84),
    vec3(0.34, 0.51, 0.94),
    smoothstep(0.0, 1.0, pow(fragTexCoord.x * fragTexCoord.y, 0.4)));

    vec3 midday = mix(vec3(0.42, 0.58, 0.75),
    vec3(0.36, 0.46, 0.82),
    smoothstep(0.0, 1.0, pow(fragTexCoord.x * fragTexCoord.y, 0.4)));

    vec3 evening = mix(vec3(0.82, 0.51, 0.25),
    vec3(0.88, 0.71, 0.39),
    smoothstep(0.0, 1.0, pow(fragTexCoord.x * fragTexCoord.y, 0.4)));

    vec3 night = mix(vec3(0.07, 0.1, 0.19),
    vec3(0.19, 0.2, 0.29),
    smoothstep(0.0, 1.0, pow(fragTexCoord.x * fragTexCoord.y, 0.4)));

    const float dayDuration = 20.0;

    vec3 colour;
    if (dayTime < dayDuration * 0.25) {
        colour = mix(morning, midday, smoothstep(0.0, dayDuration * 0.25, dayTime));
    } else if (dayTime < dayDuration * 0.5) {
        colour = mix(midday, evening, smoothstep(dayDuration * 0.25, dayDuration * 0.5, dayTime));
    } else if (dayTime < dayDuration * 0.75) {
        colour = mix(evening, night, smoothstep(dayDuration * 0.5, dayDuration * 0.75, dayTime));
    } else {
        colour = mix(night, morning, smoothstep(dayDuration * 0.75, dayDuration, dayTime));
    }
    return colour;
}

float sdfCloud(vec2 pixelCoord) {
    float puff0 = sdfCircle(pixelCoord, 100.0);
    float puff1 = sdfCircle(pixelCoord - vec2(120.0, 10.0f), 80.0);
    float puff2 = sdfCircle(pixelCoord + vec2(120.0, -10.0f), 80.0);
    return opUnion(opUnion(puff0, puff1), puff2);
}

float sdfMoon(vec2 pixelCoord) {
    float d = opSubtraction(
    sdfCircle(pixelCoord + vec2(50.0, 0.0), 80.0),
    sdfCircle(pixelCoord, 80.0)
    );
    return d;
}


float hash(vec2 v) {
    float t = dot(v, vec2(36.5328, 73.945));
    return sin(t);
}

out vec4 finalColor;

void main() {
    float dayDuration = 20.0;
    float dayTime = mod(time, dayDuration);

    // 3. Day cycle
    vec3 colour = drawBackground(dayTime);

    // 4. Sun
    vec2 pixelCoord = fragTexCoord * resolution;

    if (dayTime < dayDuration * 0.75) {
        // Sun coming
        float t = saturate(inverseLerp(dayTime, 0.0, 1.0));
        vec2 offsetMoveIn = mix(vec2(0.0, -400.0), vec2(0.0), easeOut(t, 5.0));

        // Sun offset
        vec2 offset = vec2(150.0, resolution.y * 0.2) + offsetMoveIn;
        if (dayTime > dayDuration * 0.5) {
            // Sun going
            t = saturate(inverseLerp(dayTime, dayDuration * 0.5, dayDuration * 0.5 + 1.0));
            vec2 offsetMoveOut = mix(vec2(0.0), vec2(0.0, -400.0), t);
            offset = vec2(150.0, resolution.y * 0.2) + offsetMoveOut;
        }

        // Sun drawing
        vec2 sunPos = pixelCoord - offset;
        float sun = sdfCircle(sunPos, 75.0);
        colour = mix(vec3(0.84, 0.70, 0.2), colour, smoothstep(0.0, 2.0, sun));

        // Sun glow
        float s = max(0.001, sun);
        float p = saturate(exp(-0.001 * s * s));
        colour += 0.5 * mix(black, vec3(0.9, 0.85, 0.47), p);
    }

    // 5. Moon
    if (dayTime > dayDuration * 0.5) {
        // Moon coming
        float t = saturate(inverseLerp(dayTime, dayDuration * 0.5, dayDuration * 0.5 + 1.5));
        vec2 offsetMoveIn = mix(vec2(0.0, -400.0), vec2(0.0), easeOutBounce(t));

        // Moon offset
        vec2 offset = vec2(600.0, resolution.y * 0.2) + offsetMoveIn;
        if (dayTime > dayDuration * 0.9) {
            // Moon going
            t = saturate(inverseLerp(dayTime, dayDuration * 0.9, dayDuration * 0.95));
            vec2 offsetMoveOut = mix(vec2(0.0), vec2(0.0, -400.0), t);
            offset = vec2(600.0, resolution.y * 0.2) + offsetMoveOut;
        }

        // Moon drawing
        vec2 moonPos = pixelCoord - offset;
        moonPos = rotate2D(3.14159 * 0.2) * moonPos;

        float moon = sdfMoon(moonPos);
        colour = mix(white, colour, smoothstep(0.0, 2.0, moon));

        // Moon glow
        float moonGlow = sdfMoon(moonPos);
        colour += 0.2 * mix(white, black, smoothstep(-10.0, 15.0, moonGlow));
    }

    // 6. Stars with the moon
    const float NUM_STARS = 24.0;
    for (float i = 0; i < NUM_STARS; i+=1.0) {
        float hashSample = hash(vec2(i * 13.0)) * 0.5 + 0.5;

        float t = saturate(inverseLerp(dayTime + hashSample * 0.5, dayDuration * 0.5, dayDuration * 0.5 + 1.5));

        // Fade out
        float fade = 0.0;
        if (dayTime > dayDuration * 0.9) {
            fade = saturate(inverseLerp(dayTime - hashSample * 0.25, dayDuration * 0.9, dayDuration * 0.95));
        }

        float size = mix(2.0, 1.0, hash(vec2(i, i + 1.0)));
        vec2 offset = vec2(i * 100.0, 0.0) + 150.0 * hash(vec2(i));
        offset += mix(vec2(0.0, -400.0), vec2(0.0), easeOutBounce(t));

        vec2 pos = pixelCoord - offset;
        pos.x = mod(pos.x, resolution.x);
        pos = pos - resolution * vec2(0.5, 0.25);
        pos *= size;

        //float star = sdfCircle(pos, 10.0);
        float star = sdfStar5(pos, 10.0, 2.0);
        vec3 starColour = mix(white, colour, smoothstep(0.0, 2.0, star));
        starColour += mix(0.2, 0.0, pow(smoothstep(-5.0, 15.0, star), 0.25));
        colour = mix(starColour, colour, fade);
    }


    // 1. Circle with shadow
    //    vec3 colour = drawSimpleBackground();
    //    vec2 pixelCoord = (fragTexCoord - 0.5) * resolution;
    //    float cloudShadow = sdfCircle(pixelCoord + vec2(50.0, -50.0), 100.0);
    //    float cloud = sdfCircle(pixelCoord, 100.0);
    //    colour = mix(colour, vec3(0.0), 0.5 * smoothstep(0.0, -100.0, cloudShadow));
    //    colour = mix(vec3(1.0), colour, smoothstep(0.0, 1.0, cloud));

    // 2. Various moving clouds with shadows
    //vec3 colour = drawSimpleBackground();

    // Origin in not centered
    //    vec2 pixelCoord = fragTexCoord * resolution;

    const float NUM_CLOUDS = 8.0;
    for (float i = 0; i < NUM_CLOUDS; i+=1.0) {
        float size = mix(2.0, 1.0, i / NUM_CLOUDS + 0.1 * hash(vec2(i)));
        float speed = size * 0.25;
        vec2 offset = vec2(100.0 * i + time * 75.0 * speed, 200.0 * hash(vec2(i)));
        vec2 pos = pixelCoord - offset;

        // Wrap position then recenter the origin
        pos = mod(pos, resolution);
        pos = pos - 0.5 * resolution;

        // Draw clouds
        float cloudShadow = sdfCloud(pos * size + vec2(25.0, -25.0)) - 40.0;
        float cloud = sdfCloud(pos * size);
        colour = mix(colour, vec3(0.0), 0.5 * smoothstep(0.0, -100.0, cloudShadow));
        colour = mix(vec3(1.0), colour, smoothstep(0.0, 1.0, cloud));
    }

    finalColor = vec4(colour, 1.0);
}
