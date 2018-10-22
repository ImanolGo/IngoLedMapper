from openframeworks import *
from protopixel import Content
from tempfile import mkdtemp
import os.path
from random import randint
import imp

content = Content("IngoSchuster")

sunset_file = content.add_asset("Sunset.py")
sunset = imp.load_source('sunset',sunset_file)

clouds_file = content.add_asset("Clouds.py")
clouds = imp.load_source('clouds',clouds_file)

aurora_file = content.add_asset("Aurora.py")
aurora = imp.load_source('clouds',aurora_file)

print "IngoSchuster"


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

content.add_parameter("gamma", min=0.0, max=1.0, value=0.9)
content.add_parameter("enableSunset", value=True)
content.add_parameter("enableClouds", value=True)
content.add_parameter("enableAurora", value=True)


def setup():
    """
    This will be called at the beggining, you set your stuff here
    """

    global size, sunset, clouds, aurora

    sunset = sunset.Sunset(width,height)
    clouds = clouds.Clouds(width,height)
    aurora = aurora.Aurora(width,height)

    ofEnableAlphaBlending()
    fbo.allocate(width,height)
    fbo.begin()
    ofClear(255)
    fbo.end()

    fboVisuals.allocate(width,height)
    fboVisuals.begin()
    ofClear(255)
    fboVisuals.end()

    setupShader()


def update():
    """
    For every frame, before drawing, we update stuff
    """

    global sunset

    #updateTime()

    sunset.update()
    clouds.update()
    aurora.update()

    updateFbo()
        

def updateTime():
    global elapsedTime, change_time, mode

    elapsedTime+=ofGetLastFrameTime()
    if elapsedTime>change_time:
        mode = (mode+1)%num_modes
        changeMode(mode)
        elapsedTime = 0


def updateFbo():

    global energy

    brightness = int(energy*255)
    #print brightness

    fboVisuals.begin()
    ofClear(0)
    sunset.draw()    
    clouds.draw() 
    aurora.draw()
    fboVisuals.end()


    fbo.begin()
    ofClear(0)
    if shader.isLoaded():
        shader.begin()
        shader.setUniform1f('gamma', content["gamma"])
        shader.setUniform1f('alpha', energy)
        fboVisuals.draw(0,0)
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


def setAlphas(value):
    global sunset, clouds
    sunset.setAlpha(value)
    clouds.setAlpha(value)
    aurora.setAlpha(value)
    


@content.OSC('/ppx/changeMode')
def changeMode(i):
    global elapsedTime

    print "/ppxIngoSchuster/changeMode " + str(i)

    elapsedTime = 0.0

    if i==0:
        print "Set Sunset"
        setAlphas(0.0)
        sunset.setAlpha(1.0)
    elif i==1:
        print "Set Clouds"
        setAlphas(0.0)
        clouds.setAlpha(1.0)
    elif i==2:
        print "Set Aurora"
        setAlphas(0.0)
        aurora.setAlpha(1.0)



@content.OSC('/ppx/energy')
def changeEnergy(x):
    global energy
    energy = 1.0 - x
    #print energy


@content.parameter_changed('enableSunset')
def parameter_changed(value):
    changeMode(0)

@content.parameter_changed('enableClouds')
def parameter_changed(value):
    changeMode(1)

@content.parameter_changed('enableAurora')
def parameter_changed(value):
    changeMode(2)


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






