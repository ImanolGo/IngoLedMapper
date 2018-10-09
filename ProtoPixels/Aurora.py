from protopixel import Content
from openframeworks import *
import os.path
from tempfile import mkdtemp
import numpy as np

class Aurora:

    def __init__(self, width, height):

        self.width = width
        self.height = height
        self.shader = ofShader()
        self.fbo = ofFbo()
        self.fbo2 = ofFbo()
        self.fbo.allocate(width,height)
        self.fbo2.allocate(width,height)
        self.currentAlpha = 1.0
        self.targetAlpha = 1.0
        self.skycolour = ofColor(36, 140, 255)
        self.clouds_color = ofColor(255, 255, 255)
        self.speed = 0.1
        self.number = 0.8
        self.contrast = 1.0
        self.saturation = 1.0
        self.brightness = 1.0
        self.gamma = 1.0
        self.period = 10
        
        self.setup()


    def setup(self):
        self.setupTextures()
        self.setupShader()
        self.setupShaderNoise()

    def setupTextures(self):
        content = Content("Aurora")

        ch1_file = content.add_asset('iChannel0.png')
        ch2_file = content.add_asset('iChannel1.png')
        self.img = ofImage(ch1_file)
        self.img2 = ofImage(ch2_file)
        self.fbo2 = ofFbo()
        self.fbo2.allocate(self.width, self.height)
        self.fbo2.begin()
        self.img2.draw(0,0,self.width, self.height)
        self.fbo2.end()


       #  img.load(ch1_file)
       #  self.ch1 = ofTexture()
       #  #self.ch1.allocate(int(img.getWidth()), int(img.getHeight()), int(6408), int(0))
       #  self.ch1.loadData(img.getPixels())
       #  self.ch1.generateMipmap()
       #  self.ch1.setTextureWrap(10497, 10497)

       #  img.load(ch2_file)
       #  self.ch2 = ofTexture()
       # # self.ch2.allocate(int(img.getWidth()), int(img.getHeight()), 6408, 0)
       #  self.ch2.loadData(img.getPixels());
       #  self.ch2.generateMipmap();
       #  self.ch2.setTextureWrap(10497, 10497);


    def update(self):
        self.updateNumber()
        self.updateAlpha()
        self.updateFbo()

    def translate(self, value, leftMin, leftMax, rightMin, rightMax):
        leftSpan = leftMax - leftMin
        rightSpan = rightMax - rightMin
        valueScaled = float(value -leftMin)/float(leftSpan)
        return rightMin + valueScaled*rightSpan

    def updateNumber(self):
        angle =  np.sin(ofGetElapsedTimef()*np.pi/self.period)
        self.number = self.translate(angle, -1.0,1.0, 0.2, 0.9)

    def updateAlpha(self):
        self.currentAlpha = self.currentAlpha + (self.targetAlpha - self.currentAlpha)*0.05

    def updateFbo(self):
        self.fbo.begin()
        ofClear(0)
        ofSetColor(255)
        self.drawShader()
        #self.img.draw(0,0,self.width, self.height)
        self.fbo.end()

        self.fbo2.begin()
        ofClear(0)
        if self.shaderNoise.isLoaded():
            self.shaderNoise.begin()
            self.shaderNoise.setUniform1f('alpha',  self.currentAlpha)
            self.shaderNoise.setUniform1f('amplitude', 5)
            self.shaderNoise.setUniform1f('time', ofGetElapsedTimef()*0.2)
            self.shaderNoise.setUniform1f('frequency', 1.6)
            self.shaderNoise.setUniform1f('speed', 0.7)
            self.fbo.draw(0,0)
       
            self.shaderNoise.end()
        self.fbo2.end()

    def draw(self):

        self.fbo2.draw(0,0)

        #self.drawShader()

    def drawShader(self):

        if self.shader.isLoaded():

            self.shader.begin()
            #self.shader.setUniform1f('alpha',  self.currentAlpha)
            self.shader.setUniform1f('iTime', ofGetElapsedTimef()*self.speed)
            self.shader.setUniform3f('iResolution', float(self.width), float(self.height),0.0)
            #self.shader.setUniformTexture("iChannel0", self.ch1, 1);
            self.shader.setUniformTexture("iChannel1", self.fbo2 , 2);
            self.shader.setUniform1f('contrast',  self.contrast)
            self.shader.setUniform1f('brightness', self.brightness)
            self.shader.setUniform1f('saturation', self.saturation)
            self.shader.setUniform1f('gamma', self.gamma)
            self.img.draw(-self.width/2.,-self.height/2.,self.width,self.height)
            #ofDrawRectangle(-self.width/2.,-self.height/2.,self.width,self.height)
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
            uniform sampler2DRect iChannel0;
            uniform sampler2DRect iChannel1;

            uniform float contrast = 1.0;
            uniform float saturation = 1.0;
            uniform float brightness = 1.0;

            uniform float gamma = 1.0;
            uniform float minInput = 0.0;
            uniform float maxInput = 1.0;
            uniform float minOutput = 0.0;
            uniform float maxOutput = 1.0;
            uniform float alpha = 1.0;

            in vec4 position_frag;
          """


        self.frag_contents = """
        
           #define TAU 6.2831853071

            void mainImage(out vec4 fragColor, in vec2 fragCoord) {
                vec2 uv = fragCoord.xy / iResolution.xy;
                
                float o = texture(iChannel1, uv * 0.25 + vec2(0.0, iTime * 0.025)).r;
                float d = (texture(iChannel0, uv * 0.25 - vec2(0.0, iTime * 0.02 + o * 0.02)).r * 2.0 - 1.0);
                
                float v = uv.y + d * 0.1;
                v = 1.0 - abs(v * 2.0 - 1.0);
                v = pow(v, 2.0 + sin((iTime * 0.2 + d * 0.25) * TAU) * 0.5);
                
                vec3 color = vec3(0.0);
                
                float x = (1.0 - uv.x * 0.75);
                float y = 1.0 - abs(uv.y * 2.0 - 1.0);
                color += vec3(x * 0.5, y, x) * v;
                
                vec2 seed = fragCoord.xy;
                vec2 r;
                r.x = fract(sin((seed.x * 12.9898) + (seed.y * 78.2330)) * 43758.5453);
                r.y = fract(sin((seed.x * 53.7842) + (seed.y * 47.5134)) * 43758.5453);

                float s = mix(r.x, (sin((iTime * 2.5 + 60.0) * r.y) * 0.5 + 0.5) * ((r.y * r.y) * (r.y * r.y)), 0.04); 
                //color += pow(s, 70.0) * (1.0 - v);
                
                fragColor.rgb = color;
                fragColor.a = alpha;
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

    def setupShaderNoise(self):

        self.shaderNoise = ofShader()
        temp_dir = mkdtemp()
        frag_file = os.path.join(temp_dir,'s.frag')
        vert_file = os.path.join(temp_dir,'s.vert')
        shader_file_of = os.path.join(temp_dir,'s')

        self.vert_contents = """
        #version 150

        // these are for the programmable pipeline system
        uniform mat4 modelViewProjectionMatrix;

        in vec4 position;
        in vec2 texcoord;

        out vec2 texCoordVarying;

        void main()
        {
            texCoordVarying = texcoord;
            
            gl_Position = modelViewProjectionMatrix * position;
        }
        """

        self.frag_contents_prefix = """
        #version 150    // <-- version my machine suports

        uniform sampler2DRect texture0;
        uniform float time;
        uniform float amplitude;
        uniform float speed;
        uniform float frequency;
        uniform float alpha = 1.0;


        in vec2 texCoordVarying;

        out vec4 outputColor;
        """

        self.frag_contents = """
                
        //Classic Perlin noise function declaration
        //See definition in the end of file
        float cnoise(vec3 P);

        void main(){
            vec2 pos = texCoordVarying;
            
            //Shift pos using Perlin noise
            vec2 shift;
            shift.x = cnoise( vec3(pos*speed*0.01, time * frequency + 17.0) )*amplitude;
          shift.y = cnoise( vec3(pos*speed*0.01, time * frequency + 12.0) )*amplitude;
            pos += shift;
            
            vec4 color = texture(texture0, pos);
            //Output of the shader
            outputColor = color;
            outputColor.a = alpha;
        }

        """

        self.frag_contents_suffix = """
        //-----------------------------------------------------------
        // Classic Perlin noise implementation
        // float cnoise(vec3 P)
        //-----------------------------------------------------------
        //
        //Pasted from src/classicnoise3D.glsl file,
        //from webgl-noise library by Ashima Arts and Stefan Gustavson, 
        //https://github.com/ashima/webgl-noise

        //
        // Description : Array and textureless GLSL 2D/3D/4D simplex 
        //               noise functions.
        //      Author : Ian McEwan, Ashima Arts.
        //  Maintainer : ijm
        //     Lastmod : 20110822 (ijm)
        //     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
        //               Distributed under the MIT License. See LICENSE file.
        //               https://github.com/ashima/webgl-noise
        //  

        vec3 mod289(vec3 x)
        {
          return x - floor(x * (1.0 / 289.0)) * 289.0;
        }

        vec4 mod289(vec4 x)
        {
          return x - floor(x * (1.0 / 289.0)) * 289.0;
        }

        vec4 permute(vec4 x)
        {
          return mod289(((x*34.0)+1.0)*x);
        }

        vec4 taylorInvSqrt(vec4 r)
        {
          return 1.79284291400159 - 0.85373472095314 * r;
        }

        vec3 fade(vec3 t) {
          return t*t*t*(t*(t*6.0-15.0)+10.0);
        }

        // Classic Perlin noise
        float cnoise(vec3 P)
        {
          vec3 Pi0 = floor(P); // Integer part for indexing
          vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
          Pi0 = mod289(Pi0);
          Pi1 = mod289(Pi1);
          vec3 Pf0 = fract(P); // Fractional part for interpolation
          vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
          vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
          vec4 iy = vec4(Pi0.yy, Pi1.yy);
          vec4 iz0 = Pi0.zzzz;
          vec4 iz1 = Pi1.zzzz;

          vec4 ixy = permute(permute(ix) + iy);
          vec4 ixy0 = permute(ixy + iz0);
          vec4 ixy1 = permute(ixy + iz1);

          vec4 gx0 = ixy0 * (1.0 / 7.0);
          vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
          gx0 = fract(gx0);
          vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
          vec4 sz0 = step(gz0, vec4(0.0));
          gx0 -= sz0 * (step(0.0, gx0) - 0.5);
          gy0 -= sz0 * (step(0.0, gy0) - 0.5);

          vec4 gx1 = ixy1 * (1.0 / 7.0);
          vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
          gx1 = fract(gx1);
          vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
          vec4 sz1 = step(gz1, vec4(0.0));
          gx1 -= sz1 * (step(0.0, gx1) - 0.5);
          gy1 -= sz1 * (step(0.0, gy1) - 0.5);

          vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
          vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
          vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
          vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
          vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
          vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
          vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
          vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

          vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
          g000 *= norm0.x;
          g010 *= norm0.y;
          g100 *= norm0.z;
          g110 *= norm0.w;
          vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
          g001 *= norm1.x;
          g011 *= norm1.y;
          g101 *= norm1.z;
          g111 *= norm1.w;

          float n000 = dot(g000, Pf0);
          float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
          float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
          float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
          float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
          float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
          float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
          float n111 = dot(g111, Pf1);

          vec3 fade_xyz = fade(Pf0);
          vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
          vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
          float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
          return 2.2 * n_xyz;
        } 
        """    

        with open(frag_file,'w') as f:
            f.write(self.frag_contents_prefix)
            f.write(self.frag_contents)
            f.write(self.frag_contents_suffix)
        with open(vert_file,'w') as f:
            f.write(self.vert_contents)
        self.shaderNoise.load(shader_file_of)

