#ifndef Audio_h
#define Audio_h

#include <stdio.h>


class Audio
{
public:
    static Audio * initialise();
    static Audio * instance() { return instance_; };
    
    void play_music_level_clear();
    void play_music_menu_background();
    void play_music_level_fail();
    void play_sfx_collide();
    void play_sfx_button_click();
    
    void stop_all();
    void pause_all();
    void resume_all();
    void stop_all_music();
    void stop_all_sfx();
    bool is_music_playing();
    
private:
    Audio();
    
    static Audio * instance_;
    std::string music_path_menu_background;
    std::string music_path_level_clear;
    std::string music_path_level_fail;
    std::string effect_path_collide;
    std::string effect_path_button_click;
};

#endif /* Audio_h */
