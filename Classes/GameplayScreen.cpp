#include <fstream>
#include "external/json/document.h"
#include "Audio.h"
#include "ContactListener.h"
#include "Definitions.h"
#include "GLES-Render/GLESRender.h"
#include "GameplayScreen.h"
#include "GameSettings.h"
#include "MainMenuScreen.h"
#include "PlayerProfile.h"
#include "UIComponentUtility.h"
#include "Utility.h"


cocos2d::Scene * scene_;
unsigned int level_id_;


cocos2d::Scene * GameplayScreen::create_scene(unsigned int level_id)
{
    level_id_ = level_id;
    

    scene_ = cocos2d::Scene::create();

    GameplayScreen * gameplay_screen = GameplayScreen::create();
    scene_->addChild(gameplay_screen);
    
    
    return scene_;
}


GameplayScreen::~GameplayScreen()
{
    // draw_node
    draw_node->removeFromParent();
    draw_node = NULL;
    
    
    // platforms
    for (unsigned int i = 0; i < platform_list_.size(); ++i)
        platform_list_.at(i)->~Platform();
    
    
    // blocks
    for (unsigned int i = 0; i < enemy_list_.size(); ++i)
        enemy_list_.at(i)->~Enemy();
    
    
    // rope structures
    for (unsigned int i = 0; i < rope_structure_list_.size(); ++i)
        rope_structure_list_.at(i)->~RopeStructure();
    
    
    // physics
    world_->~b2World();
    world_ = NULL;
}


bool GameplayScreen::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    PlayerProfile::set_retry_count(RETRY_REWARD);
    
    // set sdkbox listener
    sdkbox::PluginSdkboxAds::setListener(this);
    
    
    // read level file
    level_document = Utility::read_level_document("level_data/level_"+std::to_string(level_id_)+".json");
    
    
    // init background
    init_background();
    
    
    // init physics
    init_physics();
    
    
    // init draw_node
    draw_node = cocos2d::DrawNode::create(6);
    addChild(draw_node);
    
    
    // init_platforms
    init_platforms();
    
    
    // init blocks
    init_blocks();
    
    
    // init rope structures
    init_rope_structures();
    
    
    // init ui
    init_ui();
    
    
    // init cameras
    init_cameras();
    
    
    // init touch listener
    init_touch_listener();
    
    
    // set flags to default values
    flag_gamepause = false;
    flag_gamewin = false;
    flag_gamelose = false;
    flag_gameover = false;
    
    
    // schedule update
    scheduleUpdate();
    
    
    return true;
}


void GameplayScreen::init_background()
{
    background_ = Background::create(0);
    background_->setPosition(Utility::window_center());
    
    scene_->addChild(background_);
}


void GameplayScreen::init_ui()
{
    ui_ = Node::create();
    
    
    cocos2d::ui::Button * btn_pause = create_btn_pause();
    btn_pause->setPosition(cocos2d::Vec2(Utility::ui_right(), Utility::ui_top()));
    ui_->addChild(btn_pause);
    
    
    cocos2d::Label * label_retries = create_label_retries();
    label_retries->setPosition(cocos2d::Vec2(Utility::ui_left(), Utility::ui_top()));
    ui_->addChild(label_retries);
    
    
    scene_->addChild(ui_);
}


void GameplayScreen::init_physics()
{
    // create debug draw for PTM_RATIO
    GLESDebugDraw * debug_draw = new GLESDebugDraw(PTM_RATIO);
    
    // setup flags for debut draw
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
//    flags += b2Draw::e_jointBit;
//    flags += b2Draw::e_aabbBit;
//    flags += b2Draw::e_pairBit;
//    flags += b2Draw::e_centerOfMassBit;
    debug_draw->SetFlags(flags);
    
    
    // create world with gravity
    world_ = new b2World(b2Vec2(0, -10));
    world_->SetAutoClearForces(true);
    world_->SetDebugDraw(debug_draw);
    
    // create contact listener
    contact_listener_ = new ContactListener();
    
    // set contact listener for b2World
    world_->SetContactListener(contact_listener_);
    
    
    init_physics_bounds();
}


