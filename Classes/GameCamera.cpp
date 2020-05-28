#include "GameCamera.h"
#include "Utility.h"


GameCamera * GameCamera::create(cocos2d::Scene * parent_scene, cocos2d::Node * target_node, const cocos2d::Vec2 position, const cocos2d::Size size, const cocos2d::CameraFlag camera_flag)
{
    GameCamera * game_camera = new GameCamera(parent_scene, target_node, position, size, camera_flag);
    
    return game_camera;
}


GameCamera::GameCamera(cocos2d::Scene * parent_scene, cocos2d::Node * target_node, const cocos2d::Vec2 position, const cocos2d::Size size, const cocos2d::CameraFlag camera_flag)
{
    camera = cocos2d::Camera::create();
    camera->setCameraFlag(camera_flag);
    
    camera->setPosition3D(cocos2d::Vec3(position.x, position.y, cocos2d::Director::getInstance()->getZEye()));
    camera->lookAt(cocos2d::Vec3(position.x, position.y, 0), cocos2d::Vec3::UNIT_Y);
    
    bounds.setRect(position.x-size.width/2, position.y-size.height/2, size.width, size.height);
    
    target_node->setCameraMask((unsigned short)camera_flag);
    
    parent_scene->addChild(camera);
}

GameCamera::~GameCamera()
{
    camera->removeFromParent();
}



void GameCamera::move_by(cocos2d::Vec2 delta)
{
    cocos2d::Size window_size = Utility::window_size();
    float z_eye = cocos2d::Director::getInstance()->getZEye();
    float view_max_x = camera->getPosition().x+window_size.width/2;
    float view_min_x = camera->getPosition().x-window_size.width/2;
    float view_max_y = camera->getPosition().y+window_size.height/2;
    float view_min_y = camera->getPosition().y-window_size.height/2;
    
    
    if (delta.x > 0)
    {
        if (view_max_x+delta.x <= bounds.getMaxX())
        {
            camera->setPosition3D(cocos2d::Vec3(camera->getPosition().x+delta.x, camera->getPosition().y, z_eye));
        }
        else
        {
            camera->setPosition3D(cocos2d::Vec3(bounds.getMaxX()-window_size.width/2, camera->getPosition().y, z_eye));
        }
        
    }
    else if (delta.x < 0)
    {
        if (view_min_x+delta.x >= bounds.getMinX())
        {
            camera->setPosition3D(cocos2d::Vec3(camera->getPosition().x+delta.x, camera->getPosition().y, z_eye));
        }
        else
        {
            camera->setPosition3D(cocos2d::Vec3(bounds.getMinX()+window_size.width/2, camera->getPosition().y, z_eye));
        }
    }
    
    if (delta.y > 0)
    {
        if (view_max_y+delta.y <= bounds.getMaxY())
        {
            camera->setPosition3D(cocos2d::Vec3(camera->getPosition().x, camera->getPosition().y+delta.y, z_eye));
        }
        else
        {
            camera->setPosition3D(cocos2d::Vec3(camera->getPosition().x, bounds.getMaxY()-window_size.height/2, z_eye));
        }
    }
    else if (delta.y < 0)
    {
        if (view_min_y+delta.y >= bounds.getMinY())
        {
            camera->setPosition3D(cocos2d::Vec3(camera->getPosition().x, camera->getPosition().y+delta.y, z_eye));
        }
        else
        {
            camera->setPosition3D(cocos2d::Vec3(camera->getPosition().x, bounds.getMinY()+window_size.height/2, z_eye));
        }
    }
}
