//
//  MyBizierWarp.h
//  ParticleMotion
//
//  Created by james KONG on 13/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ParticleMotion_MyBizierWarp_h
#define ParticleMotion_MyBizierWarp_h
#include "ofxBezierWarp.h"
#include "ofxGLWarper.h"
#include "ofxSimpleGuiToo.h"
#include "ofxTextInputField.h"
#include "ofxProjectorBlend.h"
#define PIXEL_OVERLAP 40
#define PROJECTOR_COUNT N_SCREEN
#define PROJECTOR_WIDTH WIDTH
#define PROJECTOR_HEIGHT HEIGHT
struct Selection
{
    bool corners[4];
	bool anchors[8];  
    bool *grids;
};
class MyBizierWarp
{
public:
    void setup()
    {
        
//        offset[1][0].set(1,1);
//        offset[1][1].set(1,1);
//        offset[1][2].set(1,1);
//        offset[1][3].set(1,1);
        
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            offset[i][0].set(i,1);
            offset[i][1].set(i,1);
            offset[i][2].set(i,1);
            offset[i][3].set(i,1);
            bz[i].setup(10,N_SCREEN,WIDTH*N_SCREEN,HEIGHT,"preset"+ofToString(i)+".xml");
            //bz[i].setup(16,"preset"+ofToString(i)+".bin");
            //bz[i].recall();
            bz[i].inactive();
            //        warper[i].setup(); //initializates ofxGLWarper
            //        warper[i].deactivate();// this allows ofxGLWarper to automatically listen to the mouse and keyboard 
            gui.addPage("Screen"+ofToString(i));
            gui.addToggle("Screen"+ofToString(i)+"Active",bz[i].isActive);
            gui.addToggle("CORNER1", selection[i].corners[0]);
            gui.addToggle("CORNER2", selection[i].corners[1]);
            gui.addToggle("CORNER3", selection[i].corners[2]);
            gui.addToggle("CORNER4", selection[i].corners[3]);
            gui.addToggle("ANCHOR1", selection[i].anchors[0]);
            gui.addToggle("ANCHOR2", selection[i].anchors[1]);
            gui.addToggle("ANCHOR3", selection[i].anchors[2]);
            gui.addToggle("ANCHOR4", selection[i].anchors[3]);
            gui.addToggle("ANCHOR5", selection[i].anchors[4]);
            gui.addToggle("ANCHOR6", selection[i].anchors[5]);
            gui.addToggle("ANCHOR7", selection[i].anchors[6]);
            gui.addToggle("ANCHOR8", selection[i].anchors[7]);
            gui.addPage("Screen"+ofToString(i)+"Grid");
            
            
            selection[i].grids = new bool [(bz[i].gridRes+1)*(bz[i].gridRes+1)];
            
            for(int y = 0 ; y <=bz[i].gridRes ;y++)
            {

                for(int x = 0 ; x <=bz[i].gridRes ;x++)
                {
                    selection[i].grids[x+(y*(bz[i].gridRes+1))] = false;
                    gui.addToggle("GRID"+ofToString(x)+"-"+ofToString(y), selection[i].grids[x+(y*(bz[i].gridRes+1))]);//.setNewColumn((x==0 && y!=0));
                }
            }
//            gui.currentPage().setXMLName("Warpping"+ofToString(i)+".xml");
            gui.addPage("Screen"+ofToString(i)+"Control");
            gui.addButton("Relaod", bReload[i]);
            gui.addButton("Save", bSave[i]);
            
            gui.addButton("UP", bPointUP).setNewColumn(true);
            gui.addButton("DOWN", bPointDOWN);
            gui.addButton("LEFT", bPointLEFT);
            gui.addButton("RIGHT", bPointRIGHT);
            
        }
        fbo.allocate(WIDTH, HEIGHT);
        
        
        
        gui.loadFromXML();
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            bz[i].recall();
        }
//        blender.setShaderLocation(ofToDataPath("shaders/SmoothEdgeBlend"));
        blender.setup(WIDTH, HEIGHT, N_SCREEN, PIXEL_OVERLAP);
        blender.gamma = .5;
        blender.blendPower = 1;
        blender.luminance = 0;
        blender.addGuiPage();
        gui.loadFromXML();
