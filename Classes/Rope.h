#ifndef Rope_h
#define Rope_h

#include "Body.h"
#include "RopeBody.h"

struct RopeData
{
    unsigned int id_local;
    std::vector<PolygonBodyData *> rope_body_object_list;
};


class Rope
{
public:
    Rope(b2World * world, RopeData * object, cocos2d::Node * parent);
    
    void update();
    void set_active(bool active);
    
    
    std::vector<RopeBody *> rope_body_list;
private:
    ~Rope();
    
    friend class RopeStructure;
};


#endif /* Rope_h */
