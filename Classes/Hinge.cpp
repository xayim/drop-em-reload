#include "Hinge.h"
#include "Definitions.h"
#include "Utility.h"


Hinge::Hinge(b2World * world, RoundBodyData * object, cocos2d::Node * parent)
{
    b2FixtureDef fixture_def;
    b2BodyDef body_def;
    
    body_def.position.Set(object->position.x, object->position.y);
    body_def.angle = object->angle;
    body_def.type = (b2BodyType)object->body_type;
    body = (b2Body *)world->CreateBody(&body_def);
    
    
    b2CircleShape shape;
    
    shape.m_radius = object->radius;
    fixture_def.shape = &shape;
    body->CreateFixture(&fixture_def);
    
    
    sprite = cocos2d::Sprite::create("res/game/hinge.png");
    sprite->setPosition(cocos2d::Vec2(body->GetPosition().x, body->GetPosition().y)*PTM_RATIO);
    sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
    sprite->setScale(PTM_RATIO/sprite->getContentSize().height*0.4);
    sprite->setColor(cocos2d::Color3B(223, 196, 145));
    
    parent->addChild(sprite, 1);
}


Hinge::~Hinge()
{
    body->GetWorld()->DestroyBody(body);
    body = NULL;
    
    sprite->removeFromParent();
    sprite = NULL;
}


void Hinge::enable_rotation(float degrees)
{
    sprite->runAction(cocos2d::RepeatForever::create(cocos2d::RotateBy::create(.5, degrees)));
}


void Hinge::stop_rotation()
{
    sprite->stopAllActions();
}