void GameplayScreen::init_physics_bounds()
{
    // create world bounds
    b2BodyDef bound_def;
    b2FixtureDef bound_bottom_fixture_def, bound_left_fixture_def, bound_right_fixture_def;
    b2EdgeShape bound_bottom_shape, bound_left_shape, bound_right_shape;
    
    bound_def.position.Set(0, 0);
    bound_def.type = b2BodyType::b2_staticBody;
    
    
    world_bound_ = (b2Body *)world_->CreateBody(&bound_def);
    
    
    bound_bottom_shape.Set(b2Vec2(-(PHYSICS_WORLD_WIDTH/2), -(PHYSICS_WORLD_HEIGHT/2)), b2Vec2((PHYSICS_WORLD_WIDTH/2), -(PHYSICS_WORLD_HEIGHT/2)));
    bound_bottom_fixture_def.shape = &bound_bottom_shape;
    bound_left_shape.Set(b2Vec2(-(PHYSICS_WORLD_WIDTH/2), -(PHYSICS_WORLD_HEIGHT/2)), b2Vec2(-(PHYSICS_WORLD_WIDTH/2), (PHYSICS_WORLD_HEIGHT/2)));
    bound_left_fixture_def.shape = &bound_left_shape;
    bound_right_shape.Set(b2Vec2((PHYSICS_WORLD_WIDTH/2), -(PHYSICS_WORLD_HEIGHT/2)), b2Vec2((PHYSICS_WORLD_WIDTH/2), (PHYSICS_WORLD_HEIGHT/2)));
    bound_right_fixture_def.shape = &bound_right_shape;
    
    
    world_bound_->CreateFixture(&bound_bottom_fixture_def);
    world_bound_->CreateFixture(&bound_left_fixture_def);
    world_bound_->CreateFixture(&bound_right_fixture_def);
}


void GameplayScreen::init_platforms()
{
    for (unsigned int i = 0; i < level_document["platform"].Size(); ++i)
    {
        PolygonBodyData * platform_object;
        Platform * platform;
        
        
        platform_object = new PolygonBodyData();
        
        platform_object->id_global = level_document["platform"][i]["global_index"].GetInt();
        platform_object->body_type = level_document["platform"][i]["type"].GetInt();
        platform_object->angle = level_document["platform"][i]["angle"].GetFloat();
        platform_object->position.set(level_document["platform"][i]["position"]["x"].GetFloat(),
                                      level_document["platform"][i]["position"]["y"].GetFloat());
        platform_object->density = level_document["platform"][i]["fixture"][0]["density"].GetFloat();
        platform_object->friction = level_document["platform"][i]["fixture"][0]["friction"].GetFloat();
        platform_object->vertex_count = level_document["platform"][i]["fixture"][0]["chain"]["vertices"]["x"].Size();
        platform_object->vertices = new cocos2d::Vec2[platform_object->vertex_count];
        
        for (unsigned int j = 0; j < platform_object->vertex_count; ++j)
        {
            platform_object->vertices[j].set(level_document["platform"][i]["fixture"][0]["chain"]["vertices"]["x"][j].GetFloat(),
                                             level_document["platform"][i]["fixture"][0]["chain"]["vertices"]["y"][j].GetFloat());
        }
        
        
        platform = new Platform(world_, platform_object);
        platform_list_.push_back(platform);
    }
}


void GameplayScreen::init_blocks()
{
    for (unsigned int i = 0; i < level_document["enemy"].Size(); ++i)
    {
        PolygonBodyData * enemy_object;
        Enemy * enemy;
        
        
        enemy_object =  new PolygonBodyData();
        
        enemy_object->id_global = level_document["enemy"][i]["global_index"].GetInt();
        enemy_object->body_type = level_document["enemy"][i]["type"].GetInt();
        enemy_object->angle = level_document["enemy"][i]["angle"].GetFloat();
        enemy_object->position.set(level_document["enemy"][i]["position"]["x"].GetFloat(),
                                   level_document["enemy"][i]["position"]["y"].GetFloat());
        enemy_object->density = level_document["enemy"][i]["fixture"][0]["density"].GetFloat();
        enemy_object->friction = level_document["enemy"][i]["fixture"][0]["friction"].GetFloat();
        enemy_object->vertex_count = level_document["enemy"][i]["fixture"][0]["polygon"]["vertices"]["x"].Size();
        enemy_object->vertices = new cocos2d::Vec2[enemy_object->vertex_count];
        
        for (unsigned int j = 0; j < enemy_object->vertex_count; ++j)
        {
            enemy_object->vertices[j].set(level_document["enemy"][i]["fixture"][0]["polygon"]["vertices"]["x"][j].GetFloat(),
                                          level_document["enemy"][i]["fixture"][0]["polygon"]["vertices"]["y"][j].GetFloat());
        }
        
        
        enemy = new Enemy(world_, enemy_object, this);
        enemy_list_.push_back(enemy);
    }
}


