#ifndef UIComponentUtility_h
#define UIComponentUtility_h


#include "ui/CocosGUI.h"


class UIComponentUtility
{
public:
    static cocos2d::ui::Button * create_ui_button(const std::string &res_name,
                                                  cocos2d::Vec2 anchor,
                                                  cocos2d::Vec2 position,
                                                  float scale);
    
    static cocos2d::Label * create_ui_label(const std::string &text,
                                            const std::string &res_name,
                                            float font_size,
                                            cocos2d::Vec2 anchor,
                                            cocos2d::Vec2 position,
                                            const cocos2d::Color3B &color,
                                            bool enable_outline);
    
};

#endif /* UIComponentUtility_h */
