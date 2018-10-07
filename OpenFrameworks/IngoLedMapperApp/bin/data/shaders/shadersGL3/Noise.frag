

uniform float inoise_grain;
uniform vec3 iColor = vec3(1.0,1.0,1.0);

float hash(vec2 p)
{
    vec3 p3  = fract(vec3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

float ang(vec2 uv, vec2 center){
    return atan((uv.y-center.y),(uv.x-center.x));
}

float spir(vec2 uv, vec2 loc){
    float dist1=length(uv-loc);
    float dist2=dist1*dist1;
    float layer6=sin((ang(uv,loc)+dist2-iTime)*6.0);
    layer6 = layer6*dist1;
    return layer6;
}

float ripl(vec2 uv, vec2 loc, float speed, float frequency){
    return sin(iTime*speed-length(uv-loc)*frequency);
}

float height(in vec2 uv){
    float layer1=sin(iTime*8.54-inoise_grain*sin(length(uv-vec2(-0.41,-0.47)))*55.0);
    float layer2=sin(iTime*7.13-inoise_grain*sin(length(uv-vec2(1.35,1.32)))*43.0);
    float layer3=sin(iTime*7.92-inoise_grain*sin(length(uv-vec2(-0.34,1.28)))*42.5);
    float layer4=sin(iTime*6.71-inoise_grain*sin(length(uv-vec2(1.23,-0.24)))*47.2);

    float spiral=spir(uv,vec2(0.5,0.5));
    spiral*=3.0;
    
    float temp = layer1+layer2+layer3+layer4+spiral;
    
    float b=smoothstep(-1.5,7.0,temp);
    return b*2.0;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv=fragCoord.xy/iResolution.x;
    
    float waveHeight=0.02+height(uv);
    
    vec3 color=vec3(waveHeight*iColor.r,waveHeight*iColor.g,waveHeight*iColor.b);

	fragColor =  vec4(color,1.0);
}