void GameplayScreen::init_rope_structures()
{
    for (unsigned int i = 0; i < level_document["rope_structure"].Size(); ++i)
    {
        RopeStructureData * rope_structure_object = new RopeStructureData();
        
        rope_structure_object->id_local = i;
        rope_structure_object->jammer_object = new RoundBodyData();
        rope_structure_object->jammer_object->id_global = level_document["rope_structure"][i]["jammer"]["global_index"].GetInt();
        rope_structure_object->jammer_object->body_type = level_document["rope_structure"][i]["jammer"]["type"].GetInt();
        rope_structure_object->jammer_object->angle = level_document["rope_structure"][i]["jammer"]["angle"].GetFloat();
        rope_structure_object->jammer_object->radius = level_document["rope_structure"][i]["jammer"]["fixture"][0]["circle"]["radius"].GetFloat();
        rope_structure_object->jammer_object->position.set(level_document["rope_structure"][i]["jammer"]["position"]["x"].GetFloat(),
                                                           level_document["rope_structure"][i]["jammer"]["position"]["y"].GetFloat());
        
        
        
        for (unsigned int j = 0; j < level_document["rope_structure"][i]["hinge"].Size(); ++j)
        {
            RoundBodyData * hinge_object = new RoundBodyData();
            
            hinge_object->id_global = level_document["rope_structure"][i]["hinge"][j]["global_index"].GetInt();
            hinge_object->body_type = level_document["rope_structure"][i]["hinge"][j]["type"].GetInt();
            hinge_object->angle = level_document["rope_structure"][i]["hinge"][j]["angle"].GetFloat();
            hinge_object->radius = level_document["rope_structure"][i]["hinge"][j]["fixture"][0]["circle"]["radius"].GetFloat();
            hinge_object->position.set(level_document["rope_structure"][i]["hinge"][j]["position"]["x"].GetFloat(),
                                       level_document["rope_structure"][i]["hinge"][j]["position"]["y"].GetFloat());
            
            rope_structure_object->hinge_object_list.push_back(hinge_object);
        }
        
        
        
        for (unsigned int j = 0; j < level_document["rope_structure"][i]["rope"].Size(); ++j)
        {
            RopeData * rope_object = new RopeData();
            
            rope_object->id_local = j;
            
            for (unsigned int k = 0; k < level_document["rope_structure"][i]["rope"][j].Size(); ++k)
            {
                PolygonBodyData * rope_body_object = new PolygonBodyData();
                
                rope_body_object->id_global = level_document["rope_structure"][i]["rope"][j][k]["global_index"].GetInt();
                rope_body_object->body_type = level_document["rope_structure"][i]["rope"][j][k]["type"].GetInt();
                rope_body_object->angle = level_document["rope_structure"][i]["rope"][j][k]["angle"].GetFloat();
                rope_body_object->position.set(level_document["rope_structure"][i]["rope"][j][k]["position"]["x"].GetFloat(),
                                               level_document["rope_structure"][i]["rope"][j][k]["position"]["y"].GetFloat());
                rope_body_object->density = level_document["rope_structure"][i]["rope"][j][k]["fixture"][0]["density"].GetFloat();
                rope_body_object->friction = level_document["rope_structure"][i]["rope"][j][k]["fixture"][0]["friction"].GetFloat();
                rope_body_object->vertex_count = level_document["rope_structure"][i]["rope"][j][k]["fixture"][0]["polygon"]["vertices"]["x"].Size();
                rope_body_object->vertices = new cocos2d::Vec2[rope_body_object->vertex_count];
                
                
                for (unsigned int l = 0; l < rope_body_object->vertex_count; ++l)
                {
                    rope_body_object->vertices[l].set(level_document["rope_structure"][i]["rope"][j][k]["fixture"][0]["polygon"]["vertices"]["x"][l].GetFloat(),
                                                      level_document["rope_structure"][i]["rope"][j][k]["fixture"][0]["polygon"]["vertices"]["y"][l].GetFloat());
                }
                
                
                
                rope_object->rope_body_object_list.push_back(rope_body_object);
            }
            
            rope_structure_object->rope_object_list.push_back(rope_object);
        }
        
        
        RopeStructure * rope_structure = new RopeStructure(world_, rope_structure_object, this);
        
        if (GameSettings::gameplay_type() == GAMEPLAY_HINGE)
        {
            for (unsigned int j = 0; j < rope_structure->rope_count(); ++j)
            {
                cocos2d::ui::Button * btn_hinge = rope_structure->create_button_hinge(j);
                btn_hinge->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_hinge, this));
                
                addChild(btn_hinge);
            }
            
            
            rope_structure->set_active(true);
        }
        else if (GameSettings::gameplay_type() == GAMEPLAY_SELECT)
        {
            cocos2d::ui::Button * btn_select = rope_structure->create_button_select();
            btn_select->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_select, this));
            
            addChild(btn_select);
            
            
            rope_structure->set_active(false);
        }
        
        
        rope_structure_list_.push_back(rope_structure);
    }
}


void GameplayScreen::init_touch_listener()
{
    cocos2d::EventListenerTouchOneByOne * listener = cocos2d::EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(GameplayScreen::on_touch_began, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameplayScreen::on_touch_moved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameplayScreen::on_touch_ended, this);
    listener->setSwallowTouches(true);
    
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}


void GameplayScreen::init_cameras()
{
    // create camera for background
    camera_background_ = GameCamera::create(scene_, background_, background_->getPosition(), background_->getContentSize(), cocos2d::CameraFlag::USER2);
    
    // create camera for playground
    camera_playground_ = GameCamera::create(scene_, this, cocos2d::Vec2::ZERO, Utility::window_size(), cocos2d::CameraFlag::USER1);
    
    // create camera for ui
    camera_ui_ = GameCamera::create(scene_, ui_, Utility::window_center(), Utility::window_size(), cocos2d::CameraFlag::DEFAULT);
}


