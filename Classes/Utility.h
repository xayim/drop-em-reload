#ifndef Utility_h
#define Utility_h

#include <stdio.h>
#include "cocos2d.h"
#include "external/json/document.h"
#include "RopeStructure.h"

struct world_info
{
    std::string title;
    std::string res_path;
    std::string level_data_path;
    std::string res_name;
};

class Utility
{
public:
    static cocos2d::Size window_size();
    static float content_scale();
    static cocos2d::Vec2 scale_vector();
    static float window_scale_x();
    static float window_scale_y();

    static float get_density(float weight, cocos2d::Size area);
    static float get_density(float weight, float radius);
//    static float ptm_ratio();
    static unsigned int file_count(const std::string& path);    // returns number of files and folders in path, returns -1 if directory not found
    static unsigned int world_count();
    static struct world_info * world_info(unsigned int world_id);
    static rapidjson::Document read_level_document(const std::string& filename);
    
    
    static cocos2d::Vec2 window_center();
    static float window_top();
    static float window_bottom();
    static float window_left();
    static float window_right();
    static float unsafe_margin();
    
    static float ui_top();
    static float ui_bottom();
    static float ui_left();
    static float ui_right();
};

#endif /* Utility_h */
