#ifndef GameCamera_h
#define GameCamera_h

#include <stdio.h>
#include "cocos2d.h"


class GameCamera
{
public:
    static GameCamera * create(cocos2d::Scene * parent_scene, cocos2d::Node * target_node, const cocos2d::Vec2 position, const cocos2d::Size size, const cocos2d::CameraFlag camera_flag);
    void move_by(cocos2d::Vec2 delta);
    
    ~GameCamera();
private:
    GameCamera(cocos2d::Scene * parent_scene, cocos2d::Node * target_node, const cocos2d::Vec2 position, const cocos2d::Size size, const cocos2d::CameraFlag camera_flag);

    cocos2d::Camera * camera;
    cocos2d::Rect bounds;
};

#endif /* GameCamera_h */