cocos2d::ui::Button * GameplayScreen::create_btn_pause()
{
    cocos2d::ui::Button * btn_pause = UIComponentUtility::create_ui_button("res/ui/btn_pause.png",
                                                                           cocos2d::Vec2(1.0, 1.0),
                                                                           cocos2d::Vec2::ZERO,
                                                                           Utility::content_scale());
    btn_pause->setTag(TAG_BTN_PAUSE);
    btn_pause->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_pause, this));
    
    
    return btn_pause;
}


cocos2d::ui::Button * GameplayScreen::create_btn_resume()
{
    cocos2d::ui::Button * btn_resume = UIComponentUtility::create_ui_button("res/ui/btn_play.png",
                                                                            cocos2d::Vec2(1.0, 1.0),
                                                                            cocos2d::Vec2::ZERO,
                                                                            Utility::content_scale());
    
    btn_resume->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_resume, this));
    
    
    return btn_resume;
}


cocos2d::ui::Button * GameplayScreen::create_btn_home()
{
    cocos2d::ui::Button * btn_home = UIComponentUtility::create_ui_button("res/ui/btn_home.png",
                                                                          cocos2d::Vec2(1.0, 1.0),
                                                                          cocos2d::Vec2::ZERO,
                                                                          Utility::content_scale());
    
    btn_home->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_home, this));
    
    
    return btn_home;
}


cocos2d::ui::Button * GameplayScreen::create_btn_restart()
{
    cocos2d::ui::Button * btn_restart = UIComponentUtility::create_ui_button("res/ui/btn_reload.png",
                                                                             cocos2d::Vec2(1.0, 1.0),
                                                                             cocos2d::Vec2::ZERO,
                                                                             Utility::content_scale());
    
    btn_restart->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_restart, this));
    
    
    return btn_restart;
}


cocos2d::ui::Button * GameplayScreen::create_btn_forward()
{
    cocos2d::ui::Button * btn_advance = UIComponentUtility::create_ui_button("res/ui/btn_forward_play.png",
                                                                             cocos2d::Vec2(1.0, 1.0),
                                                                             cocos2d::Vec2::ZERO,
                                                                             Utility::content_scale());
    
    btn_advance->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_forward, this));
    
    return btn_advance;
}


cocos2d::Label * GameplayScreen::create_label_retries()
{
    cocos2d::Label * label_retries = UIComponentUtility::create_ui_label("Re: " + std::to_string(PlayerProfile::retry_count()),
                                                                       GAME_FONT,
                                                                       FONT_SIZE_HEADING_SMALL,
                                                                       cocos2d::Vec2(0.0, 1.0),
                                                                       cocos2d::Vec2::ZERO,
                                                                       cocos2d::Color3B::WHITE
                                                                       /*cocos2d::Color3B(187, 252, 254)*/,
                                                                       true);
    label_retries->setTag(TAG_LABEL_RETRY_CNT);
    
    return label_retries;
}


bool GameplayScreen::on_touch_began(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    cocos2d::log("touch_began");
 
    touch_location = touch->getLocation();
    
    return true;
}


void GameplayScreen::on_touch_moved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    cocos2d::log("touch_moved");
    
    touch_location_previous = touch_location;
    
    touch_location = touch->getLocation();
    
    scroll_by(touch_location_previous-touch_location);
    
    touch_moved = true;
}


void GameplayScreen::on_touch_ended(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    cocos2d::log("touch_ended");
    
    RopeStructure * rs_active = rope_structure_active();
    
    if (!touch_moved && rs_active)
    {
        touch_location = touch->getLocation();
        
        if (touch_location.x > Utility::window_center().x)
            remove_rope(rs_active, rs_active->rope_count()-1);
        else
            remove_rope(rs_active, 0);
        
        if (rs_active->rope_count() == 0)
        {
            rs_active->remove_select_button();
            rs_active->set_active(false);
        }
    }
    
    touch_moved = false;
}


void GameplayScreen::scroll_by(cocos2d::Vec2 delta)
{
    camera_playground_->move_by(delta);
    camera_background_->move_by(delta*0.75f);
}


void GameplayScreen::create_layout_pause()
{
//    // dim the background
//    background_->dim();
    
    
    // create menu node
    menu_node_ = Node::create();
    ui_->addChild(menu_node_);
    
    
    // create menu label
    cocos2d::Label * label_menu = UIComponentUtility::create_ui_label("Paused",
                                                                      GAME_FONT,
                                                                      FONT_SIZE_TITLE,
                                                                      cocos2d::Vec2(0.5, 1.0),
                                                                      cocos2d::Vec2(Utility::window_center().x, Utility::ui_top()),
                                                                      cocos2d::Color3B::WHITE,
                                                                      true);
    menu_node_->addChild(label_menu);
    
    
    // create resume button
    cocos2d::ui::Button * btn_resume = create_btn_resume();
    btn_resume->setPosition(cocos2d::Vec2(Utility::ui_right(), Utility::ui_top()));
    menu_node_->addChild(btn_resume);
    
    
    // create restart button
    cocos2d::ui::Button * btn_restart = create_btn_restart();
    btn_restart->setPosition(cocos2d::Vec2(Utility::ui_right(),
                                           btn_resume->getPosition().y-UI_MARGIN_Y-btn_restart->getContentSize().height*btn_restart->getScale()));
    menu_node_->addChild(btn_restart);
    
    
    // create home button
    cocos2d::ui::Button * btn_home = create_btn_home();
    btn_home->setPosition(cocos2d::Vec2(Utility::ui_right(),
                                        btn_restart->getPosition().y-UI_MARGIN_Y-btn_home->getContentSize().height*btn_restart->getScale()));
    menu_node_->addChild(btn_home);
}


