#ifndef GameSettings_h
#define GameSettings_h

#include <stdio.h>


class GameSettings
{
public:
    static bool music_flag();
    static void set_music_flag(bool flag);
    static void set_sfx_flag(bool flag);
    static bool sfx_flag();
    static bool gameplay_type();
    static void set_gameplay_type(bool type);
    static float scroll_percentage();
    static void set_scroll_percentage(float percent);
    
private:
    
};

#endif /* GameSettings_h */
