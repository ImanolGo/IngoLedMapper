from protopixel import Content
from openframeworks import *
import os.path
from tempfile import mkdtemp
import numpy as np

class Clouds:

    def __init__(self, width, height):

        self.width = width
        self.height = height
        self.shader = ofShader()
        self.fbo = ofFbo()
        self.fbo.allocate(width,height)
        self.currentAlpha = 1.0
        self.targetAlpha = 1.0
        self.skycolour = ofColor(36, 140, 255)
        self.clouds_color = ofColor(255, 255, 255)
        self.speed = 0.04
        self.number = 0.8
        self.contrast = 0.6
        self.saturation = 1.0
        self.brightness = 1.2
        self.gamma = 1.0
        self.period = 90
        self.elapsedTime = 0.0
        self.one_day_in_seconds = 60*60*24
        
        self.setup()


    def setup(self):
        self.setupShader()

    def update(self):
        self.updateTime()
        self.updateNumber()
        self.updateAlpha()
        self.updateFbo()

    def translate(self, value, leftMin, leftMax, rightMin, rightMax):
        leftSpan = leftMax - leftMin
        rightSpan = rightMax - rightMin
        valueScaled = float(value -leftMin)/float(leftSpan)
        return rightMin + valueScaled*rightSpan

    def updateTime(self):
        self.elapsedTime += ofGetLastFrameTime()
        if self.elapsedTime > self.one_day_in_seconds:
            self.elapsedTime-= self.one_day_in_seconds

    def updateNumber(self):
        angle =  np.sin(self.elapsedTime*np.pi/self.period)
        self.number = self.translate(angle, -1.0,1.0, 0.6, 1.5)
        self.speed =  self.translate(angle, -1.0,1.0, 0.01, 0.04)

    def updateAlpha(self):
        self.currentAlpha = self.currentAlpha + (self.targetAlpha - self.currentAlpha)*0.02

    def updateFbo(self):
        self.fbo.begin()
        ofClear(0)
        ofSetColor(255)
        self.drawShader()
        self.fbo.end()

    def draw(self):

        self.fbo.draw(0,0)
        #self.drawShader()

    def drawShader(self):

        if self.shader.isLoaded():

            rC = self.clouds_color.r/255.0
            gC = self.clouds_color.g/255.0
            bC = self.clouds_color.b/255.0

            rS = self.skycolour.r/255.0
            gS = self.skycolour.g/255.0
            bS = self.skycolour.b/255.0


            self.shader.begin()
            self.shader.setUniform1f('alpha',  self.currentAlpha)
            self.shader.setUniform1f('iTime', self.elapsedTime*self.speed)
            self.shader.setUniform3f('iResolution', float(self.width), float(self.height),0.0)
            self.shader.setUniform1f('cloudcover', self.number)
            self.shader.setUniform1f('contrast',  self.contrast)
            self.shader.setUniform1f('brightness', self.brightness)
            self.shader.setUniform1f('saturation', self.saturation)
            self.shader.setUniform1f('gamma', self.gamma)
            self.shader.setUniform3f('skycolour1', rS - 0.5,gS- 0.5,bS- 0.5)
            self.shader.setUniform3f('skycolour2', rS,gS,bS)
            self.shader.setUniform3f('iColor', rC,gC,bC)
            ofDrawRectangle(-self.width/2.,-self.height/2.,self.width,self.height)
            #self.fbo.draw(0,0)
       
            self.shader.end()        

    def setAlpha(self, alpha):
        self.targetAlpha = alpha

    def setupShader(self):

        temp_dir = mkdtemp()
        frag_file = os.path.join(temp_dir,'s.frag')
        vert_file = os.path.join(temp_dir,'s.vert')
        shader_file_of = os.path.join(temp_dir,'s')

        self.vert_contents = """
        #version 150

        in vec4 position;
        out vec4 position_frag;

        void main() {
            gl_Position = position;
            position_frag = position;
        }
        """


        self.frag_contents_prefix = """
            #version 150
            out vec4 outputColor;
            uniform vec3 iResolution;
            uniform float iTime;

            uniform float contrast = 1.0;
            uniform float saturation = 1.0;
            uniform float brightness = 1.0;

            uniform float gamma = 1.0;
            uniform float minInput = 0.0;
            uniform float maxInput = 1.0;
            uniform float minOutput = 0.0;
            uniform float maxOutput = 1.0;

            in vec4 position_frag;
          """


        self.frag_contents = """
        
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
            uniform float alpha = 1.0;

            const mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );

            uniform float cloudcover = 0.1;
            uniform vec3 iColor = vec3(1.0,1.0,1.0);


            #define GammaCorrection(color, gamma)                               vec3( pow(color.r, 1.0 / gamma), pow(color.g, 1.0 / gamma), pow(color.b, 1.0 / gamma) )
            #define LevelsControlInputRange(color, minInput, maxInput)              min(max(color - vec3(minInput), vec3(0.0)) / (vec3(maxInput) - vec3(minInput)), vec3(1.0))
            #define LevelsControlInput(color, minInput, gamma, maxInput)                GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma)
            #define LevelsControlOutputRange(color, minOutput, maxOutput)           mix(vec3(minOutput), vec3(maxOutput), color)
            #define LevelsControl(color, minInput, gamma, maxInput, minOutput, maxOutput)   LevelsControlOutputRange(LevelsControlInput(color, minInput, gamma, maxInput), minOutput, maxOutput)

            vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)
            {
                // Increase or decrease theese values to adjust r, g and b color channels seperately
                const float AvgLumR = 0.5;
                const float AvgLumG = 0.5;
                const float AvgLumB = 0.5;
                
                const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
                
                vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
                vec3 brtColor = color * brt;
                vec3 intensity = vec3(dot(brtColor, LumCoeff));
                vec3 satColor = mix(intensity, brtColor, sat);
                vec3 conColor = mix(AvgLumin, satColor, con);
                return conColor;
            }


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
                result =  ContrastSaturationBrightness(result, brightness, saturation, contrast);
                result = LevelsControl(result, minInput, gamma, maxInput, minOutput, maxOutput);

                fragColor =  vec4(result,alpha);
            }


        """

        self.frag_contents_suffix = """
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

        with open(frag_file,'w') as f:
            f.write(self.frag_contents_prefix)
            f.write(self.frag_contents)
            f.write(self.frag_contents_suffix)
        with open(vert_file,'w') as f:
            f.write(self.vert_contents)
        self.shader.load(shader_file_of)