void GameplayScreen::create_layout_win()
{
//    // dim background
//    background_->dim();
    
    
    // create menu node
    menu_node_ = Node::create();
    ui_->addChild(menu_node_);
    
    
    // create menu label
    cocos2d::Label * label_menu = UIComponentUtility::create_ui_label("You Win",
                                                                      GAME_FONT,
                                                                      FONT_SIZE_TITLE,
                                                                      cocos2d::Vec2(0.5, 1.0),
                                                                      cocos2d::Vec2(Utility::window_center().x, Utility::ui_top()),
                                                                      cocos2d::Color3B::WHITE,
                                                                      true);
    menu_node_->addChild(label_menu);
    
    
    // create forward button
    cocos2d::ui::Button * btn_forward = create_btn_forward();
    btn_forward->setPosition(cocos2d::Vec2(Utility::ui_right(), Utility::ui_top()));
    menu_node_->addChild(btn_forward);
    
    
    // create restart button
    cocos2d::ui::Button * btn_restart = create_btn_restart();
    btn_restart->setPosition(cocos2d::Vec2(Utility::ui_right(),
                                           btn_forward->getPosition().y-UI_MARGIN_Y-btn_restart->getContentSize().height*btn_restart->getScale()));
    menu_node_->addChild(btn_restart);
    
    
    // create home button
    cocos2d::ui::Button * btn_home = create_btn_home();
    btn_home->setPosition(cocos2d::Vec2(Utility::ui_right(),
                                        btn_restart->getPosition().y-UI_MARGIN_Y-btn_home->getContentSize().height*btn_home->getScale()));
    menu_node_->addChild(btn_home);
    
    
    
    
    // layout level complete info
//    cocos2d::Label * boxes_text_label = UIComponentUtility::create_ui_label("Boxes Droped",
//                                                                            GAME_FONT,
//                                                                            FONT_SIZE_HEADING,
//                                                                            cocos2d::Vec2(1.0, 0.5),
//                                                                            cocos2d::Vec2(Utility::window_center().x-UI_MARGIN_X, Utility::ui_top()-UI_MARGIN_Y*7),
//                                                                            cocos2d::Color3B::WHITE,
//                                                                            true);
//    ui_->addChild(boxes_text_label);
//
//
//    cocos2d::Label * boxes_score_label = UIComponentUtility::create_ui_label(std::to_string(level_document["enemy"].Size())+ " x20",
//                                                                             GAME_FONT,
//                                                                             FONT_SIZE_HEADING,
//                                                                             cocos2d::Vec2(0.0, 0.5),
//                                                                             cocos2d::Vec2(Utility::window_center().x+UI_MARGIN_X, Utility::ui_top()-UI_MARGIN_Y*7),
//                                                                             cocos2d::Color3B::WHITE,
//                                                                             true);
//    ui_->addChild(boxes_score_label);
//
//
//    cocos2d::Label * retry_text_label = UIComponentUtility::create_ui_label("Retry Left",
//                                                                            GAME_FONT,
//                                                                            FONT_SIZE_HEADING,
//                                                                            cocos2d::Vec2(1.0, 0.5),
//                                                                            cocos2d::Vec2(Utility::window_center().x-UI_MARGIN_X, boxes_text_label->getPosition().y-UI_MARGIN_Y*3),
//                                                                            cocos2d::Color3B::WHITE,
//                                                                            true);
//    ui_->addChild(retry_text_label);
//
//
//    cocos2d::Label * retry_score_label = UIComponentUtility::create_ui_label(std::to_string(PlayerProfile::retry_count())+" x10",
//                                                                             GAME_FONT,
//                                                                             FONT_SIZE_HEADING,
//                                                                             cocos2d::Vec2(0.0, 0.5),
//                                                                             cocos2d::Vec2(Utility::window_center().x+UI_MARGIN_X, boxes_score_label->getPosition().y-UI_MARGIN_Y*3),
//                                                                             cocos2d::Color3B::WHITE,
//                                                                             true);
//    ui_->addChild(retry_score_label);
//
//
//    cocos2d::Label * bonus_text_label = UIComponentUtility::create_ui_label("Level Bonus",
//                                                                            GAME_FONT,
//                                                                            FONT_SIZE_HEADING,
//                                                                            cocos2d::Vec2(1.0, 0.5),
//                                                                            cocos2d::Vec2(Utility::window_center().x-UI_MARGIN_X, retry_text_label->getPosition().y-UI_MARGIN_Y*3),
//                                                                            cocos2d::Color3B::WHITE,
//                                                                            true);
//    ui_->addChild(bonus_text_label);
//
//
//    cocos2d::Label * bonus_score_label = UIComponentUtility::create_ui_label(std::to_string(250),
//                                                                             GAME_FONT,
//                                                                             FONT_SIZE_HEADING,
//                                                                             cocos2d::Vec2(0.0, 0.5),
//                                                                             cocos2d::Vec2(Utility::window_center().x+UI_MARGIN_X, retry_score_label->getPosition().y-UI_MARGIN_Y*3),
//                                                                             cocos2d::Color3B::WHITE,
//                                                                             true);
//    ui_->addChild(bonus_score_label);
//
//
//    unsigned int total_score = level_document["enemy"].Size()*20+PlayerProfile::retry_count()*10+250;
//
//    cocos2d::Label * total_text_label = UIComponentUtility::create_ui_label("Total",
//                                                                            GAME_FONT,
//                                                                            FONT_SIZE_HEADING,
//                                                                            cocos2d::Vec2(1.0, 0.5),
//                                                                            cocos2d::Vec2(Utility::window_center().x-UI_MARGIN_X, bonus_text_label->getPosition().y-UI_MARGIN_Y*4),
//                                                                            cocos2d::Color3B::WHITE,
//                                                                            true);
//    ui_->addChild(total_text_label);
//
//
//    cocos2d::Label * total_score_label = UIComponentUtility::create_ui_label(std::to_string(total_score),
//                                                                             GAME_FONT,
//                                                                             FONT_SIZE_HEADING,
//                                                                             cocos2d::Vec2(0.0, 0.5),
//                                                                             cocos2d::Vec2(Utility::window_center().x+UI_MARGIN_X, bonus_score_label->getPosition().y-UI_MARGIN_Y*4),
//                                                                             cocos2d::Color3B::WHITE,
//                                                                             true);
//    ui_->addChild(total_score_label);
}


