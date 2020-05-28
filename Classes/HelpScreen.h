#ifndef HelpScreen_h
#define HelpScreen_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"


class HelpScreen : public cocos2d::Layer
{
public:
    static cocos2d::Scene * create_scene();
    virtual bool init();
private:
    CREATE_FUNC(HelpScreen);
    ~HelpScreen();
    
    void callback_home(cocos2d::Ref *pSender);
    
    cocos2d::ui::Button * btn_home = nullptr;
};

#endif /* HelpScreen_h */
