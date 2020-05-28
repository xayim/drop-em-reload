#ifndef Hinge_h
#define Hinge_h

#include "Body.h"


class Hinge : public Body
{
public:
    Hinge(b2World * world, RoundBodyData * object, cocos2d::Node * parent);
    
    void enable_rotation(float degrees);
    void stop_rotation();

    cocos2d::Sprite * sprite;
private:
    ~Hinge();
    
    friend class RopeStructure;
};


#endif /* Hinge_h */
