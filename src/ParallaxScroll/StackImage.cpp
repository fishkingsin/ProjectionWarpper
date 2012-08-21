#include "StackImage.h"
StackImage::StackImage()
{
    set(0,0,0,0);
}
void StackImage::pushImage(MDraws *child)
{
    childs.push_back(child);
    width+=child->getWidth();
}
void StackImage::update()
{
    for (vector<MDraws *>::iterator it = childs.begin(); it!=childs.end(); ++it) 
    {
        (*it)->update();
    }
}
void StackImage::draw()
{
    //if(isCompleted())
    //{
    x = ofxTween::update();
    //}
    glPushMatrix();
    //glTranslatef(x, y, 0);
    int _width=0;
    for (vector<MDraws *>::iterator it = childs.begin(); it!=childs.end(); ++it) 
    {
        if(x+_width+(*it)->getWidth() >0 && x+_width-(*it)->getWidth()*0.5 < ofGetWidth())
        {
            ofPushStyle();
            ofEnableAlphaBlending();

            //float alpha = ofMap(abs(x+_width+(*it)->getWidth()/2+ofGetWidth()/4), 0, ofGetWidth()/2, 0, 1);
            float mpos = x+_width-(*it)->getWidth()/2;

            ofSetColor(255,static_alpha);
            (*it)->set(_width+x, 0, (*it)->getWidth(),(*it)->getHeight());
            (*it)->draw(0,0,(*it)->getWidth(),(*it)->getHeight());//_width,0,(*it)->getWidth(),(*it)->getHeight());
            ofPopStyle();
        }
        _width+=(*it)->getWidth();
    }
    glPopMatrix();
    
}
int StackImage::size()
{
    return childs.size();
}  