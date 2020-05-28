#ifndef PlayerProfile_h
#define PlayerProfile_h

#include <stdio.h>


class PlayerProfile
{
public:
    static void set_retry_count(unsigned int count);
    static void set_coin_count(unsigned int count);
    static unsigned int retry_count();
    static unsigned int coin_count();
    static void set_ads_enabled(bool enable);
    static bool ads_enabled();
    
private:
    
};

#endif /* PlayerProfile_h */
