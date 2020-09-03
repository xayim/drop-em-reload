#include "RopeStructure.h"
#include "Definitions.h"
#include "GameSettings.h"
#include "UIComponentUtility.h"
#include "Utility.h"


RopeStructure::RopeStructure(b2World * world, RopeStructureData * object, cocos2d::Node * parent) : btn_select_(nullptr), btn_reset_(nullptr)
{
    this->world = world;
    this->data_object = object;
    this->parent = parent;
    
    
    jammer = new Jammer(world, object->jammer_object, parent);
    
    
    for (unsigned int i = 0; i < object->hinge_object_list.size(); ++i)
    {
        Hinge * hinge = new Hinge(world, object->hinge_object_list.at(i), parent);
        
        if (i == 0) hinge->enable_rotation(-15);
        else        hinge->enable_rotation(15);
        
        hinge_list.push_back(hinge);
    }
    
    
    for (unsigned int i = 0; i < object->rope_object_list.size(); ++i)
    {
        Rope * rope = new Rope(world, object->rope_object_list.at(i), parent);
        
        bind_joint(hinge_list.at(i)->body, rope->rope_body_list.at(rope->rope_body_list.size()-1)->body, b2Vec2(0.0, 0.0), b2Vec2 (0.0, 0.2));
        bind_joint(jammer->body, rope->rope_body_list.at(0)->body, b2Vec2(0.0, 0.0), b2Vec2 (0.0, -0.2));
        
        rope_list.push_back(rope);
    }
}

RopeStructure::~RopeStructure()
{
    if (jammer)     jammer->~Jammer();
    delete data_object->jammer_object;
    
    for (unsigned int i = 0; i < hinge_list.size(); ++i)
    {
        hinge_list.at(i)->~Hinge();
        
        delete data_object->hinge_object_list.at(i);
    }

    for (unsigned int i = 0; i < rope_list.size(); ++i)
    {
        rope_list.at(i)->~Rope();
        delete data_object->rope_object_list.at(i);
    }
}


void RopeStructure::set_active(bool active)
{
    is_active_ = active;
    
    if (rope_list.size() > 0)
    {
        jammer->set_active(active);
        
        for (unsigned int i = 0; i < rope_list.size(); ++i) rope_list.at(i)->set_active(active);
    }
}


void RopeStructure::reset()
{
    jammer = new Jammer(world, data_object->jammer_object, parent);
    
    for (unsigned int i = 0; i < data_object->rope_object_list.size(); ++i)
    {
        Rope * rope = new Rope(world, data_object->rope_object_list.at(i), parent);
        
        bind_joint(hinge_list.at(i)->body, rope->rope_body_list.at(rope->rope_body_list.size()-1)->body, b2Vec2(0.0, 0.0), b2Vec2 (0.0, 0.2));
        bind_joint(jammer->body, rope->rope_body_list.at(0)->body, b2Vec2(0.0, 0.0), b2Vec2 (0.0, -0.2));
        
        rope_list.push_back(rope);
    }
}


void RopeStructure::destroy_jammer()
{
    jammer->~Jammer();
    jammer = NULL;
}


void RopeStructure::remove_rope(unsigned int index)
{
    Rope * rope = rope_list.at(index%rope_list.size());
    
    
    for (std::vector<Rope *>::iterator itr = rope_list.begin(); itr != rope_list.end(); ++itr)
    {
        if (rope == *itr)
        {
            rope->~Rope();
            rope_list.erase(itr);
            break;
        }
    }
}


void RopeStructure::bind_joint(b2Body *body_a, b2Body *body_b, b2Vec2 anchor_a, b2Vec2 anchor_b)
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
    joint_def.bodyA = body_a;
    joint_def.bodyB = body_b;
    joint_def.localAnchorA.Set(anchor_a.x, anchor_a.y);
    joint_def.localAnchorB.Set(anchor_b.x, anchor_b.y);
    
    
    world->CreateJoint(&joint_def);
}


