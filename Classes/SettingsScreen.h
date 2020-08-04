#ifndef SettingsScreen_h
#define SettingsScreen_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PluginSdkBoxAds/PluginSdkBoxAds.h"


class SettingsScreen : public cocos2d::Layer, public sdkbox::PluginSdkboxAdsListener
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
//    void callback_store(cocos2d::Ref *pSender);
    void callback_help(cocos2d::Ref *pSender);
    
    void layout_buttons();
    
    
    // sdkbox ads listener callbacks
    virtual void onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type);
    virtual void onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed);
    
    
    cocos2d::ui::Button * btn_back_ = nullptr;
    cocos2d::ui::Button * btn_sfx_ = nullptr;
    cocos2d::ui::Button * btn_music_ = nullptr;
    cocos2d::ui::Button * btn_gameplay_ = nullptr;
//    cocos2d::ui::Button * btn_store_ = nullptr;
    cocos2d::ui::Button * btn_help_ = nullptr;
    cocos2d::Node * panel_ = nullptr;
};


#endif /* SettingsScreen_h */
