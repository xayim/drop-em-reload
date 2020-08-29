#ifndef MainMenuScreen_h
#define MainMenuScreen_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Background.h"
//#include "PluginIAP/PluginIAP.h"
#include "PluginSdkBoxAds/PluginSdkBoxAds.h"
#include "PluginReview/PluginReview.h"



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
    
    void callback_options(Ref * pSender);
    void callback_select_level(cocos2d::Ref *pSender);
    void callback_select_level_locked(cocos2d::Ref *pSender);
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
    
    
    // sdkbox review listener callbacks
    virtual void onDisplayAlert();
    virtual void onDeclineToRate();
    virtual void onRate();
    virtual void onRemindLater();

    
    Background * background_ = nullptr;
    
    cocos2d::ui::Button * btn_options_ = nullptr;
    cocos2d::ui::Button * btn_remove_ads_ = nullptr;
    cocos2d::ui::ScrollView * scrollview_ = nullptr;
    cocos2d::Label * label_menu_ = nullptr;
    
    bool placement_interstitial_done;
};


#endif /* MainMenuScreen_h */
