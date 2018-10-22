from protopixel import Content
from openframeworks import *
import os.path
from tempfile import mkdtemp


class Sunset:

    def __init__(self, width, height):

        self.width = width
        self.height = height
        self.shader = ofShader()
        self.fbo = ofFbo()
        self.fbo.allocate(width,height)
        self.currentAlpha = 1.0
        self.targetAlpha = 1.0
        self.elapsedTime = 0.0
        self.one_day_in_seconds = 60*60*24
        self.speed = 1.0
        self.setup()


    def setup(self):
        self.setupShader()

    def update(self):
        self.updateTime()
        self.updateAlpha()
        self.updateFbo()

    def updateTime(self):
        self.elapsedTime += ofGetLastFrameTime()
        if self.elapsedTime > self.one_day_in_seconds:
            self.elapsedTime-= self.one_day_in_seconds

    def updateAlpha(self):
        self.currentAlpha = self.currentAlpha + (self.targetAlpha - self.currentAlpha)*0.02
       # print self.currentAlpha 

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
            self.shader.begin()
            self.shader.setUniform1f('alpha',  self.currentAlpha)
            self.shader.setUniform1f('iGlobalTime', self.elapsedTime*self.speed)
            self.shader.setUniform3f('iResolution', float(self.width), float(self.height),0.0)
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
          uniform float iGlobalTime;
          uniform float alpha = 1.0;

          in vec4 position_frag;
          """


        self.frag_contents = """
        
        void mainImage( out vec4 fragColor, in vec2 fragCoord ) {

             // Normalized pixel coordinates (from 0 to 1)
            vec2 uv = fragCoord/iResolution.xy;

            // Time varying pixel color
            vec3 col = 0.5 + 0.5*cos(sin(iGlobalTime/2.)+uv.xyx+vec3(0,2,41));

            // Output to screen
            fragColor = vec4(col,alpha);
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

