//
//  ofxParallaxScroll.h
//  ParallaxScrolling
//
//  Created by james KONG on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ParallaxScrolling_StackObject_h
#define ParallaxScrolling_StackObject_h
#include "ofMain.h"
#include <list>
#include "ofxTween.h"
#include "ofxQTKitVideoPlayer.h"
#include "ofxImageSequence.h"
class ofxTweenerEventArgs: public ofEventArgs
{
public: 
    string arg;
};
class MDraws : public ofBaseDraws ,public ofRectangle
{
public:
    MDraws()
    
    {
        ptr = NULL;
        isDrawing = false;
        vPrt = NULL;
        
    }
    MDraws(ofBaseDraws *_ptr)
    {
        ofRectangle::set(0,0,_ptr->getWidth(),_ptr->getHeight());
        ptr = _ptr;
        isDrawing = false;
        vPrt = NULL;
    }
    
    MDraws(ofxQTKitVideoPlayer *_ptr)
    {
        ofRectangle::set(0,0,_ptr->getWidth(),_ptr->getHeight());
        ptr = NULL;
        isDrawing = false;
        vPrt = _ptr;
    }
    virtual void update()
    {
        
        
        
    }
    void draw(float x, float y){
        
        //ofRect(ofRectangle::x,ofRectangle::y,ofRectangle::width,ofRectangle::height);
        //ofSetColor(255);
        if(ptr!=NULL)ptr->draw(ofRectangle::x+x,ofRectangle::y+y,ofRectangle::width,ofRectangle::height);
        else if(vPrt!=NULL)vPrt->draw(ofRectangle::x+x,ofRectangle::y+y,ofRectangle::width,ofRectangle::height);
        isDrawing = true;
    }
	virtual void draw(float x, float y, float w, float h){
        //draw(ofRectangle::x,ofRectangle::y);
        if(ptr!=NULL)ptr->draw(ofRectangle::x+x,ofRectangle::y+y,ofRectangle::width,ofRectangle::height);
        else if(vPrt!=NULL)vPrt->draw(ofRectangle::x+x,ofRectangle::y+y,ofRectangle::width,ofRectangle::height);
    }
	void draw(const ofPoint & point) {
		draw(point.x, point.y);
	}
	void draw(const ofRectangle & rect) {
		draw(rect.x, rect.y, rect.width, rect.height);
	}
	void draw(const ofPoint & point, float w, float h) {
		draw(point.x, point.y, w, h);
	}
    float getWidth() {return ofRectangle::width;}
    float getHeight() {return ofRectangle::height;}
    ofBaseDraws *ptr;
    ofxQTKitVideoPlayer *vPrt;
    bool isDrawing;
};
class MImageSequence : public MDraws
{
public:
    MImageSequence(ofxImageSequence *_ptr , string filename , int x, int  y)
    {
        ofRectangle::set(0,0,_ptr->getWidth(),_ptr->getHeight());
        ptr = _ptr->getFrame(0);
        
        fn = filename;
        
    }
    void update()
    {
        if(x>-ofRectangle::width && x < ofGetWidth()+50 )
        {
            
        }
        else
        {
            
        }
        
    }
    virtual void draw(float x, float y, float w, float h){
        
        ptr->draw(ofRectangle::x+x,ofRectangle::y+y,ofRectangle::width,ofRectangle::height);
        
    }
    ofxImageSequence *seq;
    string fn;
    
};

class MImage : public MDraws
{
public:
    MImage(ofImage *_ptr , string filename)
    {
        ofRectangle::set(0,0,_ptr->getWidth(),_ptr->getHeight());
        ptr = _ptr;
        img  =_ptr;
        fn = filename;
        _ptr->clear();
    }
    void update()
    {
        if(x>-ofRectangle::width && x < ofGetWidth()+50 )
        {
            
            if(!img->isAllocated())
            {
                img->loadImage(fn);
                ofLog(OF_LOG_VERBOSE,"MImage loadImage : "+ fn);
            }
            
        }
        else
        {
            if(img->isAllocated())
            {
                img->clear();
                ofLog(OF_LOG_VERBOSE,"MImage clear : "+ fn);
            }
        }
        
    }
    virtual void draw(float x, float y, float w, float h){
        
        if(img->isAllocated())
        {
            img->draw(ofRectangle::x+x,ofRectangle::y+y,ofRectangle::width,ofRectangle::height);
        }
    }
    ofImage *img;
    string fn;
    
};
class MVideo : public MDraws
{
public:
    
    MVideo(ofxQTKitVideoPlayer *_ptr , string filename)
    {
        ofRectangle::set(0,0,_ptr->getWidth(),_ptr->getHeight());
        ptr = _ptr->getTexture();
        
        fn = filename;
        _ptr->stop();
        vid = _ptr;
    }
    void update()
    {

        if(x>-ofRectangle::width+50 && x < ofGetWidth()+50 )
        {
            if(!vid->isLoaded())
            {
                vid->loadMovie(fn);
                ofLog(OF_LOG_VERBOSE,"MVideo loadMovie : "+ fn);
            }
            if(!vid->isPlaying())
            {
                vid->play();
                ofLog(OF_LOG_VERBOSE,"MVideo play : "+ fn);
            }
            vid->update();
            
        }
        else
        {
            if(vid->isPlaying())
            {
                vid->stop();
                ofLog(OF_LOG_VERBOSE,"MVideo stop : "+ fn);
            }
            if(vid->isLoaded())
            {
                vid->close();
                ofLog(OF_LOG_VERBOSE,"MVideo close : "+ fn);
            }
        }
        
    }
    virtual void draw(float x, float y, float w, float h){
        
        if(vid->isLoaded())
        {
            vid->draw(ofRectangle::x+x,ofRectangle::y+y,ofRectangle::width,ofRectangle::height);
        }
    }
    ofxQTKitVideoPlayer *vid;
    string fn;
};
class StackObject : public ofRectangle ,public ofBaseUpdates, public ofxTween
{
    bool fire ;
    string name;
public:
    StackObject()
    {
        name = "default";
        fire = false;
        set(0,0,0,0);
        static_alpha = 50;
        ofAddListener(ofEvents().update, this, &StackObject::_update);
    }
    virtual void _update(ofEventArgs &e)
    {
        if(isRunning())
        {
            fire = true;
        }
        if(isCompleted() && fire)
        {
            fire = false;
            static ofxTweenerEventArgs a;
            a.arg = name;
            ofNotifyEvent(TWEEN_COMPLETE, a, this);
        }
        update();
    }
    virtual void update()
    {
        
    }
    virtual void draw()
    {
    }
    virtual float getWidth()
    {
        return width;
    }
    virtual float getWidth(int i)
    {
        return childs[i]->getWidth();
    }
    virtual int size()
    {
        return 0;
    }
    virtual void setName(string _name)
    {
        name = _name;
    }
    
    virtual void setAlpha(int a)
    {
        static_alpha = a;
    }
    ofEvent <ofxTweenerEventArgs> TWEEN_COMPLETE;
    vector <MDraws*> childs;
protected:
    int static_alpha;
    
};

#endif
