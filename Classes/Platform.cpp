#include "Platform.h"
#include "Definitions.h"
#include "Utility.h"


Platform::Platform(b2World * world, PolygonBodyData * object)
{
    b2BodyDef body_def;
    
    body_def.position.Set(object->position.x, object->position.y);
    body_def.angle = object->angle;
    body_def.type = (b2BodyType)object->body_type;
    
    body = (b2Body *)world->CreateBody(&body_def);
    
    
    
    b2FixtureDef fixture_def;
    b2ChainShape shape;
    
    vertex_count = object->vertex_count;
    b2_vertices = new b2Vec2[vertex_count];
    vertices = new cocos2d::Vec2[vertex_count];
    
    for (unsigned int j = 0; j < vertex_count; ++j)
    {
        b2_vertices[j].Set(object->vertices[j].x, object->vertices[j].y);
        
        vertices[j].set((body->GetPosition().x+b2_vertices[j].x)*PTM_RATIO,
                        (body->GetPosition().y+b2_vertices[j].y)*PTM_RATIO);
    }
    
    shape.CreateChain(b2_vertices, vertex_count);
    fixture_def.shape = &shape;
    fixture_def.restitution = 0.2;
    
    body->CreateFixture(&fixture_def);
}


Platform::~Platform()
{
    delete [] b2_vertices;
    delete [] vertices;
    
    body->GetWorld()->DestroyBody(body);
    body = NULL;
}


void Platform::draw(cocos2d::DrawNode * draw_node)
{
    draw_node->drawPoly(vertices, vertex_count, false, cocos2d::Color4F(156.f/255.f, 78.f/255.f, 200.f/255.f, 1.f));
}
