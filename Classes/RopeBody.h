#ifndef RopeBody_h
#define RopeBody_h

#include "Body.h"


class RopeBody : public Body
{
public:
    RopeBody(b2World * world, PolygonBodyData * object, cocos2d::Node * parent);
    ~RopeBody();
    
    virtual void update() override;
    void set_active(bool active);
    
    cocos2d::Sprite * sprite;
    
private:
    
    friend class Rope;
    
};


#endif /* RopeBody_h */
