#include "Background.h"
#include "Utility.h"


Background * Background::create(unsigned int level_id)
{
    Background * background = Background::create();
    
    return background;
}

bool Background::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    float scale = Utility::content_scale();
    
    background_0_ = cocos2d::Sprite::create("res/background/01_underwater/background_0.png");
    background_0_->setAnchorPoint(cocos2d::Vec2(1, 0.5));
    background_0_->setScale(scale);
    addChild(background_0_);
    
    background_1_ = cocos2d::Sprite::create("res/background/01_underwater/background_1.png");
    background_1_->setAnchorPoint(cocos2d::Vec2(0, 0.5));
    background_1_->setScale(scale);
    addChild(background_1_);
    
    setContentSize(cocos2d::Size(background_0_->getContentSize().width * 2, background_0_->getContentSize().height) * scale);
    
    return true;
}

Background::~Background()
{
//    background_0_->removeFromParent();
//    background_1_->removeFromParent();
}
