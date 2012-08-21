#ifndef _TEST_APP
#define _TEST_APP

/*
 *  ParallaxScroll
 *
 *  Created by James Kong Kingsin on 12-08-07.
 *	ParallaxScroll effect control by network message
 *  
 *
 */
#include "ofMain.h"


#include "ofxTweener.h"
#include "ofxParallaxScroll.h"
#include "ofxNetwork.h"
#include "StackParticle.h"

#include "ofxQTKitVideoPlayer.h"
#include "ofxSimpleGuiToo.h"
#include "WarpGlobal.h"
//#define USE_FLUID
#ifdef USE_FLUID
#include "MSACore.h"
#include "MSAFluidSolver.h"
#include "MSAFluid.h"
#include "MSATimer.h"

#include "Vector.h"
#include "ParticleSystem.h"

using namespace cinder;
#endif
//#define USE_PARTICLE
class testApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    void exit()
    {
        TCP.close();
    }
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
	
    void tweenerCompleted(ofxTweenerEventArgs &e);
    ofImage img;
    ofxQTKitVideoPlayer video;
    ofxParallaxScroll parallax;
    
    ofxTCPServer TCP;
//    bool showLog;
    #ifdef USE_PARTICLE
    StackParticle particle;
#endif    
    //MSA FLUID
    #ifdef USE_FLUID
    
    void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(Vec2f pos, Vec2f vel, bool addColor, bool addForce);
    
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	
	MSA::FluidSolver	fluidSolver;
	MSA::FluidDrawerGl	fluidDrawer;	
    
	
	MSA::Vec2f				pMouse;
    bool drawParticles;
    ParticleSystem		particleSystem;
#endif
    ofPoint warpPoints[4];
    MyBizierWarp warpper;
    bool bAlignRight;
    
    ofxXmlSettings settings;
    string helpStr;
    ofTrueTypeFont font;
    float helpX,helpY;
    bool showHelp,showDraw;
};

#endif
