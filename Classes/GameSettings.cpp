#include "GameSettings.h"
#include <audio/include/SimpleAudioEngine.h>


bool GameSettings::music_flag()
{
    return cocos2d::UserDefault::getInstance()->getBoolForKey("music_flag", true);
}


void GameSettings::set_music_flag(bool flag)
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("music_flag", flag);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(flag);
}


void GameSettings::set_sfx_flag(bool flag)
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("sfx_flag", flag);
}


bool GameSettings::sfx_flag()
{
    return cocos2d::UserDefault::getInstance()->getBoolForKey("sfx_flag", true);
}


bool GameSettings::gameplay_type()
{
    return cocos2d::UserDefault::getInstance()->getBoolForKey("gameplay_type", false);
}


void GameSettings::set_gameplay_type(bool type)
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("gameplay_type", type);
}


float GameSettings::scroll_percentage()
{
    return cocos2d::UserDefault::getInstance()->getFloatForKey("scroll_percentage", 0);
}


void GameSettings::set_scroll_percentage(float percent)
{
    cocos2d::UserDefault::getInstance()->setFloatForKey("scroll_percentage", percent);
}
