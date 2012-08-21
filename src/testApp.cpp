#include "testApp.h"



#define USE_TRANSPARENCY false
static bool reload = false;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
        ofSetWindowShape(WIDTH,HEIGHT);
//    showLog = false;
    glDisable(GL_DEPTH_TEST);
    
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    //glEnable(GL_POLYGON_SMOOTH);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    ofHideCursor();
    ofSetFrameRate(60);
    ofToggleFullscreen();

    settings.loadFile("configs.xml");
    if(settings.pushTag("DATA"))
    {
        helpStr = settings.getValue("HELP", "help:\nH/h toggle instruction");
        ofLog(OF_LOG_VERBOSE,"helpStr:\n"+helpStr);
        string fontName = settings.getAttribute("FONT","name","/Library/Fonts/Arial.ttf");
        float fontSize = settings.getAttribute("FONT","size",12.0f);
        font.loadFont(fontName,fontSize);
//        ofRectangle rect = font.getStringBoundingrBox(helpStr, 0, 0);
        helpX = 10;//ofGetWidth()-font.stringWidth(helpStr);
        helpY = ofGetHeight()-font.stringHeight(helpStr);
        ofLog(OF_LOG_VERBOSE,"rect.width:%f rect.height:%f helpX :%f , helpY :%f",font.stringWidth(helpStr), font.stringHeight(helpStr),helpX,helpY);
        settings.popTag();
    }

    
    parallax.setup(N_SCREEN,WIDTH*N_SCREEN);
    reload = true;
    img.loadImage("starry_night.jpeg");
    video.loadMovie("movies/example.m4v");
    video.play();
    video.setLoopState(OF_LOOP_NORMAL);
    StackImage *stack_image = new StackImage();
    
    ofDirectory dir;
    dir.allowExt("jpg");
    dir.allowExt("jpeg");
    dir.allowExt("png");
    int num = dir.listDir("images/first");
    for(int i = 0 ; i < num ; i++)
    {
       ofImage *new_img =  new ofImage();
      if(new_img->loadImage(dir.getPath(i)))
      {
          
          MImage *new_draw = new MImage(new_img,dir.getPath(i) );
          stack_image->pushImage(new_draw);

          //stack_image->pushImage((MDraws*)new_img);
      }
        else
        {
            ofLog(OF_LOG_VERBOSE,"Can not load File "+dir.getPath(i));
        }
        if(i==6)
        {
            MVideo *special = new MVideo(&video,"movies/example.m4v");
            
            stack_image->pushImage(special);
        }
    }
    stack_image->setName("images/first");
    stack_image->setAlpha( 230 );
    stack_image->x = 0;
    stack_image->y = 0;
    ofAddListener(stack_image->TWEEN_COMPLETE, this, &testApp::tweenerCompleted);
    parallax.pushLayer(stack_image);
#ifdef USE_PARTICLE
    particle.setup(1024,768);
    particle.setName("particle");
    ofAddListener(particle.TWEEN_COMPLETE, this, &testApp::tweenerCompleted);
    parallax.pushLayer(&particle);
#endif    
    stack_image = new StackImage();
    ofAddListener(stack_image->TWEEN_COMPLETE, this, &testApp::tweenerCompleted);
    num = dir.listDir("images/second");
    stack_image->setName("images/second");
    for(int i = 0 ; i < num ; i++)
    {
        ofImage *new_img =  new ofImage();
        if(new_img->loadImage(dir.getPath(i)))
        {
            MImage *new_draw = new MImage(new_img,dir.getPath(i));
            stack_image->pushImage(new_draw);
        }
        else
        {
            ofLog(OF_LOG_VERBOSE,"Can not load File "+dir.getPath(i));
        }
    }
    stack_image->x = 0;
    stack_image->y = 0;
    stack_image->setAlpha( 50 );
    parallax.pushLayer(stack_image);

    //setup the server to listen on 11999
	TCP.setup(7170);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/TCP]
	TCP.setMessageDelimiter("\n");
    
    //setup Fluid
    // setup fluid stuff
    gui.addTitle("warpper");
    
    gui.config->toggleHeight = 18;
	gui.config->buttonHeight = 18;
	gui.config->sliderTextHeight = 22;
	gui.config->titleHeight = 18;
	gui.config->fullActiveColor = 0x094c50;
    gui.addToggle("showHelp",showHelp);
    gui.addToggle("showDraw",showDraw);
    warpper.setup();
    #ifdef USE_FLUID
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup( &fluidSolver );
	particleSystem.setFluidSolver( &fluidSolver );
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
    
    gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
	gui.addButton("resizeFluid", resizeFluid);
	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01); 
	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003); 
	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1); 
	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50); 
	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
	gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, kFluidDrawCount, (string*)FluidDrawerGl::drawOptionTitles);
	gui.addToggle("fs.doRGB", fluidSolver.doRGB); 
	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement); 
	gui.addToggle("drawFluid", drawFluid); 
    gui.addToggle("drawParticles", drawParticles); 
	gui.addToggle("fs.wrapX", fluidSolver.wrap_x); 
	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
    
	gui.setDefaultKeys(true);
