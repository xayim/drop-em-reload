#ifndef GameplayScreen_h
#define GameplayScreen_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "external/json/document.h"
//#include "PluginSdkBoxAds/PluginSdkBoxAds.h"
#include "Background.h"
#include "ContactListener.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Platform.h"
#include "RopeStructure.h"


class GameplayScreen : public cocos2d::Layer/*, public sdkbox::PluginSdkboxAdsListener*/
{
public:
    static cocos2d::Scene * create_scene(unsigned int level_id);
        
private:
    ~GameplayScreen();
    
    CREATE_FUNC(GameplayScreen);
    
    virtual bool init();
    virtual void update(float deltaTime);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
    
    
    // ACTIONS
    void init_touch_listener();
    void init_background();
    void init_physics();
    void init_physics_bounds();
    void init_platforms();
    void init_blocks();
    void init_rope_structures();
    void init_ui();
    void init_cameras();
    cocos2d::ui::Button * create_btn_pause();
    cocos2d::Sprite * create_sprite_retry_cnt();
    cocos2d::Label * create_label_score();
    cocos2d::Label * create_label_retry_cnt();
    void open_menu_pause();
    void open_menu_game_win();
    void open_menu_game_lose();
    void open_menu_store();
    void scroll_by(cocos2d::Vec2 delta);
    void remove_rope(RopeStructure * rope_structure, unsigned int rope_index);
    void check_contacts();
    
    
    // CALLBACKS
    void callback_pause(Ref * pSender);
    void callback_retry(Ref * pSender);
    void callback_get_retry(Ref * pSender);
    void callback_resume(Ref * pSender);
    void callback_restart(Ref * pSender);
    void callback_main_menu(Ref * pSender);
    void callback_advance(Ref * pSender);
    void callback_hinge(Ref *pSender);
    void callback_select(Ref *pSender);
    bool on_touch_began(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void on_touch_moved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void on_touch_ended(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    // sdkbox ads listener callbacks
//    virtual void onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type);
//    virtual void onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed);
    
    
    // GAME
    GameCamera * camera_background_;
    GameCamera * camera_playground_;
    GameCamera * camera_ui_;
    Background * background_;
    Node * ui_;
    rapidjson::Document level_document;
    unsigned int score_;
//    unsigned int retry_count_;
//    unsigned int coin_count_;
    
    
    // PHYSICS
    b2World * world_;
    ContactListener * contact_listener_;
    b2Body * world_bound_;
    std::vector<Enemy *> enemy_list_;
    std::vector<Platform *> platform_list_;
    std::vector<RopeStructure *> rope_structure_list_;
    
    
    // GRAPHICS
    Node * panel;
    cocos2d::Label * label_score_;
    cocos2d::Label * label_retry_cnt_;
    cocos2d::ui::Button * btn_pause_;
    cocos2d::Sprite * btn_retry_;
    cocos2d::DrawNode * draw_node;
    
    
    // VARIABLE
    bool touch_moved;
    cocos2d::Vec2 touch_location;
    cocos2d::Vec2 touch_location_previous;
};
#endif /* GameplayScreen_h */
