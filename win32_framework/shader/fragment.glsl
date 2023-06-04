// Fragment shader by thuong.nv 2023.05.28
// TODO : + Rectangle bound
//        + Rectangle file

#version 330 core

in vec2 tex_coord;

uniform vec2 resolution;
uniform vec2 btn_size;

const vec4 v4borderColor = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 v4fillcolor = vec4(1.0, 0.0, 0.0, 1.0);
const float fborderthickness = 1.0;
const float radius = 10.0;

float RoundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius)
{
    return length(max(abs(CenterPosition)-Size+Radius,0.0))-Radius;
}

float RectSDF(vec2 p, vec2 b, float r)
{
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

void main() 
{
    float edgeSoftness = 1.0f;

    vec2 pos = btn_size * tex_coord;

    float fDist = RectSDF(pos-btn_size/2.0, btn_size/2.0 - fborderthickness/2.0-1.0, radius);
    float fBlendAmount = smoothstep(-1, 1.0, abs(fDist) - fborderthickness / 2.0);

    vec4 v4FromColor = v4borderColor;
    vec4 v4ToColor = (fDist < 0.0) ? v4fillcolor : vec4(0.0);
    gl_FragColor = mix(v4FromColor, v4ToColor, fBlendAmount );
}
