#ifndef TitleScreen_h
#define TitleScreen_h

#include "cocos2d.h"


class TitleScreen : public cocos2d::Layer
{
public:
    static cocos2d::Scene * create_scene(void);
    virtual bool init();
private:
    CREATE_FUNC(TitleScreen);
    
    bool on_touch_began(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void update(float delta_time);
    
    void goto_main_menu(float delta);
    
    
    cocos2d::Label * text;
    float timer;
    bool touch_began;
};

#endif /* TitleScreen_h */
