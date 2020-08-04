#include "TitleScreen.h"
#include "Audio.h"
#include "Definitions.h"
#include "GameSettings.h"
#include "MainMenuScreen.h"
#include "UIComponentUtility.h"
#include "Utility.h"


cocos2d::Scene * TitleScreen::create_scene()
{
    cocos2d::Scene * scene = cocos2d::Scene::create();
    
    TitleScreen * splash = TitleScreen::create();
    
    scene->addChild(splash);
    
    return scene;
}


bool TitleScreen::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    printf("Window Size: %f, %f\n", Utility::window_size().width, Utility::window_size().height);
    printf("Content Scale: %f\n", Utility::content_scale());
    
    
    Audio::initialise();
    
    timer = 0;
    touch_began = false;
    
    
    cocos2d::Sprite * background = cocos2d::Sprite::create("res/title_screen_background.png");
    background->setPosition(Utility::window_center());
    background->setScale(Utility::window_size().height/background->getContentSize().height);
    addChild(background);
    
    
    
    text = UIComponentUtility::create_ui_label("TAP TO PLAY",
                                               FONT_KEN_FUTURE_THIN,
                                               24,
                                               cocos2d::Vec2(0.5, 0.0),
                                               cocos2d::Vec2(Utility::window_center().x, Utility::ui_bottom()),
                                               cocos2d::Color3B::WHITE);
    
    text->enableOutline(cocos2d::Color4B::BLACK, 1);
    addChild(text);
    
    
    
    cocos2d::EventListenerTouchOneByOne * touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = CC_CALLBACK_2(TitleScreen::on_touch_began, this);
    touch_listener->setSwallowTouches(true);
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
 
    
    
    Audio::instance()->play_music_menu_background();
    
    
    GameSettings::set_scroll_percentage(0.f);
    
    
    scheduleUpdate();
    
    
    return true;
}


void TitleScreen::goto_main_menu(float delta_time)
{
    cocos2d::Scene * scene = MainMenuScreen::create_scene();
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, scene));
}


bool TitleScreen::on_touch_began(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    if (!touch_began)
    {
        touch_began = true;
        
        unscheduleUpdate();
        
        text->setVisible(true);
        text->runAction(cocos2d::Blink::create(TRANSITION_TIME*.7, 2));
        
        scheduleOnce(schedule_selector(TitleScreen::goto_main_menu), TRANSITION_TIME);
    }
    
    
    return true;
}


void TitleScreen::update(float delta_time)
{
    timer += delta_time;
    
    if (timer > 1)
    {
        timer = 0;
        
        text->setVisible(!text->isVisible());
    }
}
