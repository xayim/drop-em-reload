#include "SettingsScreen.h"
#include "Audio.h"
#include "Background.h"
#include "Definitions.h"
#include "GameSettings.h"
#include "HelpScreen.h"
#include "MainMenuScreen.h"
#include "Utility.h"
//#include "WorldSelectScreen.h"


cocos2d::Scene * SettingsScreen::create_scene()
{
    cocos2d::Scene * scene = cocos2d::Scene::create();
    SettingsScreen * settings_screen = SettingsScreen::create();
    
    scene->addChild(settings_screen);
    
    return scene;
}

bool SettingsScreen::init()
{
//    cocos2d::Size window_size = cocos2d::Director::getInstance()->getVisibleSize();
//    float scale = 1-(1/cocos2d::Director::getInstance()->getContentScaleFactor());
//    cocos2d::Vec2 window_center = Utility::window_center();
//    float window_bottom = Utility::window_bottom();
//    float window_left = Utility::window_left();
//    float scale_x = Utility::scale_vector().x;
//    bool music_flag = Audio::instance()->music_flag();
//    bool sfx_flag = Audio::instance()->sfx_flag();
    
    
    
    
//    Background * background = Background::create(5);
//    background->setPosition(Utility::window_center());
//    addChild(background);
    
    
    layout_buttons();
//    btn_music = cocos2d::ui::Button::create();
//
//    if (GameSettings::music_flag()) btn_music->loadTextureNormal("res/ui/btn_music.png");
//    else                            btn_music->loadTextureNormal("res/ui/btn_music_off.png");
//
//    btn_music->setPosition(cocos2d::Vec2(window_center.x-btn_music->getContentSize().width, window_center.y));
//    btn_music->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_music, this));
//    addChild(btn_music);
//
//
//    btn_sfx = cocos2d::ui::Button::create();
//
//    if (GameSettings::sfx_flag())   btn_sfx->loadTextureNormal("res/ui/btn_sound.png");
//    else                            btn_sfx->loadTextureNormal("res/ui/btn_sound_off.png");
//
//    btn_sfx->setPosition(cocos2d::Vec2(window_center.x+btn_sfx->getContentSize().width, window_center.y));
//    btn_sfx->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_sfx, this));
//    addChild(btn_sfx);
//
//
//    btn_gameplay = cocos2d::ui::Button::create();
//
//    if (GameSettings::gameplay_type())  btn_gameplay->loadTextureNormal("res/ui/btn_gameplay_2.png");
//    else                                btn_gameplay->loadTextureNormal("res/ui/btn_gameplay_1.png");
//
//    btn_gameplay->setPosition(cocos2d::Vec2(window_center.x, window_center.y));
//    btn_gameplay->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_gameplay, this));
//    addChild(btn_gameplay);
    
    
    btn_back_ = cocos2d::ui::Button::create("res/ui/btn_back.png");
    btn_back_->setScale(Utility::content_scale());
    btn_back_->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
    btn_back_->setPosition(cocos2d::Vec2(Utility::ui_left(), Utility::ui_bottom()));
    btn_back_->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_back, this));
    addChild(btn_back_);
    
    
    return true;
}


SettingsScreen::~SettingsScreen()
{
    
}


void SettingsScreen::callback_back(Ref *pSender)
{
    Audio::instance()->play_sfx_button_click();
    
    cocos2d::Scene * scene = MainMenuScreen::create_scene();
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
}


void SettingsScreen::callback_music(Ref *pSender)
{
    cocos2d::ui::Button * button = (cocos2d::ui::Button *) pSender;
    bool music_flag = GameSettings::music_flag();
    
    
    if (music_flag)   button->loadTextureNormal("res/ui/btn_music_off.png");
    else                button->loadTextureNormal("res/ui/btn_music.png");
    
    
    GameSettings::set_music_flag(!music_flag);
    
    Audio::instance()->play_sfx_button_click();
}


void SettingsScreen::callback_sfx(Ref *pSender)
{
    cocos2d::ui::Button * button = (cocos2d::ui::Button *) pSender;
    bool sfx_flag = GameSettings::sfx_flag();
    
    
    if (sfx_flag)   button->loadTextureNormal("res/ui/btn_sound_off.png");
    else            button->loadTextureNormal("res/ui/btn_sound.png");
    
    
    GameSettings::set_sfx_flag(!sfx_flag);
    
    Audio::instance()->play_sfx_button_click();
}


