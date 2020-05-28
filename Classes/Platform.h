#ifndef Platform_h
#define Platform_h

#include "Body.h"


class Platform : public Body
{
public:
    Platform(b2World * world, PolygonBodyData * object);
    ~Platform();
    
    void draw(cocos2d::DrawNode * draw_node);
    
    cocos2d::Vec2 * vertices;
    b2Vec2 * b2_vertices;
    unsigned int vertex_count;
private:
    
};

#endif /* Platform_h */