//    warpPoints[0].x = 0;
//    warpPoints[0].y = 0;
//    warpPoints[1].x = 320;
//    warpPoints[1].y = 0;
//    warpPoints[2].x = 320;
//    warpPoints[2].y = 240;
//    warpPoints[3].x = 0;
//    warpPoints[3].y = 240;
//
//    gui.addQuadWarper("Warp", video, warpPoints).setNewColumn(true);
	gui.currentPage().setXMLName("MSAFluidDemo.xml");
#endif
    
    gui.setDefaultKeys(true);

    gui.loadFromXML();
    
	gui.setAutoSave(true);
    bAlignRight = false;

	
}
#ifdef USE_FLUID
void testApp::fadeToColor(float r, float g, float b, float speed) {
    glColor4f(r, g, b, speed);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
}


// add force and dye to fluid, and create particles
void testApp::addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce ) {
    float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    if(speed > 0) {
		pos.x = constrain(pos.x, 0.0f, 1.0f);
		pos.y = constrain(pos.y, 0.0f, 1.0f);
		
        const float colorMult = 100;
        const float velocityMult = 30;
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if(addColor) {
			Color drawColor( CM_HSV, ( getElapsedFrames() % 360 ) / 360.0f, 1, 1 );
            //			Color drawColor;
            //			drawColor.setHSV(( getElapsedFrames() % 360 ) / 360.0f, 1, 1 );
			
			fluidSolver.addColorAtIndex(index, drawColor * colorMult);
			
			if( drawParticles )
				particleSystem.addParticles( pos * Vec2f( getWindowSize() ), 10 );
		}
		
		if(addForce)
			fluidSolver.addForceAtIndex(index, vel * velocityMult);
		
    }
}
#endif
#define NUM_BYTE 512
//--------------------------------------------------------------
void testApp::update(){
    warpper.update();
//    video.update();
    //temp
    parallax.mouseX = mouseX;
    parallax.mouseY = mouseY;
    //fluid begin
    #ifdef USE_FLUID
    if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / getWindowAspectRatio() );
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}
//    Vec2f eventPos = Vec2f(getWindowWidth()/2, getWindowHeight()/2);
	Vec2f mouseNorm = Vec2f( 0.5,0.5);
    
	//Vec2f mouseVel = Vec2f(sin(ofGetElapsedTimef()*0.1)*10,cos(ofGetElapsedTimef()*0.1)*10 );
    if(ofGetElapsedTimef()<10)
    {
    for(int i = 0 ; i < 360 ; i+=30)
    {
        Vec2f mouseVel = Vec2f(sin(i/360 *TWO_PI),cos(i/360 *TWO_PI) );
        addToFluid( mouseNorm, mouseVel, true, true );
    }
    }
    fluidSolver.update();

    //fluid end
#endif
    
    for(int i = 0; i < TCP.getLastID(); i++){
		if( !TCP.isClientConnected(i) )continue;
        
        
        string str = TCP.receive(i);
        if(str.find("left")!=string::npos)
        {
            parallax.moveleft();
        }
        else if(str.find("right")!=string::npos) {
            parallax.moveright();
        }
		if(str!="")
        {
            string sendOut = str+"\r\n";
            TCP.sendRawBytes(i, sendOut.c_str(), sendOut.length());//(i, str);
            ofLog(OF_LOG_VERBOSE,str);
            //free(c);
        }
	}
    
    Tweener.update();
    if(reload)
    {
        
            reload = false;
    }
	
	
    
    warpper.begin();
        ofSetColor(0);
        ofRect(0,0,WIDTH*N_SCREEN,HEIGHT);
    
        ofSetColor(255, 255, 255);
    #ifdef USE_FLUID
    //fluid
    if( drawFluid ) {
		glColor3f(1, 1, 1);
		fluidDrawer.draw(0, 0, getWindowWidth(), getWindowHeight());
	} else {
		if(getElapsedFrames()%5==0) fadeToColor( 0, 0, 0, 0.1f );
	}
    if( drawParticles )
		particleSystem.updateAndDraw( drawFluid );

    //fluid
