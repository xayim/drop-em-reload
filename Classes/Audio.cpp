#include "Audio.h"
#include <audio/include/SimpleAudioEngine.h>
#include "GameSettings.h"


Audio * Audio::instance_ = NULL;

Audio * Audio::initialise()
{
    CCLOG("Audio::initialise()");
    
    if (!instance_) instance_ = new Audio();
    
    return instance_;
}

Audio::Audio()
{
    CocosDenshion::SimpleAudioEngine * audio_instance = CocosDenshion::SimpleAudioEngine::getInstance();
    
    music_path_level_clear = "music/level_clear.mp3";
    music_path_menu_background = "music/music_01.mp3";
    music_path_level_fail = "music/level_failed_1.mp3";
    effect_path_collide = "sfx/ball_collision_2.wav";
    effect_path_button_click = "sfx/button_click.wav";
    
    audio_instance->preloadBackgroundMusic(music_path_menu_background.c_str());
    audio_instance->preloadBackgroundMusic(music_path_level_clear.c_str());
    audio_instance->preloadBackgroundMusic(music_path_level_fail.c_str());
    audio_instance->preloadEffect(effect_path_collide.c_str());
    audio_instance->preloadEffect(effect_path_button_click.c_str());
    
    audio_instance->setBackgroundMusicVolume(GameSettings::music_flag());
}


void Audio::play_music_menu_background()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(music_path_menu_background.c_str(), true);
}

void Audio::play_music_level_clear()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(music_path_level_clear.c_str(), false);
}

void Audio::play_music_level_fail()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(music_path_level_fail.c_str(), false);
}

void Audio::play_sfx_collide()
{
    if (GameSettings::sfx_flag())
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effect_path_collide.c_str());
}

void Audio::play_sfx_button_click()
{
    if (GameSettings::sfx_flag())
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(effect_path_button_click.c_str());
}

void Audio::stop_all()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void Audio::pause_all()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void Audio::resume_all()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void Audio::stop_all_music()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void Audio::stop_all_sfx()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

bool Audio::is_music_playing()
{
    return CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}
