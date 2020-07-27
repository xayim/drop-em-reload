#ifndef MainMenuScreen_h
#define MainMenuScreen_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Background.h"
//#include "PluginIAP/PluginIAP.h"
#include "PluginSdkBoxAds/PluginSdkBoxAds.h"
#include "PluginReview/PluginReview.h"
//#include "PluginShare/PluginShare.h"



class MainMenuScreen : public cocos2d::Layer, public sdkbox::ReviewListener, public sdkbox::PluginSdkboxAdsListener
{
public:
    static cocos2d::Scene * create_scene();
    virtual bool init();
    
private:
    CREATE_FUNC(MainMenuScreen);
    ~MainMenuScreen();
    
    void init_ui();
    void init_background();
    
    cocos2d::ui::ScrollView * create_scrollview(unsigned int items_count);
    
    cocos2d::Sprite * create_sprite_retry_cnt();
    cocos2d::Sprite * create_sprite_coin_cnt();
    cocos2d::ui::Button * create_btn_options();
    cocos2d::ui::Button * create_btn_remove_ads();
    cocos2d::Label * create_label_retry_cnt();
    cocos2d::Label * create_label_coin_cnt();
    
    void callback_help(Ref * pSender);
    void callback_options(Ref * pSender);
    void callback_select_level(cocos2d::Ref *pSender);
    void callback_select_level_locked(cocos2d::Ref *pSender);
    void callback_get_retry(cocos2d::Ref *pSender);
    void callback_get_coin(cocos2d::Ref *pSender);
    void callback_remove_ads(cocos2d::Ref *pSender);
    
    
    
    // sdkbox inapp listener callbacks
//    virtual void onInitialized(bool success);
//    virtual void onSuccess(const sdkbox::Product& p);
//    virtual void onFailure(const sdkbox::Product& p, const std::string& msg);
//    virtual void onCanceled(const sdkbox::Product& p);
//    virtual void onRestored(const sdkbox::Product& p);
//    virtual void onProductRequestSuccess(const std::vector<sdkbox::Product>& products);
//    virtual void onProductRequestFailure(const std::string& msg);
//    virtual void onRestoreComplete(bool ok, const std::string &msg);
    
    
    // sdkbox ads listener callbacks
    virtual void onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type);
    virtual void onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed);
    
    // sdkbox admob listener callbacks
//    virtual void adViewDidReceiveAd(const std::string &name);
//    virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg);
//    virtual void adViewWillPresentScreen(const std::string &name);
//    virtual void adViewDidDismissScreen(const std::string &name);
//    virtual void adViewWillDismissScreen(const std::string &name);
//    virtual void adViewWillLeaveApplication(const std::string &name);
//    virtual void reward(const std::string &name, const std::string &currency, double amount);
//    // from 2.4.3.5+
//    virtual void adViewDidFailToPresentWithError(const std::string &name, const std::string &msg);
    
    
    
    // sdkbox review listener callbacks
    virtual void onDisplayAlert();
    virtual void onDeclineToRate();
    virtual void onRate();
    virtual void onRemindLater();
    
    
    
    // sdkbox share listener callbacks
//    virtual void onShareState(const sdkbox::SocialShareResponse& response);

    
    Background * background_ = nullptr;
    
//    cocos2d::ui::Button * btn_help_ = nullptr;
    cocos2d::ui::Button * btn_options_ = nullptr;
    cocos2d::Sprite * btn_coin_ = nullptr;
    cocos2d::Sprite * btn_retry_ = nullptr;
    cocos2d::ui::Button * btn_remove_ads = nullptr;
    cocos2d::Label * label_coin_cnt_ = nullptr;
    cocos2d::Label * label_retry_cnt_ = nullptr;
    cocos2d::ui::ScrollView * scrollview_ = nullptr;
    
    bool placement_interstitial_done;
};


#endif /* MainMenuScreen_h */
