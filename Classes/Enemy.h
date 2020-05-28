#ifndef Enemy_h
#define Enemy_h

#include <stdio.h>
#include "Body.h"


class Enemy : public Body
{
public:
    Enemy(b2World * world, PolygonBodyData * object, cocos2d::Node * parent);
    ~Enemy();
    
    virtual void update() override;
    
    cocos2d:: Sprite * sprite;
    
private:
    PolygonBodyData * data_object;
    
};

#endif /* Enemy_h */
