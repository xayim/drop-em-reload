#include "PlayerProfile.h"


void PlayerProfile::set_coin_count(unsigned int count)
{
    cocos2d::UserDefault::getInstance()->setIntegerForKey("coin_count", count);
}


void PlayerProfile::set_retry_count(unsigned int count)
{
    cocos2d::UserDefault::getInstance()->setIntegerForKey("retry_count", count);
}


unsigned int PlayerProfile::coin_count()
{
    return cocos2d::UserDefault::getInstance()->getIntegerForKey("coin_count", 50);
}


unsigned int PlayerProfile::retry_count()
{
    return cocos2d::UserDefault::getInstance()->getIntegerForKey("retry_count", 5);
}


void PlayerProfile::set_ads_enabled(bool enable)
{
    cocos2d::UserDefault::getInstance()->setBoolForKey("ads_enabled", enable);
}


bool PlayerProfile::ads_enabled()
{
    return cocos2d::UserDefault::getInstance()->getBoolForKey("ads_enabled", true);
}
