#include "MainMenuScreen.h"
#include "Audio.h"
#include "Background.h"
#include "Definitions.h"
#include "GameplayScreen.h"
#include "GameSettings.h"
#include "PlayerProfile.h"
#include "SettingsScreen.h"
#include "TitleScreen.h"
#include "Utility.h"


cocos2d::Scene * MainMenuScreen::create_scene()
{
    cocos2d::Scene * scene = cocos2d::Scene::create();
    
    MainMenuScreen * main_menu_screen = MainMenuScreen::create();
    
    scene->addChild(main_menu_screen);
    

    return scene;
}

bool MainMenuScreen::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    init_background();
    
    init_ui();
    
    
//    sdkbox::IAP::setListener(this);
//    sdkbox::IAP::refresh();
    
//    sdkbox::PluginSdkboxAds::setListener(this);
    
    
    sdkbox::PluginReview::setListener(this);
    sdkbox::PluginReview::show();
    
    
    
    
//    sdkbox::SocialShareInfo share_info;
//
//    share_info.title = "Awesome Game!";
//    share_info.showDialog = true;
//    share_info.text = "I play Drop 'Em, you should play too!";
//    share_info.platform = sdkbox::SocialPlatform::Platform_Select;
//    share_info.link = "https://fb.me/529188751338446";
//    
//    
//    sdkbox::PluginShare::setListener(this);
//    sdkbox::PluginShare::share(share_info);
    
    
    
    
    
    
    
    
    return true;
}


MainMenuScreen::~MainMenuScreen()
{
    //    sdkbox::PluginSdkboxAds::hide("placement-banner");
    //    sdkbox::PluginShare::removeListener();
    //    sdkbox::IAP::removeListener();
    //    sdkbox::PluginReview::removeListener();
}


void MainMenuScreen::init_background()
{
    background_ = Background::create(0);
    background_->setPosition(Utility::window_center());
    addChild(background_);
}


void MainMenuScreen::init_ui()
{
    scrollview_ = create_scrollview(Utility::file_count("level_data"));
    addChild(scrollview_);
    
    
    btn_retry_ = create_sprite_retry_cnt();
    btn_retry_->setPosition(cocos2d::Vec2(Utility::ui_left(), Utility::ui_top()));
    addChild(btn_retry_);
    
    label_retry_cnt_ = create_label_retry_cnt();
    label_retry_cnt_->setPosition(cocos2d::Vec2(btn_retry_->getPosition().x+(btn_retry_->getContentSize().width*btn_retry_->getScale()),
                                                btn_retry_->getPosition().y-(btn_retry_->getContentSize().height*btn_retry_->getScale())));
    addChild(label_retry_cnt_);
    
    
    btn_coin_ = create_sprite_coin_cnt();
    btn_coin_->setPosition(cocos2d::Vec2(Utility::ui_left(), Utility::ui_bottom()));
    addChild(btn_coin_);
    
    label_coin_cnt_ = create_label_coin_cnt();
    label_coin_cnt_->setPosition(cocos2d::Vec2(btn_coin_->getPosition().x+(btn_coin_->getContentSize().width*btn_coin_->getScale()),
                                               btn_coin_->getPosition().y));
    addChild(label_coin_cnt_);
    
    
    btn_options_ = create_btn_options();
    btn_options_->setPosition(cocos2d::Vec2(Utility::ui_right(), Utility::ui_top()));
    addChild(btn_options_);
    
    
    if (PlayerProfile::ads_enabled())
    {
        btn_remove_ads = create_btn_remove_ads();
        btn_remove_ads->setPosition(cocos2d::Vec2(Utility::ui_right(), Utility::ui_bottom()));
        addChild(btn_remove_ads);
    }
}


