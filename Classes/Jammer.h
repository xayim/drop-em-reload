#ifndef Jammer_h
#define Jammer_h

#include "Body.h"


class Jammer : public Body
{
public:
    Jammer(b2World * world, RoundBodyData * object, cocos2d::Node * parent);
    
    virtual void update() override;
    
    cocos2d::Sprite * sprite;
private:
    ~Jammer();
    void set_active(bool active);
    
    friend class RopeStructure;
};

#endif /* Jammer_h */
