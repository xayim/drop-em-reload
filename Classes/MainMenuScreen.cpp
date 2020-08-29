#include "MainMenuScreen.h"
#include "Audio.h"
#include "Background.h"
#include "Definitions.h"
#include "GameplayScreen.h"
#include "GameSettings.h"
#include "PlayerProfile.h"
#include "SettingsScreen.h"
#include "TitleScreen.h"
#include "UIComponentUtility.h"
#include "Utility.h"


cocos2d::Scene * MainMenuScreen::create_scene()
{
    cocos2d::Scene * scene = cocos2d::Scene::create();
    
    scene->addChild(MainMenuScreen::create());

    return scene;
}

bool MainMenuScreen::init()
{
    // init layer
    if (!Layer::init())
    {
        return false;
    }

//    sdkbox::IAP::setListener(this);
//    sdkbox::IAP::refresh();
    
    // set sdkbox listeners
    sdkbox::PluginSdkboxAds::setListener(this);
    sdkbox::PluginReview::setListener(this);
    
    // prompt for review dialogue
    sdkbox::PluginReview::show();

    
    // init background
    init_background();
    
    // init ui
    init_ui();
    
    
    return true;
}


MainMenuScreen::~MainMenuScreen()
{
//    sdkbox::PluginSdkboxAds::hide("placement-banner");
//    sdkbox::PluginSdkboxAds::setListener(NULL);
//    sdkbox::PluginShare::removeListener();
//    sdkbox::IAP::removeListener();
    sdkbox::PluginReview::removeListener();
}


void MainMenuScreen::init_background()
{
    background_ = Background::create(0);
    background_->setPosition(Utility::window_center());
    addChild(background_);
}


void MainMenuScreen::init_ui()
{
    // create menu label
    label_menu_ = UIComponentUtility::create_ui_label("Main Menu",
                                                      GAME_FONT,
                                                      FONT_SIZE_TITLE,
                                                      cocos2d::Vec2(0.5, 1.0),
                                                      cocos2d::Vec2(Utility::window_center().x, Utility::ui_top()),
                                                      cocos2d::Color3B::WHITE,
                                                      true);
    addChild(label_menu_);
    
    
    // create scrollview of levels
    scrollview_ = create_scrollview(Utility::file_count("level_data"));
    addChild(scrollview_);
    
    
    // create button options
    btn_options_ = UIComponentUtility::create_ui_button("res/ui/btn_menu.png",
                                                        cocos2d::Vec2(1.0, 1.0),
                                                        cocos2d::Vec2(Utility::ui_right(), Utility::ui_top()),
                                                        Utility::content_scale());
    
    btn_options_->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_options, this));
    addChild(btn_options_);
    
    
    
    // create button to remove ads if ads are enabled
    if (PlayerProfile::ads_enabled())
    {
        btn_remove_ads_ = UIComponentUtility::create_ui_button("res/ui/icon_block_ads.png",
                                                               cocos2d::Vec2(1.0, 0.0),
                                                               cocos2d::Vec2(Utility::ui_right(), Utility::ui_bottom()),
                                                               Utility::content_scale());
        
        btn_remove_ads_->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_remove_ads, this));
        addChild(btn_remove_ads_);
    }
}


cocos2d::ui::ScrollView * MainMenuScreen::create_scrollview(unsigned int items_count)
{
    std::stringstream string_stream;
    
    cocos2d::ui::ScrollView * scrollview = cocos2d::ui::ScrollView::create();
    scrollview->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollview->setContentSize(Utility::window_size());
    scrollview->setInnerContainerSize(cocos2d::Size(320*(items_count-1)+(Utility::window_size().width),
                                                    Utility::window_size().height));
    scrollview->setTouchEnabled(true);
    scrollview->setBounceEnabled(true);
    scrollview->setScrollBarEnabled(false);
    scrollview->scrollToPercentHorizontal(GameSettings::scroll_percentage(), 0, false);
    
    
    for (unsigned int i = 0; i < items_count; ++i)
    {
        cocos2d::Label * level_title = UIComponentUtility::create_ui_label(std::to_string(i+1),
                                                                           GAME_FONT,
                                                                           FONT_SIZE_TITLE,
                                                                           cocos2d::Vec2(0.5, 0.5),
                                                                           cocos2d::Vec2::ZERO,
                                                                           cocos2d::Color3B::WHITE,
                                                                           true);
        
        string_stream.str("");
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
        
        
        cocos2d::ui::Button * level_button = UIComponentUtility::create_ui_button(string_stream.str().c_str(),
                                                                                  cocos2d::Vec2(0.5, 0.5),
                                                                                  cocos2d::Vec2::ZERO,
                                                                                  1);
        level_button->setTitleLabel(level_title);
        level_button->setTag(i);
        level_button->setZoomScale(0.f);
        
        
        if (!is_locked) level_button->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_select_level, this));
        else            level_button->addClickEventListener(CC_CALLBACK_1(MainMenuScreen::callback_select_level_locked, this));
        
        
        
        cocos2d::ui::Layout * level_button_layout = cocos2d::ui::Layout::create();
        level_button_layout->setPosition(cocos2d::Vec2(Utility::window_center().x+(i*320),
                                                       Utility::window_center().y));
        level_button_layout->setContentSize(level_button->getContentSize());
        level_button_layout->addChild(level_button);
        
        
        scrollview->addChild(level_button_layout);
    }
    
    
    return scrollview;
}


void MainMenuScreen::callback_options(Ref *pSender)
{
    Audio::instance()->play_sfx_button_click();
    
    GameSettings::set_scroll_percentage(scrollview_->getScrolledPercentHorizontal());
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, SettingsScreen::create_scene()));
}



void MainMenuScreen::callback_select_level(Ref *pSender)
{
    cocos2d::ui::Button * button = (cocos2d::ui::Button *)pSender;
    
    Audio::instance()->play_sfx_button_click();
    
    sdkbox::PluginSdkboxAds::hide("placement-banner");
    
    GameSettings::set_scroll_percentage(scrollview_->getScrolledPercentHorizontal());
    

    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, GameplayScreen::create_scene(button->getTag())));
}


void MainMenuScreen::callback_select_level_locked(Ref *pSender)
{
    
}


void MainMenuScreen::callback_remove_ads(cocos2d::Ref *pSender)
{
    PlayerProfile::set_ads_enabled(false);
    
    btn_remove_ads_->removeFromParent();
    btn_remove_ads_ = NULL;
    
    sdkbox::PluginSdkboxAds::hide("placement-banner");
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
void MainMenuScreen::onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type)
{
    if (!PlayerProfile::ads_enabled())
        return;


    if (zone == "banner" && action_type == sdkbox::AdActionType::LOADED)
    {
        sdkbox::PluginSdkboxAds::placement("placement-banner");
    }

    if (!placement_interstitial_done && zone == "interstitial" && action_type == sdkbox::AdActionType::LOADED)
    {
        sdkbox::PluginSdkboxAds::placement("placement-interstitial");
        placement_interstitial_done = true;
    }
}


void MainMenuScreen::onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed)
{

}


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
