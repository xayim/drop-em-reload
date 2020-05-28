#include <dirent.h>
#include <fstream>
#include "Definitions.h"
#include "Utility.h"


cocos2d::Size Utility::window_size()
{
    return cocos2d::Director::getInstance()->getWinSize();
}


float Utility::get_density(float weight, cocos2d::Size area)
{
    return weight/(area.width*area.height);
}


float Utility::get_density(float weight, float radius)
{
    return weight/(radius*radius);
}


float Utility::content_scale()
{
    return 1-(1/cocos2d::Director::getInstance()->getContentScaleFactor());
}

cocos2d::Vec2 Utility::scale_vector()
{
    return cocos2d::Vec2(cocos2d::Director::getInstance()->getWinSize().width/2732.f, cocos2d::Director::getInstance()->getWinSize().height/2048.f);
}


float Utility::window_scale_x()
{
    return cocos2d::Director::getInstance()->getWinSize().width/2732.f;
}


float Utility::window_scale_y()
{
    return cocos2d::Director::getInstance()->getWinSize().height/2048.f;
}


cocos2d::Vec2 Utility::window_center()
{
    return cocos2d::Vec2(cocos2d::Director::getInstance()->getWinSize().width/2, cocos2d::Director::getInstance()->getWinSize().height/2);
}


float Utility::window_top()
{
    return cocos2d::Director::getInstance()->getSafeAreaRect().origin.y+cocos2d::Director::getInstance()->getSafeAreaRect().size.height;
}


float Utility::window_bottom()
{
    return cocos2d::Director::getInstance()->getSafeAreaRect().origin.y;
}


float Utility::window_left()
{
    return cocos2d::Director::getInstance()->getSafeAreaRect().origin.x;
}


float Utility::window_right()
{
    return cocos2d::Director::getInstance()->getSafeAreaRect().origin.x+cocos2d::Director::getInstance()->getSafeAreaRect().size.width;
}


float Utility::unsafe_width()
{
    return window_size().width-window_right();
}


//float Utility::ptm_ratio()
//{
//    return cocos2d::Director::getInstance()->getWinSize().height/PHYSICS_WORLD_HEIGHT;
//}


unsigned int Utility::file_count(const std::string& path)
{
    unsigned int file_count = 0;
    DIR * directory;
    dirent * ent;
    
    directory = opendir(cocos2d::FileUtils::getInstance()->fullPathForFilename(path).c_str());
    
    if (directory != NULL)
    {
        while ((ent = readdir(directory)) != NULL)
        {
            if (ent->d_name[0] != '.')
            {
                file_count++;
            }
        }
    }
    
    return file_count;
}


world_info * Utility::world_info(unsigned int id)
{
    std::ifstream input_file;
    struct world_info * world_info = nullptr;
    
    input_file.open(cocos2d::FileUtils::getInstance()->fullPathForFilename("level_data/level_config.json"));
    
    if (input_file.is_open())
    {
        // reading source file
        char read_char;
        std::string read_string;
        
        input_file >> read_char;
        
        while (!input_file.eof())
        {
            read_string.append(&read_char, 1);
            input_file >> read_char;
        }
        
        input_file.close();
        
        rapidjson::Document world_info_document;
        world_info_document.Parse<0>(read_string.c_str());

        
        world_info = new struct world_info();
        
        world_info->title = world_info_document["world_array"][id]["title"].GetString();
        world_info->res_path = world_info_document["world_array"][id]["res_path"].GetString();
        world_info->level_data_path = world_info_document["world_array"][id]["level_data_path"].GetString();
        world_info->res_name = world_info_document["world_array"][id]["res_name"].GetString();
    }
    
    return world_info;
}


unsigned int Utility::world_count()
{
    std::ifstream input_file;
    unsigned int world_count = 0;
    
    input_file.open(cocos2d::FileUtils::getInstance()->fullPathForFilename("level_data/level_config.json"));
    
    if (input_file.is_open())
    {
        // reading source file
        char read_char;
        std::string read_string;
        
        input_file >> read_char;
        
        while (!input_file.eof())
        {
            read_string.append(&read_char, 1);
            input_file >> read_char;
        }
        
        input_file.close();
        
        rapidjson::Document world_info_document;
        world_info_document.Parse<0>(read_string.c_str());
        
        world_count = world_info_document["world_array"].Size();
    }
    
    return world_count;
}


rapidjson::Document Utility::read_level_document(const std::string &filename)
{
    std::ifstream file;
    rapidjson::Document document;
    
    file.open(cocos2d::FileUtils::getInstance()->fullPathForFilename(filename));
    
    if (file.is_open())
    {
        char read_char;
        std::string read_string;
        file >> read_char;
        
        while (!file.eof())
        {
            read_string.append(&read_char, 1);
            file >> read_char;
        }
        
        file.close();
        
        document.Parse<0>(read_string.c_str());
    }
    
    return document;
}