cocos2d::ui::Button * RopeStructure::create_button_hinge(unsigned int tag)
{
    cocos2d::ui::Button * btn_hinge = cocos2d::ui::Button::create("res/ui/btn_hook.png", "res/ui/btn_hook_pressed.png", "res/ui/btn_hook.png");
    
    btn_hinge->setAnchorPoint(cocos2d::Vec2(0.5, 0));
    btn_hinge->setPosition(cocos2d::Vec2(hinge_list.at(tag)->sprite->getPosition().x, hinge_list.at(tag)->sprite->getPosition().y+hinge_list.at(tag)->sprite->getContentSize().height/2*hinge_list.at(tag)->sprite->getScale()));
    
    btn_hinge->setScale(0.4f, ((2048.f/2)*Utility::content_scale())-btn_hinge->getPosition().y);
    btn_hinge->setOpacity(191);
    btn_hinge->setTag(tag);
    btn_hinge->setUserData(this);
    btn_hinge->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
    
    
    btn_hinge_.push_back(btn_hinge);
    
    return btn_hinge;
}


cocos2d::ui::Button * RopeStructure::create_button_select()
{
    btn_select_ = cocos2d::ui::Button::create("res/ui/btn_select_rope_structure.png");
    btn_select_->setPosition(cocos2d::Vec2(jammer->sprite->getPosition().x, jammer->sprite->getPosition().y+jammer->sprite->getContentSize().height/2*jammer->sprite->getScale()));
    btn_select_->setScale((hinge_list.at(1)->sprite->getPosition().x-hinge_list.at(0)->sprite->getPosition().x)/btn_select_->getContentSize().width,
                          ((hinge_list.at(1)->sprite->getPosition().x-hinge_list.at(0)->sprite->getPosition().x)/btn_select_->getContentSize().width)/2);
    
    btn_select_->setOpacity(0);
    btn_select_->setUserData(this);
    btn_select_->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
    
    return btn_select_;
}


cocos2d::ui::Button * RopeStructure::create_button_reset()
{
    btn_reset_ = UIComponentUtility::create_ui_button("res/ui/btn_retry.png",
                                                      cocos2d::Vec2(0.5, 0.5), cocos2d::Vec2((data_object->hinge_object_list.at(0)->position.x+data_object->hinge_object_list.at(1)->position.x)/2, (data_object->hinge_object_list.at(0)->position.y+data_object->hinge_object_list.at(1)->position.y)/2)*PTM_RATIO,
                                                      Utility::content_scale());


    btn_reset_->setColor(cocos2d::Color3B(192, 177, 170));
    btn_reset_->setUserData(this);
    btn_reset_->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
    btn_reset_->runAction(cocos2d::RepeatForever::create(cocos2d::RotateBy::create(.5, 15)));

    return btn_reset_;
}


void RopeStructure::remove_reset_button()
{
    btn_reset_->removeFromParent();
    btn_reset_ = NULL;
}


void RopeStructure::remove_select_button()
{
    btn_select_->removeFromParent();
    btn_select_ = NULL;
}


void RopeStructure::remove_hinge_button(cocos2d::ui::Button * btn_hinge)
{
    std::vector<cocos2d::ui::Button *>::iterator btn_del_itr;
    
        
//    btn_del_itr = std::find(btn_hinge_.begin(), btn_hinge_.end(), btn_hinge);
    
    btn_hinge_.erase(std::find(btn_hinge_.begin(), btn_hinge_.end(), btn_hinge));
    
    btn_hinge->removeFromParent();
}


void RopeStructure::enable_reset_button(bool enable)
{
    btn_reset_->setEnabled(enable);
}


void RopeStructure::enable_select_button(bool enable)
{
    btn_select_->setEnabled(enable);
}


void RopeStructure::enable_hinge_button(bool enable)
{
    for (unsigned int i = 0; i < btn_hinge_.size(); ++i)    btn_hinge_.at(i)->setEnabled(enable);
}


void RopeStructure::update()
{
    if (jammer)     jammer->update();
    
    for (unsigned int i = 0; i < rope_list.size(); ++i)
    {
        rope_list.at(i)->update();
    }
}
