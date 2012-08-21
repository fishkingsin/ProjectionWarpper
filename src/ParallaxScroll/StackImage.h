//
//  ofxParallaxScroll.h
//  ParallaxScrolling
//
//  Created by james KONG on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ParallaxScrolling_StackImage_h
#define ParallaxScrolling_StackImage_h
#include "StackObject.h"

class StackImage : public  StackObject
{
    
public:
    StackImage();
    void pushImage(MDraws *child);
    
    void draw();
    int size();
    void update();
    float getWidth()
    {
        return width;
    }

    float getWidth(int i)
    {
        return childs[i]->getWidth();
    }

    
    
    
    
};
#endif
