#include "HelpScreen.h"
#include "Audio.h"
#include "Definitions.h"
#include "MainMenuScreen.h"
#include "Utility.h"


cocos2d::Scene * HelpScreen::create_scene()
{
    cocos2d::Scene * scene = cocos2d::Scene::create();
    HelpScreen * help_screen = HelpScreen::create();
    
    scene->addChild(help_screen);
    
    return scene;
}


bool HelpScreen::init()
{
    float window_right = Utility::window_right();
    float window_top = Utility::window_top();
    
    
    btn_home = cocos2d::ui::Button::create("res/ui/btn_home.png");
    btn_home->setScale(Utility::content_scale());
    btn_home->setPosition(cocos2d::Vec2(window_right-btn_home->getContentSize().width/2,
                                        window_top-btn_home->getContentSize().height/2));
    btn_home->addClickEventListener(CC_CALLBACK_1(HelpScreen::callback_home, this));
    addChild(btn_home);
    
    
    return true;
}


HelpScreen::~HelpScreen()
{
    
}


void HelpScreen::callback_home(cocos2d::Ref *pSender)
{
    Audio::instance()->play_sfx_button_click();
    
    cocos2d::Scene * scene = MainMenuScreen::create_scene();
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
}

