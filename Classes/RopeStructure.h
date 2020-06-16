#ifndef RopeStructure_h
#define RopeStructure_h

#include "ui/CocosGUI.h"
#include "Hinge.h"
#include "Jammer.h"
#include "Rope.h"


struct RopeStructureData
{
    unsigned int id_local;
    RoundBodyData * jammer_object;
    std::vector<RoundBodyData *> hinge_object_list;
    std::vector<RopeData *> rope_object_list;
};


class RopeStructure
{
public:
    RopeStructure(b2World * world, RopeStructureData * object, cocos2d::Node * parent);
    ~RopeStructure();
    
    void update();
    void set_active(bool active);
    void reset();
    void destroy_jammer();
    unsigned int remove_rope(unsigned int index);
    cocos2d::ui::Button * create_button_hinge(unsigned int tag);
    cocos2d::ui::Button * create_button_select();
    cocos2d::ui::Button * create_button_reset();
    RopeStructureData * object() { return data_object; };
    unsigned int rope_count() { return (unsigned int)rope_list.size(); };
    
    
    Jammer * jammer;
    std::vector<Rope *> rope_list;
    std::vector<Hinge *> hinge_list;
    
private:
    void bind_joint(b2Body * body_a, b2Body * body_b, b2Vec2 anchor_a, b2Vec2 anchor_b);
    
    
    RopeStructureData * data_object;
    b2World * world;
    cocos2d::Node * parent;
};

#endif /* RopeStructure_h */