#endif
        ofPushMatrix();
        ofTranslate(0,0);
        if(showDraw)parallax.draw();
        else {
            ofBackgroundGradient(ofColor(150), ofColor(50));
        }
        ofPopMatrix();
        //img.draw(-WIDTH*i,0,WIDTH*N_SCREEN,HEIGHT);
    warpper.end();
    

}


//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    
    if(showDraw)warpper.draw();
    
	ofPushStyle();
    //particle.draw();
//	if(showLog)
//    {
//	    ostringstream os;
//        os << "c: show/hide controls"<<endl
//        << "1: active screen 1"<<endl
//        << "2: active screen 2"<<endl
//        << "0: deactive all screen"<<endl
//        <<"a: corner/anchor controls"<<endl
//        <<"s: save"<<endl
//        <<"d: default"<<endl
//        <<"p: preset (last saved)"<<endl
//        <<"framerate: " <<ofToString(ofGetFrameRate(),0)<<endl;
//        ofSetHexColor(0xFFFFFF);
//        ofDrawBitmapString(os.str(),10,ofGetHeight()-200);
//        ofPopStyle();
//	
//	
//        
//	}

	gui.draw();
    
    if(showHelp)font.drawString(helpStr, helpX, helpY);
    //ofDrawBitmapString(helpStr, 0,0);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key) {
            case 'h':
            showHelp = !showHelp;
            //ofShowCursor();
            break;
		case 'f':
			ofToggleFullscreen();
			break;
            
        case OF_KEY_LEFT:
            parallax.moveleft();
            #ifdef USE_PARTICLE
            particle.reset();
#endif
            break;
        case OF_KEY_RIGHT:
            parallax.moveright();
            #ifdef USE_PARTICLE
            particle.reset();
#endif
            break;
//        case '[':
//            gui.prevPage();
//            break;
//        case ']':
//            gui.nextPage();
//            break;
        case '\\':
            bAlignRight =!bAlignRight;
            gui.setAlignRight(bAlignRight);
            break;
        case ' ':
            //gui.isOn();
            if(gui.isOn())
            {
             //   gui.hide();
                ofHideCursor();
            }
            else {
                //gui.show();
                ofShowCursor();
            }
            break;
			
		case 'b': {
#ifdef USE_FLUID
			Timer timer;
			const int ITERS = 3000;
			timer.start();
			for( int i = 0; i < ITERS; ++i ) fluidSolver.update();
			timer.stop();
			cout << ITERS << " iterations took " << timer.getSeconds() << " seconds." << std::endl;
#endif
		}
			break;
			
    }
    warpper.keyPressed(key);

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
	
    warpper.keyReleased(key);

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
#ifdef USE_FLUID
	Vec2f eventPos = Vec2f(x, y);
	Vec2f mouseNorm = Vec2f( eventPos) / getWindowSize();
	Vec2f mouseVel = Vec2f( eventPos - pMouse ) / getWindowSize();
	addToFluid( mouseNorm, mouseVel, true, true );
	pMouse = eventPos;
#endif	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    warpper.mouseDragged(x, y, button);
    #ifdef USE_FLUID
    Vec2f eventPos = Vec2f(x, y);
	Vec2f mouseNorm = Vec2f( eventPos ) / getWindowSize();
	Vec2f mouseVel = Vec2f( eventPos - pMouse ) / getWindowSize();
	addToFluid( mouseNorm, mouseVel, false, true );
	pMouse = eventPos;
#endif
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    warpper.mousePressed(x, y, button);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    warpper.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
#ifdef USE_FLUID
    particleSystem.setWindowSize( Vec2f( w, h ) );
#endif    
}
void testApp::tweenerCompleted(ofxTweenerEventArgs &e)
{
    ofLog(OF_LOG_VERBOSE,"testApp: traget.name = "+e.arg);
}