cocos2d::ui::ScrollView * MainMenuScreen::create_scrollview(unsigned int items_count)
{
    cocos2d::Size window_size = Utility::window_size();
    cocos2d::Vec2 window_center = Utility::window_center();
    
    
    cocos2d::ui::ScrollView * scrollview = cocos2d::ui::ScrollView::create();
    scrollview->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollview->setContentSize(window_size);
    scrollview->setInnerContainerSize(cocos2d::Size(320*(items_count-1)+(window_size.width), window_size.height));
    scrollview->setTouchEnabled(true);
    scrollview->setBounceEnabled(true);
    scrollview->setScrollBarEnabled(false);
    scrollview->scrollToPercentHorizontal(GameSettings::scroll_percentage(), 0, false);
    
    
    for (unsigned int i = 0; i < items_count; ++i)
    {
        cocos2d::Label * level_title = cocos2d::Label::createWithTTF(std::to_string(i+1), FONT_SKRANJI_BOLD, 48);
        level_title->setColor(cocos2d::Color3B::WHITE);
        level_title->enableOutline(cocos2d::Color4B::BLACK, 1);
        
        
        std::stringstream string_stream;
        string_stream << "res/ui/base_level.png";
        bool is_locked = false;
        
        if (!UNLOCK_LEVELS)
        {
            string_stream.str("");
            string_stream << "level_unlocked" << i;
            
            if (i == 0)
            {
                string_stream.str("");
                string_stream << "res/ui/base_level.png";
                is_locked = false;
            }
            else if (!cocos2d::UserDefault::getInstance()->getBoolForKey(string_stream.str().c_str(), false))
            {
                string_stream.str("");
                string_stream << "res/ui/base_level_inactive.png";
                is_locked = true;
            }
        }
        
        
        cocos2d::ui::Button * level_button = cocos2d::ui::Button::create(string_stream.str().c_str());
        level_button->setTitleLabel(level_title);
        level_button->setTag(i);
        level_button->setZoomScale(0.f);
        
        if (!is_locked)
            level_button->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_select_level, this));
        else
            level_button->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_select_level_locked, this));
        
        
        
        cocos2d::ui::Layout * level_button_layout = cocos2d::ui::Layout::create();
        level_button_layout->setPosition(cocos2d::Vec2(window_center.x+(i*320), window_center.y));
        level_button_layout->setContentSize(level_button->getContentSize());
        level_button_layout->addChild(level_button);
        
        
        scrollview->addChild(level_button_layout);
    }
    
    
    return scrollview;
}


cocos2d::Sprite * MainMenuScreen::create_sprite_retry_cnt()
{
    cocos2d::Sprite * spr_retry = cocos2d::Sprite::create("res/ui/btn_retry_2.png");
    
    spr_retry->setAnchorPoint(cocos2d::Vec2(0.0, 1.0));
    spr_retry->setScale(Utility::content_scale());
//    btn_retry->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_get_retry, this));
    
    
    return spr_retry;
}


cocos2d::Sprite * MainMenuScreen::create_sprite_coin_cnt()
{
    cocos2d::Sprite * spr_coin = cocos2d::Sprite::create("res/ui/icon_coin.png");
    
    spr_coin->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
    spr_coin->setScale(Utility::content_scale());
//    btn_coin->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_get_coin, this));
    
    
    return spr_coin;
}


cocos2d::ui::Button * MainMenuScreen::create_btn_options()
{
    cocos2d::ui::Button * btn_options = cocos2d::ui::Button::create("res/ui/btn_menu.png");
    
    btn_options->setAnchorPoint(cocos2d::Vec2(1.0, 1.0));
    btn_options->setScale(Utility::content_scale());
    btn_options->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_options, this));

    
    return btn_options;
}


cocos2d::ui::Button * MainMenuScreen::create_btn_remove_ads()
{
    cocos2d::ui::Button * btn_remove_ads = cocos2d::ui::Button::create("res/ui/icon_block_ads.png");
    
    btn_remove_ads->setAnchorPoint(cocos2d::Vec2(1.0, 0.0));
    btn_remove_ads->setScale(Utility::content_scale());
    btn_remove_ads->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_remove_ads, this));

    
    return btn_remove_ads;
}


cocos2d::Label * MainMenuScreen::create_label_retry_cnt()
{
    cocos2d::Label * label_retry_cnt = cocos2d::Label::createWithTTF(std::to_string(PlayerProfile::retry_count()), FONT_KEN_FUTURE_THIN, 14);
    
    label_retry_cnt->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
    label_retry_cnt->setColor(cocos2d::Color3B(192, 177, 170));
    label_retry_cnt->enableOutline(cocos2d::Color4B::BLACK, 1);
//    label_retry_cnt->enableShadow();
    
    
    return label_retry_cnt;
}


cocos2d::Label * MainMenuScreen::create_label_coin_cnt()
{
    cocos2d::Label * label_coin_cnt = cocos2d::Label::createWithTTF(std::to_string(PlayerProfile::coin_count()), FONT_KEN_FUTURE_THIN, 14);
    
    label_coin_cnt->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
    label_coin_cnt->setColor(cocos2d::Color3B(192, 177, 170));
    label_coin_cnt->enableOutline(cocos2d::Color4B::BLACK, 1);
//    label_coin_cnt->enableShadow();
    
    
    return label_coin_cnt;
}