//        ofShader shader;
//        shader.load("shaders/SmoothEdgeBlend");
    }
    void update()
    {
        //        for(int i = 0 ; i< N_SCREEN ;i++)
        //        {
        //            bz[i].recall();
        //        }
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            if(bReload[i])
            {
                bz[i].recall();
            }
            else if(bSave[i])
            {
                bz[i].save();
            }
        }
        //ofLog(OF_LOG_VERBOSE," bPointUP %i ",bPointUP);
        float diff = ofGetElapsedTimef()-elapsed;
        if (bPointUP)
        {
            modifyPoint(ofPoint(0,-diff));    
        }
        else if (bPointDOWN)
        {
            modifyPoint(ofPoint(0,diff));  
        }
        else if (bPointLEFT)
        {
            modifyPoint(ofPoint(-diff,0));  
        }
        else if (bPointRIGHT)
        {
            modifyPoint(ofPoint(diff,0)); 
        }
        
        
    }
    void modifyPoint(ofPoint p )
    {
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            if(bz[i].isActive)
            {
            if(selection[i].corners[0])bz[i].corners[0]+=p;
            if(selection[i].corners[1])bz[i].corners[1]+=p;
            if(selection[i].corners[2])bz[i].corners[2]+=p;
            if(selection[i].corners[3])bz[i].corners[3]+=p;
            if(selection[i].anchors[0])bz[i].anchors[0]+=p;
            if(selection[i].anchors[1])bz[i].anchors[1]+=p;
            if(selection[i].anchors[2])bz[i].anchors[2]+=p;
            if(selection[i].anchors[3])bz[i].anchors[3]+=p;
            if(selection[i].anchors[4])bz[i].anchors[4]+=p;
            if(selection[i].anchors[5])bz[i].anchors[5]+=p;
            if(selection[i].anchors[6])bz[i].anchors[6]+=p;
            if(selection[i].anchors[7])bz[i].anchors[7]+=p;
                for(int y = 0 ; y <=bz[i].gridRes ;y++)
                {
                    
                    for(int x = 0 ; x <=bz[i].gridRes ;x++)
                    {
                        
                        if(selection[i].grids[x+(y*(bz[i].gridRes+1))])bz[i].gridPoint[x][y]+=p;
                    }
                }
            }
        }
    }
    void begin()
    {
        fbo.begin();
//        blender.begin();
//        
        ofSetColor(100, 100, 100);
        ofRect(0, 0, blender.getCanvasWidth(), blender.getCanvasHeight());
    }
    void end()
    {
        
        fbo.end();
//        blender.end();
    }
    void draw()
    {
        blender.begin();
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            bz[i].draw(fbo.getTextureReference(),offset[i], N_SCREEN);

        }
        blender.end();       
        blender.draw();
        if(gui.isOn())
        {
            int size = 30;
            ofPushStyle();
            ofNoFill();
            ofSetColor(100, 255,  100);
            for(int i = 0 ; i < N_SCREEN ; i++)
            {
                if(bz[i].isActive)
                {
                    if(selection[i].corners[0])ofCircle(bz[i].corners[0],size);
                    if(selection[i].corners[1])ofCircle(bz[i].corners[1],size);
                    if(selection[i].corners[2])ofCircle(bz[i].corners[2],size);
                    if(selection[i].corners[3])ofCircle(bz[i].corners[3],size);
                    if(selection[i].anchors[0])ofCircle(bz[i].anchors[0],size);
                    if(selection[i].anchors[1])ofCircle(bz[i].anchors[1],size);
                    if(selection[i].anchors[2])ofCircle(bz[i].anchors[2],size);
                    if(selection[i].anchors[3])ofCircle(bz[i].anchors[3],size);
                    if(selection[i].anchors[4])ofCircle(bz[i].anchors[4],size);
                    if(selection[i].anchors[5])ofCircle(bz[i].anchors[5],size);
                    if(selection[i].anchors[6])ofCircle(bz[i].anchors[6],size);
                    if(selection[i].anchors[7])ofCircle(bz[i].anchors[7],size);
                    
                    for(int y = 0 ; y <=bz[i].gridRes ;y++)
                    {
                        
                        for(int x = 0 ; x <=bz[i].gridRes ;x++)
                        {
                         
                            if(selection[i].grids[x+((y*(bz[i].gridRes+1)))]){
                                ofPoint p = bz[i].drawDrid(x,y);
                                //textfield.text = ofToString(p.x)+"\n"+ofToString(p.y);
                                //textfield.draw(p.x,p.y);
                            }
                        }
                    }
                }
            }
            ofPopStyle();
        }
        
        
    }
    //--------------------------------------------------------------
    void keyPressed(int key){
        
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            bz[i].keyPressed(key);
        }
        switch(key)
        {
            case 'A':
            if(!bPointLEFT){
                bPointLEFT = true;
                elapsed = ofGetElapsedTimef();
            }
                break;
            case 'D':
            if(!bPointRIGHT){
                bPointRIGHT = true;
                elapsed = ofGetElapsedTimef();
            }    break;
            case 'S':
            if(!bPointDOWN){
                bPointDOWN = true;
                elapsed = ofGetElapsedTimef();
            }   break;
            case 'W':
            if(!bPointUP){
                bPointUP = true;
                elapsed = ofGetElapsedTimef();
            }   break;
        }
        for(int i = 0 ; i < N_SCREEN ; i++)
        {
            if(bz[i].isActive)bz[i].active();
            else bz[i].inactive();
        }
        
    }
    
    //--------------------------------------------------------------
    void keyReleased(int key){
        
        switch(key)
        {
            case 'A':
            {
                bPointLEFT = false;
            }
                break;
            case 'D':
            {
                bPointRIGHT = false;
                
            }    break;
            case 'S':
            {
                bPointDOWN = false;
                
            }   break;
            case 'W':
            {
                bPointUP = false;
                
            }   break;
        
//            case '0':
//                //            warper[0].deactivate();
//                bz[0].inactive();
//                if(N_SCREEN>1)
//                {
//                    bz[1].inactive();
//                    //                 warper[1].deactivate();
//                }
//                
//                //ofHideCursor();
//                break;
//            case '1':
//                bz[0].active();
//                //            warper[0].activate();
//                if(N_SCREEN>1)
//                {
//                    bz[1].inactive();
//                    //                 warper[1].deactivate();
//                }
//                //ofShowCursor();
//                break;
//            case '2':
//                bz[0].inactive();
//                //            warper[0].deactivate();
//                if(N_SCREEN>1)
//                {
//                    bz[1].active();
//                    //                warper[1].activate();
//                }
//                //ofShowCursor();
//                break;
                
                
            case 'd':
                for(int i = 0 ; i < N_SCREEN ; i++)
                {
                    bz[i].defaults(i);
                    
                }
                break;
                    
                
                
                
        }
        for(int i = 0 ; i < N_SCREEN ; i++)
        {
            if(bz[i].isActive)bz[i].active();
            else bz[i].inactive();
        }
        
    }
    
    
    //--------------------------------------------------------------
    void mouseDragged(int x, int y, int button){
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            bz[i].mouseDragged(x, y);
        }
        
    }
    
    //--------------------------------------------------------------
    void mousePressed(int x, int y, int button){
        
        
        
        for(int i= 0 ; i < N_SCREEN ; i++)
        {
            bz[i].mousePressed(x, y);
        }
        elapsed = ofGetElapsedTimef();
    }
    
    //--------------------------------------------------------------
    void mouseReleased(int x, int y, int button){
        
        //        for(int i= 0 ; i < N_SCREEN ; i++)
        //        {
        //            bz[i].mouseReleased(x, y);
        //        }
        for(int i = 0 ; i < N_SCREEN ; i++)
        {
            if(bz[i].isActive)bz[i].active();
            else bz[i].inactive();
        }
        elapsed = ofGetElapsedTimef();
    }
    
    ofxBezierWarp bz[N_SCREEN];
    ofxProjectorBlend blender;
    //ofxGLWarper warper[N_SCREEN];
    ofFbo fbo;//[N_SCREEN];
    ofPoint offset[N_SCREEN][4];
//    ofPoint offset2[4];
    bool bReload[N_SCREEN],bSave[N_SCREEN];
    int choice_out[N_SCREEN];
    Selection selection[N_SCREEN];
    bool bPointUP,bPointDOWN,bPointLEFT,bPointRIGHT;
    float elapsed;
    ofxTextInputField textfield;
};
#endif
