#ifndef SettingsScreen_h
#define SettingsScreen_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"


class SettingsScreen : public cocos2d::Layer
{
public:
    static cocos2d::Scene * create_scene();
    virtual bool init();
    
private:
    CREATE_FUNC(SettingsScreen);
    ~SettingsScreen();
    
    void callback_back(cocos2d::Ref *pSender);
    void callback_music(cocos2d::Ref *pSender);
    void callback_sfx(cocos2d::Ref *pSender);
    void callback_gameplay(cocos2d::Ref *pSender);
    void callback_gameplay_hinge(cocos2d::Ref *pSender);
    void callback_gameplay_select(cocos2d::Ref *pSender);
    void callback_store(cocos2d::Ref *pSender);
    void callback_help(cocos2d::Ref *pSender);
    
    void layout_buttons();
    
    cocos2d::ui::Button * btn_back = nullptr;
    cocos2d::ui::Button * btn_sfx = nullptr;
    cocos2d::ui::Button * btn_music = nullptr;
    cocos2d::ui::Button * btn_gameplay = nullptr;
    cocos2d::ui::Button * btn_store = nullptr;
    cocos2d::ui::Button * btn_help = nullptr;
    cocos2d::Node * panel = nullptr;
};


#endif /* SettingsScreen_h */