void SettingsScreen::callback_gameplay(cocos2d::Ref *pSender)
{
    float scale = Utility::content_scale();
    cocos2d::Vec2 window_centre = Utility::window_center();
    
    
    btn_music_->removeFromParent();
    btn_sfx_->removeFromParent();
    btn_gameplay_->removeFromParent();
    btn_store_->removeFromParent();
    btn_help_->removeFromParent();
    
    
    panel_ = Node::create();
    panel_->setPosition(window_centre);
    panel_->setScale(scale);
    addChild(panel_);
    
    
    cocos2d::Sprite * panel_bg = cocos2d::Sprite::create("res/ui/panel_small.png");
    panel_->addChild(panel_bg);
    
    
    cocos2d::Sprite * panel_ribbon = cocos2d::Sprite::create("res/ui/ribbon.png");
    panel_ribbon->setPosition(cocos2d::Vec2(panel_bg->getPosition().x, panel_bg->getPosition().y+panel_bg->getContentSize().height/2));
    panel_->addChild(panel_ribbon);
    
    
    cocos2d::Label * panel_label = cocos2d::Label::createWithTTF("Gameplay", FONT_SKRANJI_REG, 20);
    panel_label->setPosition(panel_ribbon->getPosition());
    panel_label->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    panel_label->setColor(cocos2d::Color3B::WHITE);
//    panel_label->enableShadow();
    panel_label->enableOutline(cocos2d::Color4B::BLACK, 1);
    panel_->addChild(panel_label);
    
    
    cocos2d::ui::Button * btn_gameplay_hinge = cocos2d::ui::Button::create("res/ui/btn_1.png");
    btn_gameplay_hinge->setPosition(cocos2d::Vec2(-btn_gameplay_hinge->getContentSize().width, panel_bg->getPosition().y));
    btn_gameplay_hinge->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_gameplay_hinge, this));
    panel_->addChild(btn_gameplay_hinge);
    
    
    cocos2d::ui::Button * btn_gameplay_touch = cocos2d::ui::Button::create("res/ui/btn_2.png");
    btn_gameplay_touch->setPosition(cocos2d::Vec2(btn_gameplay_hinge->getContentSize().width, panel_bg->getPosition().y));
    btn_gameplay_touch->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_gameplay_select, this));
    panel_->addChild(btn_gameplay_touch);
    
    
    Audio::instance()->play_sfx_button_click();
}


void SettingsScreen::callback_gameplay_hinge(cocos2d::Ref *pSender)
{
    panel_->removeFromParent();
    
    GameSettings::set_gameplay_type(GAMEPLAY_HINGE);
    
    layout_buttons();
    
    
    Audio::instance()->play_sfx_button_click();
}


void SettingsScreen::callback_gameplay_select(cocos2d::Ref *pSender)
{
    panel_->removeFromParent();
    
    GameSettings::set_gameplay_type(GAMEPLAY_SELECT);
    
    layout_buttons();
    
    
    Audio::instance()->play_sfx_button_click();
}


void SettingsScreen::callback_store(cocos2d::Ref *pSender)
{
    
    
    Audio::instance()->play_sfx_button_click();
}


void SettingsScreen::callback_help(cocos2d::Ref *pSender)
{
//    cocos2d::Scene * scene = HelpScreen::create_scene();
//    
//    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
    
    Audio::instance()->play_sfx_button_click();
}


void SettingsScreen::layout_buttons()
{
    cocos2d::Vec2 window_center = Utility::window_center();
    
    
    btn_music_ = cocos2d::ui::Button::create();
    
    if (GameSettings::music_flag()) btn_music_->loadTextureNormal("res/ui/btn_music.png");
    else                            btn_music_->loadTextureNormal("res/ui/btn_music_off.png");
    
    btn_music_->setPosition(cocos2d::Vec2(window_center.x-btn_music_->getContentSize().width*3, window_center.y));
    btn_music_->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_music, this));
    addChild(btn_music_);
    
    
    btn_sfx_ = cocos2d::ui::Button::create();
    
    if (GameSettings::sfx_flag())   btn_sfx_->loadTextureNormal("res/ui/btn_sound.png");
    else                            btn_sfx_->loadTextureNormal("res/ui/btn_sound_off.png");
    
    btn_sfx_->setPosition(cocos2d::Vec2(window_center.x-btn_sfx_->getContentSize().width*1.5, window_center.y));
    btn_sfx_->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_sfx, this));
    addChild(btn_sfx_);
    
    
    btn_gameplay_ = cocos2d::ui::Button::create();
    
    if (GameSettings::gameplay_type())  btn_gameplay_->loadTextureNormal("res/ui/btn_gameplay_2.png");
    else                                btn_gameplay_->loadTextureNormal("res/ui/btn_gameplay_1.png");
    
    btn_gameplay_->setPosition(cocos2d::Vec2(window_center.x, window_center.y));
    btn_gameplay_->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_gameplay, this));
    addChild(btn_gameplay_);
    
    
    btn_store_ = cocos2d::ui::Button::create();
    btn_store_->loadTextureNormal("res/ui/btn_store.png");
    btn_store_->setPosition(cocos2d::Vec2(window_center.x+btn_store_->getContentSize().width*1.5, window_center.y));
    btn_store_->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_store, this));
    addChild(btn_store_);
    
    
    btn_help_ = cocos2d::ui::Button::create();
    btn_help_->loadTextureNormal("res/ui/btn_help.png");
    btn_help_->setPosition(cocos2d::Vec2(window_center.x+btn_help_->getContentSize().width*3, window_center.y));
    btn_help_->addClickEventListener(CC_CALLBACK_1(SettingsScreen::callback_help, this));
    addChild(btn_help_);
}