void GameplayScreen::create_layout_lose()
{
//    // dim background
//    background_->dim();
    
    
    // create menu node
    menu_node_ = Node::create();
    ui_->addChild(menu_node_);
    
    
    // create menu label
    cocos2d::Label * label_menu = UIComponentUtility::create_ui_label("You Lose",
                                                                      GAME_FONT,
                                                                      FONT_SIZE_TITLE,
                                                                      cocos2d::Vec2(0.5, 1.0),
                                                                      cocos2d::Vec2(Utility::window_center().x, Utility::ui_top()),
                                                                      cocos2d::Color3B::WHITE,
                                                                      true);
    menu_node_->addChild(label_menu);
    
    
    // create restart button
    cocos2d::ui::Button * btn_restart = create_btn_restart();
    btn_restart->setPosition(cocos2d::Vec2(Utility::ui_right(), Utility::ui_top()));
    menu_node_->addChild(btn_restart);
    
    
    // create home button
    cocos2d::ui::Button * btn_home = create_btn_home();
    btn_home->setPosition(cocos2d::Vec2(Utility::ui_right(),
                                         btn_restart->getPosition().y-UI_MARGIN_Y-btn_home->getContentSize().height*btn_home->getScale()));
    ui_->addChild(btn_home);
}


void GameplayScreen::callback_pause(cocos2d::Ref *pSender)
{
    cocos2d::ui::Button * btn_pause = (cocos2d::ui::Button *)pSender;
        
    btn_pause->removeFromParent();
    
    
    background_->dim();
    
    cocos2d::Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
    
    
    if (flag_gamewin)          create_layout_win();
    else if (flag_gamelose)    create_layout_lose();
    else
    {
        Audio::instance()->play_sfx_button_click();
        
        create_layout_pause();
        
        flag_gamepause = true;
    }
    
    
    for (unsigned int i = 0; i < rope_structure_list_.size(); ++i)
    {
        if (rope_structure_list_.at(i)->has_btn_select())    rope_structure_list_.at(i)->enable_select_button(false);
        
        if (rope_structure_list_.at(i)->has_btn_reset())    rope_structure_list_.at(i)->enable_reset_button(false);
        
        if (rope_structure_list_.at(i)->has_btn_hinbe())    rope_structure_list_.at(i)->enable_hinge_button(false);
    }
}


