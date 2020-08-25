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
    void set_retry_count_label(const std::string &label);
    void reset();
    void destroy_jammer();
    void remove_rope(unsigned int index);
    void remove_select_button();
    void remove_reset_button();
    void remove_reset_count_label();
    cocos2d::ui::Button * create_button_hinge(unsigned int tag);
    cocos2d::ui::Button * create_button_select();
    cocos2d::ui::Button * create_button_reset();
    cocos2d::Label * create_label_retry_count(const std::string &label);
    RopeStructureData * object() { return data_object; };
    unsigned int rope_count() { return (unsigned int)rope_list.size(); };
    
    
    Jammer * jammer;
    std::vector<Rope *> rope_list;
    std::vector<Hinge *> hinge_list;
    
    
    cocos2d::Label * label_retry_cnt_;
    cocos2d::ui::Button * btn_select_;
    cocos2d::ui::Button * btn_reset_;
    
private:
    void bind_joint(b2Body * body_a, b2Body * body_b, b2Vec2 anchor_a, b2Vec2 anchor_b);
    
    
    RopeStructureData * data_object;
    b2World * world;
    cocos2d::Node * parent;
//    cocos2d::ui::Button * btn_select_;
//    cocos2d::ui::Button * btn_reset_;
//    cocos2d::Label * label_retry_cnt_;
};

#endif /* RopeStructure_h */
