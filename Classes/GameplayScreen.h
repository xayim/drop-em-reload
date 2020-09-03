#ifndef GameplayScreen_h
#define GameplayScreen_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "external/json/document.h"
#include "PluginSdkBoxAds/PluginSdkBoxAds.h"
#include "Background.h"
#include "ContactListener.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Platform.h"
#include "RopeStructure.h"

#define TAG_BTN_PAUSE           0
#define TAG_LABEL_RETRY_CNT     1



class GameplayScreen : public cocos2d::Layer, public sdkbox::PluginSdkboxAdsListener
{
public:
    static cocos2d::Scene * create_scene(unsigned int level_id);
        
private:
    ~GameplayScreen();
    
    CREATE_FUNC(GameplayScreen);
    
    virtual bool init();
    virtual void update(float delta_time);
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
    cocos2d::ui::Button * create_btn_resume();
    cocos2d::ui::Button * create_btn_home();
    cocos2d::ui::Button * create_btn_restart();
    cocos2d::ui::Button * create_btn_forward();
    cocos2d::Label * create_label_retries();

    void create_layout_pause();
    void create_layout_win();
    void create_layout_lose();

    void scroll_by(cocos2d::Vec2 delta);
    void remove_rope(RopeStructure * rope_structure, unsigned int rope_index);
    void check_contacts();
    unsigned int jammers_count();
    unsigned int enemies_count();
    void set_label_retry_cnt(const std::string &label);
    RopeStructure * rope_structure_active();
    bool is_game_over() { return flag_gamewin || flag_gamelose; };
    void update_game_state();
    
    
    // CALLBACKS
    void callback_pause(Ref * pSender);
    void callback_reset_jammer(Ref * pSender);
    void callback_resume(Ref * pSender);
    void callback_restart(Ref * pSender);
    void callback_home(Ref * pSender);
    void callback_forward(Ref * pSender);
    void callback_hinge(Ref *pSender);
    void callback_select(Ref *pSender);
    
    bool on_touch_began(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void on_touch_moved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void on_touch_ended(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    // sdkbox ads listener callbacks
    virtual void onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type);
    virtual void onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed);
    
    
    // GAME
    rapidjson::Document level_document;
    
    GameCamera * camera_background_;
    GameCamera * camera_playground_;
    GameCamera * camera_ui_;
    
    Background * background_;
    Node * ui_;
    
    bool flag_gamepause;
    bool flag_gamewin;
    bool flag_gamelose;
    bool flag_gameover;
    
    
    // PHYSICS
    ContactListener * contact_listener_;
    b2World * world_;
    b2Body * world_bound_;
    std::vector<Enemy *> enemy_list_;
    std::vector<Platform *> platform_list_;
    std::vector<RopeStructure *> rope_structure_list_;
    
    
    // GRAPHICS
    Node * menu_node_;
    cocos2d::DrawNode * draw_node;
    
    
    // VARIABLE
    bool touch_moved;
    cocos2d::Vec2 touch_location;
    cocos2d::Vec2 touch_location_previous;
};
#endif /* GameplayScreen_h */
