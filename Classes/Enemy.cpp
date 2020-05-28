#include "Enemy.h"
#include "Definitions.h"
#include "Utility.h"


Enemy::Enemy(b2World * world, PolygonBodyData * object, cocos2d::Node * parent)
{
    this->data_object = object;
    
    
    b2BodyDef body_def;
    
    body_def.position.Set(object->position.x, object->position.y);
    body_def.angle = object->angle;
    body_def.type = (b2BodyType)object->body_type;
    
    body = (b2Body *)world->CreateBody(&body_def);
    
    
    
    b2FixtureDef fixture_def;
    b2PolygonShape shape;
    
    b2Vec2 * vertices = new b2Vec2[object->vertex_count];
    
    for (unsigned int j = 0; j < object->vertex_count; ++j)
    {
        vertices[j].Set(object->vertices[j].x, object->vertices[j].y);
    }
    
    
    shape.Set(vertices, object->vertex_count);
    fixture_def.shape = &shape;
    fixture_def.density = 0.25;
    
    body->CreateFixture(&fixture_def);
    
    
    sprite = cocos2d::Sprite::create("res/game/enemy.png");
    sprite->setPosition(cocos2d::Vec2(body->GetPosition().x, body->GetPosition().y)*PTM_RATIO);
    sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
    sprite->setScale(PTM_RATIO/sprite->getContentSize().height);
    sprite->setColor(cocos2d::Color3B(178, 56, 133));
    
    
    parent->addChild(sprite);
}


Enemy::~Enemy()
{
    body->GetWorld()->DestroyBody(body);
    body = NULL;
    
    sprite->removeFromParent();
    sprite = NULL;
}


void Enemy::update()
{
    sprite->setPosition(cocos2d::Vec2(body->GetPosition().x, body->GetPosition().y)*PTM_RATIO);
    sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
}
