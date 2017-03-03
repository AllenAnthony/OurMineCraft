#ifndef OURCRAFT_PARAM_H
#define OURCRAFT_PARAM_H
#define TICKS_PER_SEC 60
#define SECTOR_SIZE 16
#define WALKING_SPEED 5
#define FLYING_SPEED 15
#define GRAVITY 20.0
#define MAX_JUMP_HEIGHT 1.0
#define JUMP_SPEED math.sqrt(2 * GRAVITY * MAX_JUMP_HEIGHT)
#define TERMINAL_VELOCITY 50
#define PLAYER_HEIGHT 2

/**
 * 窗口参数
 */
#define WIDTH 800
#define HEIGHT 600

/**
 * 上下左右四个键
 */
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

/**
 * 方块切换按键  对应数字键0~4
 */
#define BLOCK0 96
#define BLOCK1 97
#define BLOCK2 98
#define BLOCK3 99
#define BLOCK4 100

#endif //OURCRAFT_PARAM_H
