// Fragment shader by thuong.nv 2023.05.28
// TODO : + Rectangle bound
//        + Rectangle file

#version 330 core

in vec2 tex_coord;

uniform vec2 resolution;

const vec4 v4borderColor = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 v4fillcolor = vec4(1.0, 0.0, 0.0, 1.0);
const float fborderthickness = 6.0;
const float radius = 60.0;

float RectSDF(vec2 p, vec2 b, float r)
{
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

void main() 
{
    vec2 pos = resolution * tex_coord;

    float fDist = RectSDF(pos-resolution/2.0, resolution/2.0 - fborderthickness/2.0-1.0, radius);
    float fBlendAmount = smoothstep(-1.0, 1.0, abs(fDist) - fborderthickness / 2.0);

    vec4 v4FromColor = v4borderColor;
    vec4 v4ToColor = (fDist < 0.0) ? v4fillcolor : vec4(0.0);
    gl_FragColor = mix(v4FromColor, v4ToColor, fBlendAmount);
}

//float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius)
//{
//    return length(max(abs(CenterPosition)-Size+Radius,0.0))-Radius;
//}
//
//void main()
//{
//    vec2 pos = resolution * tex_coord;
//    float thickness      = 10.0;
//    float shadowSoftness = 80.0f;
//    vec2  shadowOffset   = vec2(10.0, -10.0);
//    float edgeSoftness   = 1.0;
//
//    float _distance       = roundedBoxSDF(pos, resolution/2.0, radius);
//    float smoothedAlpha  = 1.0 - smoothstep(-edgeSoftness, edgeSoftness, abs(_distance) - thickness);
//    vec4  quadColor      = mix(vec4(1.0), vec4(0.0, 0.2, 1.0, smoothedAlpha), smoothedAlpha);
//    
//    float shadowDistance = roundedBoxSDF(pos + shadowOffset, resolution/2.0, radius);
//    float shadowAlpha    = 1.0 - smoothstep(-shadowSoftness/2.0, shadowSoftness/2.0, abs(shadowDistance));
//    vec4 shadowColor     = vec4(0.4, 0.4, 0.4, 1.0);
//    gl_FragColor            = mix(quadColor, shadowColor, shadowAlpha - smoothedAlpha);
//}