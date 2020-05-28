#include "Jammer.h"
#include "Definitions.h"
#include "Utility.h"


Jammer::Jammer(b2World * world, RoundBodyData * object, cocos2d::Node * parent)
{
    b2BodyDef body_def;
    
    body_def.position.Set(object->position.x, object->position.y);
    body_def.angle = object->angle;
    body_def.type = (b2BodyType)object->body_type;
    
    body = (b2Body *)world->CreateBody(&body_def);
    
    
    b2FixtureDef fixture_def;
    b2CircleShape shape;
    
    shape.m_radius = object->radius;
    fixture_def.shape = &shape;
    fixture_def.density = 10;
    
    body->CreateFixture(&fixture_def);
    
    
    sprite = cocos2d::Sprite::create("res/game/jammer.png");
    sprite->setPosition(cocos2d::Vec2(body->GetPosition().x, body->GetPosition().y)*PTM_RATIO);
    sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
    sprite->setScale(PTM_RATIO/sprite->getContentSize().height*0.8);
    sprite->setColor(cocos2d::Color3B(187, 249, 107));
    sprite->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
    
    
    parent->addChild(sprite, 1);
}


Jammer::~Jammer()
{
    body->GetWorld()->DestroyBody(body);
    body = NULL;
    
    sprite->removeFromParent();
    sprite = NULL;
}


void Jammer::set_active(bool active)
{
    cocos2d::Color3B color = sprite->getColor();
    
    if (active) sprite->setColor(cocos2d::Color3B(color.r*BRIGHT_FACTOR, color.g*BRIGHT_FACTOR, color.b*BRIGHT_FACTOR));
    else        sprite->setColor(cocos2d::Color3B(color.r/BRIGHT_FACTOR, color.g/BRIGHT_FACTOR, color.b/BRIGHT_FACTOR));
}


void Jammer::update()
{
    sprite->setPosition(cocos2d::Vec2(body->GetPosition().x, body->GetPosition().y)*PTM_RATIO);
    sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
}
