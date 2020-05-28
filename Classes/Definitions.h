#ifndef Definitions_h
#define Definitions_h

#define UNLOCK_WORLDS   1
#define UNLOCK_LEVELS   1
#define TRANSITION_TIME  0.5
#define BRIGHT_FACTOR   4
#define PHYSICS_WORLD_HEIGHT    20
#define PTM_RATIO           (cocos2d::Director::getInstance()->getWinSize().height/PHYSICS_WORLD_HEIGHT)
#define GAMEPLAY_HINGE      0
#define GAMEPLAY_SELECT     1
#define UI_BUTTON_HEIGHT    (160*(Utility::window_scale_x()))
#define UI_BORDER_MARGIN    16
#define FONT_SKRANJI_REG        "fonts/Skranji-Regular.ttf"
#define FONT_SKRANJI_BOLD       "fonts/Skranji-Bold.ttf"
#define COIN_PACKAGE_1      160
#define COIN_PACKAGE_2      ((COIN_PACKAGE_1*2)*(1+0.2))
#define COIN_PACKAGE_5      ((COIN_PACKAGE_1*5)*(1+0.4))
#define RETRY_REWARD        10


#endif /* Definitions_h */
