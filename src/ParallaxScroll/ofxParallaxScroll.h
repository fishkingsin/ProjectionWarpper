//
//  ofxParallaxScroll.h
//  ParallaxScrolling
//
//  Created by james KONG on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ParallaxScrolling_ofxParallaxScroll_h
#define ParallaxScrolling_ofxParallaxScroll_h
#include "StackObject.h"
#include "StackImage.h"
class ofxParallaxScroll : public ofRectangle
{
    private :
    enum MDIRECTION{ 
        RIGHT,
        LEFT ,
        NONE ,
    };
    MDIRECTION DIRECTION;
    int currentIndex;
    bool buttonLock;
    list <StackObject *> childs;
    ofxEasingBack 	easingback;
    ofxEasingBounce 	easingbounce;
    ofxEasingCirc 	easingcirc;
    ofxEasingCubic 	easingcubic;
    ofxEasingElastic easingelastic;
    ofxEasingExpo 	easingexpo;
    ofxEasingLinear 	easinglinear;
    ofxEasingQuad 	easingquad;
    ofxEasingQuart 	easingquart;
    ofxEasingQuint 	easingquint;
    ofxEasingSine	easingsine;
public:
    int mouseX,mouseY;
    void setup(int _width, int _height);
//    {
//        currentIndex  =0;
//        buttonLock = false;
//        DIRECTION = NONE;
//        width = ofGetWidth();
//        height = ofGetHeight();
//    }
    
    void update();
//    {
//        //        process();
//    }
    void draw();
//    {
//        //draw the list from top to bottom last to first
//        if(childs.size()>1)
//        {
//            float div = -(childs.size()*0.5f);
//            float offsetX = (width/2 - mouseX)*0.01;
//            float offsetY = (height/2 - mouseY)*0.01;
//            for (list<StackObject*>::reverse_iterator it = childs.rbegin(); it!=childs.rend(); ++it) 
//            {
//                glPushMatrix();
//                glTranslatef(offsetX*div, offsetY*div, 0);
//                (*it)->draw();
//                glPopMatrix();
//                div++;
//            }
//        }
//        else
//        {
//            childs.front()->draw();
//        }
//    }
    void moveleft();
//    {
//        DIRECTION=LEFT;
//        process();
//    }
    void moveright();
//    {
//        DIRECTION=RIGHT;
//        process();
//    }
    void pushLayer(StackObject * child);
//    {
//        if(childs.size()==0)
//        {
//            width = child->getWidth();
//        }
//        childs.push_back(child);
//    }
    void process();
//    {
//        if (DIRECTION == RIGHT || DIRECTION == LEFT )
//        {
//            if (DIRECTION == RIGHT && currentIndex<(childs.front()->size()))
//            {
//                currentIndex++;
//            }
//            if (DIRECTION == LEFT  && currentIndex>0)
//            {
//               currentIndex--;
//            }
//            float _width = childs.front()->getWidth()/childs.front()->size();
//            for (list<StackObject*>::iterator it = childs.begin(); it!=childs.end(); ++it) 
//            {
//                float div  = ((*it)->getWidth()-(_width*((*it!=childs.front())?2:1)))/childs.front()->getWidth();
//                //Tweener.addTween((*it)->x, div*_width*currentIndex*-1, 1);
//                (*it)->setParameters(1,easingquad,ofxTween::easeOut,(*it)->x,div*_width*currentIndex*-1,1000,0);
//            }
//            DIRECTION = NONE;
//            
//        }
//        
//    }
};
    
#endif