void MainMenuScreen::callback_options(Ref *pSender)
{
    Audio::instance()->play_sfx_button_click();
    
    GameSettings::set_scroll_percentage(scrollview_->getScrolledPercentHorizontal());
    
    cocos2d::Scene * scene = SettingsScreen::create_scene();
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
}



void MainMenuScreen::callback_select_level(Ref *pSender)
{
    cocos2d::ui::Button * button = (cocos2d::ui::Button *)pSender;
    
    Audio::instance()->play_sfx_button_click();
    
    GameSettings::set_scroll_percentage(scrollview_->getScrolledPercentHorizontal());
    
//    sdkbox::PluginSdkboxAds::hide("placement-banner");
    
    cocos2d::Scene * scene = GameplayScreen::create_scene(button->getTag());
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
}


void MainMenuScreen::callback_select_level_locked(Ref *pSender)
{
    
}


void MainMenuScreen::callback_get_retry(Ref *pSender)
{
    
}


void MainMenuScreen::callback_get_coin(Ref *pSender)
{
    
}


void MainMenuScreen::callback_remove_ads(cocos2d::Ref *pSender)
{
    PlayerProfile::set_ads_enabled(false);
    
    btn_remove_ads->removeFromParent();
    btn_remove_ads = NULL;
    
//    sdkbox::PluginSdkboxAds::hide("placement-banner");
}


// sdkbox inapp callbacks
//void MainMenuScreen::onInitialized(bool success)
//{
//    if (success)    printf("InApp init success");
//    else            printf("InApp init failed");
//}
//
//
//void MainMenuScreen::onSuccess(const sdkbox::Product& p)
//{
//
//}
//
//
//void MainMenuScreen::onFailure(const sdkbox::Product& p, const std::string& msg)
//{
//
//}
//
//
//void MainMenuScreen::onCanceled(const sdkbox::Product& p)
//{
//
//}
//
//
//void MainMenuScreen::onRestored(const sdkbox::Product& p)
//{
//
//}
//
//
//void MainMenuScreen::onProductRequestSuccess(const std::vector<sdkbox::Product>& products)
//{
//
//}
//
//
//void MainMenuScreen::onProductRequestFailure(const std::string& msg)
//{
//
//}
//
//
//void MainMenuScreen::onRestoreComplete(bool ok, const std::string &msg)
//{
//
//}


//sdkbox ads listener callbacks
//void MainMenuScreen::onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type)
//{
//    if (!PlayerProfile::ads_enabled())
//        return;
//
//
//    if (zone == "banner" && action_type == sdkbox::AdActionType::LOADED)
//    {
//        sdkbox::PluginSdkboxAds::placement("placement-banner");
//    }
//
////    if (!placement_interstitial_done && zone == "interstitial" && action_type == sdkbox::AdActionType::LOADED)
////    {
////        sdkbox::PluginSdkboxAds::placement("placement-interstitial");
////        placement_interstitial_done = true;
////    }
//}


//void MainMenuScreen::onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed)
//{
//
//}


// sdkbox admob listener callbacks
//void MainMenuScreen::adViewDidReceiveAd(const std::string &name)
//{
//
//}
//
//
//void MainMenuScreen::adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg)
//{
//
//}
//
//
//void MainMenuScreen::adViewWillPresentScreen(const std::string &name)
//{
//
//}
//
//
//void MainMenuScreen::adViewDidDismissScreen(const std::string &name)
//{
//
//}
//
//
//void MainMenuScreen::adViewWillDismissScreen(const std::string &name)
//{
//
//}
//
//
//void MainMenuScreen::adViewWillLeaveApplication(const std::string &name)
//{
//
//}
//
//
//void MainMenuScreen::reward(const std::string &name, const std::string &currency, double amount)
//{
//
//}
//
//
//// from 2.4.3.5+
//void MainMenuScreen::adViewDidFailToPresentWithError(const std::string &name, const std::string &msg)
//{
//
//}


// sdkbox review listener callbacks
void MainMenuScreen::onDisplayAlert()
{

}


void MainMenuScreen::onDeclineToRate()
{

}


void MainMenuScreen::onRate()
{

}


void MainMenuScreen::onRemindLater()
{

}


// sdkbox share listener callbacks
//void MainMenuScreen::onShareState(const sdkbox::SocialShareResponse& response)
//{
//    
//}
