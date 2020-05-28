#ifndef Body_h
#define Body_h

#include "cocos2d.h"
#include "external/Box2D/include/Box2D/Box2D.h"


struct BodyData
{
    unsigned int id_global = 0;
    unsigned int body_type = 0;
    float angle = 0;
    float friction = 0;
    float restitution = 0;
    float density = 0;
    bool sensor = false;
    cocos2d::Vec2 position = cocos2d::Vec2::ZERO;
};

struct RoundBodyData : BodyData
{
    float radius = 0;
};

struct PolygonBodyData : BodyData
{
    unsigned int vertex_count = 0;
    cocos2d::Vec2 * vertices = nullptr;
    
    ~PolygonBodyData()
    {
        delete vertices;
    }
};

struct JointData
{
    cocos2d::Vec2 anchor_a = cocos2d::Vec2::ZERO;
    cocos2d::Vec2 anchor_b = cocos2d::Vec2::ZERO;
    bool enable_limit = false;
    bool enable_motor = false;
    float joint_speed = 0;
    float lower_limit = 0;
    float upper_limit = 0;
    float max_motor_torque = 0;
    float motor_speed = 0;
    float ref_angle = 0;
    std::string name_a = "";
    std::string name_b = "";
};

class Body
{
public:
    Body();
    
    virtual void update();
    
    unsigned int id_global;
    unsigned int id_local;
    b2Body * body;
    
protected:
    
};



#endif /* Body_h */