void GameplayScreen::callback_reset_jammer(cocos2d::Ref *pSender)
{
    cocos2d::ui::Button * btn_retry = (cocos2d::ui::Button *)pSender;
    RopeStructure * rope_structure = (RopeStructure *)btn_retry->getUserData();
    
    if (PlayerProfile::retry_count() > 0)
    {
        rope_structure->reset();
        
        rope_structure->remove_reset_button();
        
        
        PlayerProfile::set_retry_count(PlayerProfile::retry_count()-1);
        
        set_label_retry_cnt("Re: "+std::to_string(PlayerProfile::retry_count()));
        
        
        if (PlayerProfile::retry_count() == 0)
        {
            for (unsigned int i = 0; i < rope_structure_list_.size(); ++i)
            {
                if (rope_structure_list_.at(i)->has_btn_reset())    rope_structure_list_.at(i)->remove_reset_button();
            }
        }
        
        
        
        if (GameSettings::gameplay_type() == GAMEPLAY_HINGE)
        {
            for (unsigned int i = 0; i < rope_structure->rope_count(); ++i)
            {
                cocos2d::ui::Button * btn_hinge = rope_structure->create_button_hinge(i);
                btn_hinge->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_hinge, this));
                
                addChild(btn_hinge);
            }
        }
        else if (GameSettings::gameplay_type() == GAMEPLAY_SELECT)
        {
            cocos2d::ui::Button * btn_select = rope_structure->create_button_select();
            btn_select->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_select, this));
            
            addChild(btn_select);
            
            
            RopeStructure * rs_active = rope_structure_active();
            
            if (rs_active)  rs_active->set_active(false);
        }
        
        
        rope_structure->set_active(true);
    }
    
    
    Audio::instance()->play_sfx_button_click();
}


void GameplayScreen::callback_resume(cocos2d::Ref *pSender)
{
    menu_node_->removeFromParent();
    
    // bright the background
    background_->bright();

    cocos2d::Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
    
    
    cocos2d::ui::Button * btn_pause = create_btn_pause();
    btn_pause->setPosition(cocos2d::Vec2(Utility::ui_right(), Utility::ui_top()));
    ui_->addChild(btn_pause);
    
    
    for (unsigned int i = 0; i < rope_structure_list_.size(); ++i)
    {
        if (rope_structure_list_.at(i)->has_btn_select())   rope_structure_list_.at(i)->enable_select_button(true);
        
        if (rope_structure_list_.at(i)->has_btn_reset())    rope_structure_list_.at(i)->enable_reset_button(true);
        
        if (rope_structure_list_.at(i)->has_btn_hinbe())    rope_structure_list_.at(i)->enable_hinge_button(true);
    }
    
    
    Audio::instance()->play_sfx_button_click();
    
    flag_gamepause = false;
}


void GameplayScreen::callback_restart(cocos2d::Ref *pSender)
{
    unsigned int level_id = level_document["level_id"].GetInt();
    
    Audio::instance()->play_sfx_button_click();
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, GameplayScreen::create_scene(level_id)));
}


void GameplayScreen::callback_home(cocos2d::Ref *pSender)
{
    Audio::instance()->play_sfx_button_click();
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, MainMenuScreen::create_scene()));
}


void GameplayScreen::callback_forward(cocos2d::Ref *pSender)
{
    unsigned int level_id = level_document["level_id"].GetInt()+1;
    
    if (level_id == Utility::file_count("level_data"))   level_id = 0;
    
    
    Audio::instance()->play_sfx_button_click();
    
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(TRANSITION_TIME, GameplayScreen::create_scene(level_id)));
}


void GameplayScreen::callback_hinge(Ref *pSender)
{
    cocos2d::ui::Button * button = (cocos2d::ui::Button *)pSender;
    RopeStructure * rope_structure = (RopeStructure *)button->getUserData();

    
    remove_rope(rope_structure, (unsigned int)button->getTag());

    rope_structure->remove_hinge_button(button);
}


void GameplayScreen::callback_select(Ref *pSender)
{
    cocos2d::ui::Button * button = (cocos2d::ui::Button *)pSender;
    RopeStructure * rope_structure = (RopeStructure * )button->getUserData();
    RopeStructure * rs_active = rope_structure_active();
    
    if (rs_active)  rs_active->set_active(false);
    
    rope_structure->set_active(true);
}


//sdkbox ads listener callbacks
void GameplayScreen::onAdAction( const std::string& ad_unit_id, const std::string& zone, sdkbox::AdActionType action_type)
{
    if (ad_unit_id == "UnityAds" && zone == "reward" && action_type == sdkbox::AdActionType::AD_ENDED)
    {
        PlayerProfile::set_retry_count(RETRY_REWARD);
        
        set_label_retry_cnt("Re: " + std::to_string(PlayerProfile::retry_count()));
    }
}


void GameplayScreen::onRewardAction( const std::string& ad_unit_id, const std::string& zone_id, float reward_amount, bool reward_succeed)
{
    if (reward_succeed)
    {
        PlayerProfile::set_retry_count(RETRY_REWARD);

        set_label_retry_cnt("Re: " + std::to_string(PlayerProfile::retry_count()));
    }
}


void GameplayScreen::remove_rope(RopeStructure *rope_structure, unsigned int rope_index)
{
    rope_structure->remove_rope(rope_index);
}


unsigned int GameplayScreen::jammers_count()
{
    unsigned int count = 0;
    
    for (unsigned i = 0; i < rope_structure_list_.size(); ++i)
    {
        if (rope_structure_list_.at(i)->jammer)     ++count;
    }
    
    
    return count;
}


