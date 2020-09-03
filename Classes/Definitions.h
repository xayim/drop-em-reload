#ifndef Definitions_h
#define Definitions_h

#define UNLOCK_WORLDS   1
#define UNLOCK_LEVELS   1
#define TRANSITION_TIME  0.5
#define BRIGHT_FACTOR   4
#define PHYSICS_WORLD_HEIGHT    32.0
#define PHYSICS_WORLD_WIDTH     ((4.0/3.0)*PHYSICS_WORLD_HEIGHT)
#define PTM_RATIO           (cocos2d::Director::getInstance()->getWinSize().height/PHYSICS_WORLD_HEIGHT)
#define GAMEPLAY_HINGE      0
#define GAMEPLAY_SELECT     1
#define UI_MARGIN_X     16
#define UI_MARGIN_Y     8
#define FONT_SKRANJI_REG        "fonts/Skranji-Regular.ttf"
#define FONT_SKRANJI_BOLD       "fonts/Skranji-Bold.ttf"
#define FONT_KEN_FUTURE_THIN    "fonts/kenvector_future_thin.ttf"
#define FONT_KEN_FUTURE         "fonts/kenvector_future.ttf"
#define GAME_FONT           FONT_KEN_FUTURE_THIN
#define FONT_SIZE_TITLE             23
#define FONT_SIZE_HEADING           15
#define FONT_SIZE_HEADING_SMALL     11
#define FONT_SIZE_TEXT              8
#define COIN_PACKAGE_1      160
#define COIN_PACKAGE_2      ((COIN_PACKAGE_1*2)*(1+0.2))
#define COIN_PACKAGE_5      ((COIN_PACKAGE_1*5)*(1+0.4))
#define RETRY_REWARD        3
#define COLOR_JAMMER_ACTIVE     (cocos2d::Color3B(187, 249, 107))
#define COLOR_JAMMER_INACTIVE   (cocos2d::Color3B(46.75, 62.25, 26.75))
#define COLOR_ROPE_ACTIVE     (cocos2d::Color3B(151, 93, 113))
#define COLOR_ROPE_INACTIVE   (cocos2d::Color3B(37.75, 23.25, 28.25))


#endif /* Definitions_h */
