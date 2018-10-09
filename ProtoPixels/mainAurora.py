from openframeworks import *
from protopixel import Content
from tempfile import mkdtemp
import os.path
from random import randint
import imp

content = Content("IngoSchusterAurora")

aurora_file = content.add_asset("Aurora.py")
aurora = imp.load_source('aurora',aurora_file)

print "IngoSchusterSunset"


#a global variable
num_modes = 3
elapsedTime = 0.0
change_time = 350
width = 600
height= 100
content.FBO_SIZE = (width,height) #optional: define size of FBO, default=(100,100)
targetAlpha = 1.0
currentAlpha = 0.0
energy = 1.0
fbo = ofFbo()
fboVisuals = ofFbo()
shader = ofShader()
mode = 0

content.add_parameter("gamma", min=0.0, max=1.0, value=0.7)


def setup():
    """
    This will be called at the beggining, you set your stuff here
    """

    global size, aurora

    aurora = aurora.Aurora(width,height)

    ofEnableAlphaBlending()
    fbo.allocate(width,height)
    fbo.begin()
    ofClear(255)
    fbo.end()

    setupShader()


def update():
    """
    For every frame, before drawing, we update stuff
    """

    global aurora

    aurora.update()
    updateFbo()
    

def updateFbo():

    global energy 
    fbo.begin()
    ofClear(0)
    if shader.isLoaded():
        shader.begin()
        shader.setUniform1f('gamma', content["gamma"])
        shader.setUniform1f('alpha', energy)
        aurora.draw()  
    fbo.end()


def draw():
    """
    For every frame draw stuff. Do not forget to clear the frmebuffer!
    """

    ofEnableAlphaBlending()
    fbo.draw(0,0)



  
def exit():
    """
    Before removing the script, in case you have pending business.
    """
    pass


def on_enable():
    """
    This function is called when this content just got enabled.
    """
    pass


def on_disable():
    """
    This function is called when this content just got disabled.
    `update` and `draw` functions are not called while this content is disabled.
    """
    pass



@content.OSC('/ppx/energy')
def changeEnergy(x):
    global energy
    energy = 1.0 - x
    #print energy


def setupShader():

    global shader

    temp_dir = mkdtemp()
    frag_file = os.path.join(temp_dir,'s.frag')
    vert_file = os.path.join(temp_dir,'s.vert')
    shader_file_of = os.path.join(temp_dir,'s')

    vert_contents = """
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

    frag_contents_prefix = """
    #version 150    // <-- version my machine suports

    uniform sampler2DRect texture0;
    uniform float gamma = 0.2;
    uniform float alpha  = 1.0;

    in vec2 texCoordVarying;

    out vec4 outputColor;
    """

    frag_contents = """
            
    void main(){


        vec2 pos = texCoordVarying;
            

        //Output of the shader
        outputColor = texture(texture0, pos);
        outputColor.rgb = pow(outputColor.rgb, vec3(1.0 / gamma));
        outputColor.a = alpha;
        
    }

    """

    frag_contents_suffix = """
        
        
    """    

    with open(frag_file,'w') as f:
        f.write(frag_contents_prefix)
        f.write(frag_contents)
        f.write(frag_contents_suffix)
    with open(vert_file,'w') as f:
        f.write(vert_contents)
    shader.load(shader_file_of)