unsigned int GameplayScreen::enemies_count()
{
    return (unsigned int)enemy_list_.size();
}


void GameplayScreen::set_label_retry_cnt(const std::string &label)
{
    cocos2d::Label * label_retry_cnt = (cocos2d::Label *) ui_->getChildByTag(TAG_LABEL_RETRY_CNT);
    
    label_retry_cnt->setString(label);
}


RopeStructure * GameplayScreen::rope_structure_active()
{
    RopeStructure * active = NULL;
    
    for (unsigned int i = 0; i < rope_structure_list_.size(); ++i)
    {
        if (rope_structure_list_.at(i)->is_active())
        {
            active = rope_structure_list_.at(i);
            
            break;
        }
    }
    
    
    return active;
}


void GameplayScreen::update_game_state()
{
    if (enemies_count() == 0)                                               flag_gamewin = true;
    else if (jammers_count() == 0 && PlayerProfile::retry_count() == 0)     flag_gamelose = true;
}


void GameplayScreen::check_contacts()
{
    std::vector<MyContact>::iterator contact_itr;
    MyContact contact;
    b2Body *body_a, *body_b;
    
    RopeStructure *rope_structure;
    std::vector<RopeStructure *> rope_structure_del_list;
    std::vector<RopeStructure *>::iterator rope_structure_itr;
    
    Enemy *enemy;
    std::vector<Enemy *> enemy_del_list;
    std::vector<Enemy *>::iterator enemy_del_itr;
    std::vector<Enemy *>::iterator enemy_itr;
    
    
    for (contact_itr = contact_listener_->contacts_.begin(); contact_itr != contact_listener_->contacts_.end(); ++contact_itr)
    {
        contact = *contact_itr;
        
        body_a = contact.fixtureA->GetBody();
        body_b = contact.fixtureB->GetBody();
        
        
        for (rope_structure_itr = rope_structure_list_.begin(); rope_structure_itr != rope_structure_list_.end(); ++rope_structure_itr)
        {
            rope_structure = *rope_structure_itr;
            
            if (rope_structure->jammer)
            {
                if ((rope_structure->jammer->body == body_a && world_bound_ == body_b) ||
                    (world_bound_ == body_a && rope_structure->jammer->body == body_b))
                {
                    rope_structure_del_list.push_back(rope_structure);
                }
            }
            
        }
        
        
        for (enemy_itr = enemy_list_.begin(); enemy_itr != enemy_list_.end(); ++enemy_itr)
        {
            enemy = *enemy_itr;

            if ((enemy->body == body_a && world_bound_ == body_b) ||
                (world_bound_ == body_a && enemy->body == body_b))
            {
                enemy_del_list.push_back(enemy);
            }
        }
    }
    
    
    for (rope_structure_itr = rope_structure_del_list.begin(); rope_structure_itr != rope_structure_del_list.end(); ++rope_structure_itr)
    {
        rope_structure = *rope_structure_itr;
        
        rope_structure->destroy_jammer();
        
        
        // create a button to reset that rope structure and add it to layer
        if (!is_game_over() && PlayerProfile::retry_count() > 0)
        {
            cocos2d::ui::Button * btn_reset = rope_structure->create_button_reset();
            btn_reset->addClickEventListener(CC_CALLBACK_1(GameplayScreen::callback_reset_jammer, this));

            addChild(btn_reset);
        }
    }
    
    for (enemy_itr = enemy_del_list.begin(); enemy_itr != enemy_del_list.end(); ++enemy_itr)
    {
        enemy = *enemy_itr;
        
        enemy_del_itr = std::find(enemy_list_.begin(), enemy_list_.end(), enemy);
        
        enemy->~Enemy();
        enemy_list_.erase(enemy_del_itr);
    }
}


void GameplayScreen::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    cocos2d::Director::getInstance()->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
//    world_->DrawDebugData();
    
    
    draw_node->clear();
    
    for (unsigned int i = 0; i < platform_list_.size(); ++i)
    {
        platform_list_.at(i)->draw(draw_node);
    }
    
    
    cocos2d::Director::getInstance()->popMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}


void GameplayScreen::update(float delta_time)
{
    if (!flag_gamepause)
    {
        // update world
        world_->Step(delta_time, 60, 60);

        
        // update enemies
        for (unsigned int i = 0; i < enemy_list_.size(); ++i)
        {
            enemy_list_.at(i)->update();
        }
        
        // update rope structures
        for (unsigned int i = 0; i < rope_structure_list_.size(); ++i)
        {
            rope_structure_list_.at(i)->update();
        }
        
        
        // check for contacts
        check_contacts();
        
        
        if (!is_game_over())
        {
            // update game state
            update_game_state();
        }
        else if (!flag_gameover)
        {
            log("gameover");
            callback_pause((Ref *)ui_->getChildByTag(TAG_BTN_PAUSE));
            
            flag_gameover = true;
        }
    }
}
