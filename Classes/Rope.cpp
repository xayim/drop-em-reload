#include "Rope.h"


Rope::Rope(b2World * world, RopeData * object, cocos2d::Node * parent)
{
//    this->object = object;
    
    for (unsigned int i = 0; i < object->rope_body_object_list.size(); ++i)
    {
        RopeBody * rope_body = new RopeBody(world, object->rope_body_object_list.at(i), parent);
        
        if (i == 0) rope_body->sprite->setVisible(false);
        
        rope_body_list.push_back(rope_body);
    }
    
    
    for (unsigned int i = 1; i < rope_body_list.size(); ++i)
    {
        b2RevoluteJointDef joint_def;
        
        joint_def.enableLimit = false;
        joint_def.enableMotor = false;
        joint_def.collideConnected = false;
        joint_def.lowerAngle = 0.f;
        joint_def.upperAngle = 0.f;
        joint_def.referenceAngle = 0.f;
        joint_def.motorSpeed = 0.f;
        joint_def.maxMotorTorque = 0.f;
        joint_def.bodyA = rope_body_list.at(i)->body;
        joint_def.bodyB = rope_body_list.at(i-1)->body;
        //        joint_def.bodyA = Utility::find_body(joint_object->name_a, this);
        //        joint_def.bodyB = Utility::find_body(joint_object->name_b, this);
        joint_def.localAnchorA.Set(0.0, -0.2);
        joint_def.localAnchorB.Set(0.0, 0.2);
        
        
        world->CreateJoint(&joint_def);
    }
}


Rope::~Rope()
{
    for (unsigned int i = 0; i < rope_body_list.size(); ++i)    rope_body_list.at(i)->~RopeBody();
}


void Rope::set_active(bool active)
{
    for (unsigned int i = 0; i < rope_body_list.size(); ++i)    rope_body_list.at(i)->set_active(active);
}


void Rope::update()
{
    for (unsigned int i = 0; i < rope_body_list.size(); ++i)
    {
        rope_body_list.at(i)->update();
    }
}
