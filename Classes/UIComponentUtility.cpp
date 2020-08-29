#include "UIComponentUtility.h"



cocos2d::ui::Button * UIComponentUtility::create_ui_button(const std::string &res_name, cocos2d::Vec2 anchor, cocos2d::Vec2 position, float scale)
{
    cocos2d::ui::Button * ui_btn = cocos2d::ui::Button::create(res_name);
    
    ui_btn->setAnchorPoint(anchor);
    ui_btn->setPosition(position);
    ui_btn->setScale(scale);
    
    
    return ui_btn;
}


cocos2d::Label * UIComponentUtility::create_ui_label(const std::string &text, const std::string &res_name, float font_size, cocos2d::Vec2 anchor, cocos2d::Vec2 position, const cocos2d::Color3B &color, bool enable_outline)
{
    cocos2d::Label * label = cocos2d::Label::createWithTTF(text, res_name, font_size);
    
    label->setAnchorPoint(anchor);
    label->setPosition(position);
    label->setColor(color);
    
    if (enable_outline) label->enableOutline(cocos2d::Color4B::BLACK, 1);
    
    
    return label;
}
