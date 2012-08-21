#include "ofxParallaxScroll.h"

void ofxParallaxScroll::setup(int _width, int _height)
{
    ofLog(OF_LOG_VERBOSE,string(typeid(this).name())+" "+string(__FUNCTION__));
    set(0,0,0,0);
    currentIndex  =0;
    buttonLock = false;
    DIRECTION = NONE;
    width = _width;
    height = _height;
}

void ofxParallaxScroll::update()
{
    //        process();
}
void ofxParallaxScroll::draw()
{
//draw the list from top to bottom last to first
        if(childs.size()>1)
        {
            float div = -(childs.size()*0.5f);
            float offsetX = (width/2 - mouseX)*0.01;
            float offsetY = (height/2 - mouseY)*0.01;
            for (list<StackObject*>::reverse_iterator it = childs.rbegin(); it!=childs.rend(); ++it) 
            {
                glPushMatrix();
                glTranslatef(offsetX*div, offsetY*div, 0);
                (*it)->draw();
                glPopMatrix();
                div++;
            }
        }
        else
        {
            childs.front()->draw();
        }
}
void ofxParallaxScroll::moveleft()
{
    DIRECTION=LEFT;
    process();
}
void ofxParallaxScroll::moveright()
{
    DIRECTION=RIGHT;
    process();
}
void ofxParallaxScroll::pushLayer(StackObject * child)
{
//    if(childs.size()==0)
//    {
//        width = child->getWidth();
//    }
    childs.push_back(child);
}
void ofxParallaxScroll::process()
{
    if (DIRECTION == RIGHT || DIRECTION == LEFT )
    {
        if (DIRECTION == RIGHT && currentIndex<(childs.front()->size()-2))
        {
            currentIndex++;
        }
        if (DIRECTION == LEFT  && currentIndex>0)
        {
            currentIndex--;
        }
        float _width = childs.front()->getWidth()/childs.front()->size()*1.0f;
        float i = 1.0f;
        for (list<StackObject*>::iterator it = childs.begin(); it!=childs.end(); ++it) 
        {
            float div  = ((*it)->getWidth()-(_width*((*it!=childs.front())?2:0)))/childs.front()->getWidth();
            //Tweener.addTween((*it)->x, div*_width*currentIndex*-1, 1);
            (*it)->setParameters(1,easingquad,ofxTween::easeOut,(*it)->x,div*_width *currentIndex*-1,1000,0);
            i++;
        }
        DIRECTION = NONE;
        
    }
    
}

