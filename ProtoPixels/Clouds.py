from protopixel import Content
from openframeworks import *

import os.path
from tempfile import mkdtemp

content = Content("Clouds")

width = 100
height = 600
content.FBO_SIZE = (width,height)
shaderfile = content.add_asset('shader')
shader = ofShader()

temp_dir = mkdtemp()
frag_file = os.path.join(temp_dir,'s.frag')
vert_file = os.path.join(temp_dir,'s.vert')
shader_file_of = os.path.join(temp_dir,'s')

content.add_parameter("sky_color", type="color", value=ofColor(20, 200, 255))
content.add_parameter("clouds_color", type="color", value=ofColor(255, 255, 255))
content.add_parameter("speed", min=0.0, max=1.0, value=0.8)
content.add_parameter("number", min=0.0, max=2.0, value=0.8)

def setup():
    if content['shader path']:
        shader_path_changed(content['shader path'])

    with open(frag_file,'w') as f:
        f.write(frag_contents_prefix)
        f.write(frag_contents)
        f.write(frag_contents_suffix)
    with open(vert_file,'w') as f:
        f.write(vert_contents)
    shader.load(shader_file_of)

def draw():

    rC = content['clouds_color'].r/255.0
    gC = content['clouds_color'].g/255.0
    bC = content['clouds_color'].b/255.0

    rS = content['sky_color'].r/255.0
    gS = content['sky_color'].g/255.0
    bS = content['sky_color'].b/255.0

    if shader.isLoaded():
        shader.begin()
        shader.setUniform3f('iResolution', float(content.FBO_SIZE[0]), float(content.FBO_SIZE[1]),0.0)
        shader.setUniform1f('iTime', ofGetElapsedTimef()*content['speed']*0.1)
        shader.setUniform1f('cloudcover', content['number'])
        shader.setUniform3f('skycolour1', rS,gS,bS)
        shader.setUniform3f('skycolour2', rS,gS,bS)
        shader.setUniform3f('iColor', rC,gC,bC)
        ofDrawRectangle(-width/2.,-height/2.,width,height)
        shader.end()

@content.parameter_changed('shader path')
def shader_path_changed(p):
    print p
    frag_contents = open(p).read()
    with open(frag_file,'w') as f:
        f.write(frag_contents_prefix)
        f.write(frag_contents)
        f.write(frag_contents_suffix)
    with open(vert_file,'w') as f:
        f.write(vert_contents)
    shader.load(shader_file_of)


vert_contents = """
#version 150

in vec4 position;
out vec4 position_frag;

void main() {
    gl_Position = position;
    position_frag = position;
}
"""

frag_contents_prefix = """
#version 150
out vec4 outputColor;
uniform vec3 iResolution;
uniform float iTime;

in vec4 position_frag;
"""

frag_contents = """
// This code can be found in 
// https://www.shadertoy.com/view/MdXGDH
// and it's property of its creator.
// This is distributed for illustration purposes only.


const float cloudscale = 1.1;
//const float speed = 0.03;
const float clouddark = 0.5;
const float cloudlight = 0.3;
//const float cloudcover = 0.2;
const float cloudalpha = 8.0;
const float skytint = 0.5;
uniform vec3 skycolour1 = vec3(0.0, 0.0, 0.0);
uniform vec3 skycolour2 = vec3(0.0, 0.0, 0.0);

const mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );

uniform float cloudcover = 0.1;
uniform vec3 iColor = vec3(1.0,1.0,1.0);



vec2 hash( vec2 p ) {
    p = vec2(dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)));
    return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p ) {
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;
    vec2 i = floor(p + (p.x+p.y)*K1);   
    vec2 a = p - i + (i.x+i.y)*K2;
    vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0); //vec2 of = 0.5 + 0.5*vec2(sign(a.x-a.y), sign(a.y-a.x));
    vec2 b = a - o + K2;
    vec2 c = a - 1.0 + 2.0*K2;
    vec3 h = max(0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
    vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    return dot(n, vec3(70.0));  
}

float fbm(vec2 n) {
    float total = 0.0, amplitude = 0.1;
    for (int i = 0; i < 7; i++) {
        total += noise(n) * amplitude;
        n = m * n;
        amplitude *= 0.4;
    }
    return total;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{

    vec2 p = fragCoord.xy / iResolution.xy;
    vec2 uv = p*vec2(iResolution.x/iResolution.y,1.0);    
    float time = iTime ;
    float q = fbm(uv * cloudscale * 0.5);
    
    //ridged noise shape
    float r = 0.0;
    uv *= cloudscale;
    uv -= q - time;
    float weight = 0.8;
    for (int i=0; i<8; i++){
        r += abs(weight*noise( uv ));
        uv = m*uv + time;
        weight *= 0.7;
    }
    
    //noise shape
    float f = 0.0;
    uv = p*vec2(iResolution.x/iResolution.y,1.0);
    uv *= cloudscale;
    uv -= q - time;
    weight = 0.7;
    for (int i=0; i<8; i++){
        f += weight*noise( uv );
        uv = m*uv + time;
        weight *= 0.6;
    }
    
    f *= r + f;
    
    //noise colour
    float c = 0.0;
    time = iTime;
    uv = p*vec2(iResolution.x/iResolution.y,1.0);
    uv *= cloudscale*2.0;
    uv -= q - time;
    weight = 0.4;
    for (int i=0; i<7; i++){
        c += weight*noise( uv );
        uv = m*uv + time;
        weight *= 0.6;
    }
    
    //noise ridge colour
    float c1 = 0.0;
    time = iTime;
    uv = p*vec2(iResolution.x/iResolution.y,1.0);
    uv *= cloudscale*3.0;
    uv -= q - time;
    weight = 0.4;
    for (int i=0; i<7; i++){
        c1 += abs(weight*noise( uv ));
        uv = m*uv + time;
        weight *= 0.6;
    }
    
    c += c1;
    
    vec3 skycolour = mix(skycolour2, skycolour1, p.y);
    vec3 cloudcolour = vec3(1.0, 1.0, 1.0) * clamp((clouddark + cloudlight*c), 0.0, 1.0);
   
    f = cloudcover + cloudalpha*f*r;
    
    vec3 result = mix(skycolour, clamp(skytint * skycolour + cloudcolour, 0.0, 1.0), clamp(cloudcover*(f + c), 0.0, 1.0));

    result = vec3(result.r*iColor.r, result.g*iColor.g, result.b*iColor.b);
    fragColor =  vec4(result,1.0);
}


"""

frag_contents_suffix = """
void main()
{
    vec2 pos = position_frag.xy;
    pos.x /= 2.0;
    pos.y /= 2.0;
    pos.x += 0.5;
    pos.y += 0.5;
    pos.x *= iResolution.x;
    pos.y *= iResolution.y;
    mainImage( outputColor, pos);
}
"""