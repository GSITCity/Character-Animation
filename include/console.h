#pragma once
#include <Windows.h>

class Console {
public:
    /*
    * @brief 先回到默认的控制台大小再全屏化
    * @return 0 函数正常执行
    */
    int maxScreen();

    /*
    * @brief 隐藏控制台中的光标
    * @return 0 函数正常执行
    */
    int hideCursor();

    /*
    * @brief 移动控制台中光标的位置
    * @param x_position 光标的X坐标,默认值为0
    * @param y_position 光标的Y坐标,默认值为0
    * @return 0 函数正常执行
    */
    int moveCursor(short x_position = 0, short y_position = 0);

    /*
    * @brief 设置控制台字体大小
    * @param font_width 字体的宽度
    * @param font_height 字体的高度
    * @return 0 函数正常执行
    */
    int setConsoleFontSize(int font_width, int font_height);

    /*
    * @brief 获取屏幕的大小
    * @param screen_width 屏幕的宽度
    * @param screen_height 屏幕的高度
    * @return 0 函数正常执行
    */
    int getScreenSize(int& screen_width, int& screen_height);
};