#include "console.h"

int Console::maxScreen() {
    /*获取前台窗口句柄*/
    HWND foreground_window_handle = GetForegroundWindow();
    /*先恢复到正常大小,防止字符动画在初始全屏的情况下播放出错*/
    ShowWindow(foreground_window_handle, SW_NORMAL);
    /*最大化控制台*/
    ShowWindow(foreground_window_handle, SW_MAXIMIZE);

    return 0;
}

int Console::hideCursor() {
    HANDLE std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO console_cursor_info;

    GetConsoleCursorInfo(std_handle, &console_cursor_info);
    console_cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(std_handle, &console_cursor_info);

    return 0;
}

int Console::moveCursor(short x_position, short y_position) {
    HANDLE std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursor_position = { x_position, y_position };
    SetConsoleCursorPosition(std_handle, cursor_position);

    return 0;
}

int Console::setConsoleFontSize(int font_width, int font_height) {
    HANDLE std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX console_font_info = { 0 };
    console_font_info.cbSize = sizeof(console_font_info);

    /*获取当前字体信息*/
    GetCurrentConsoleFontEx(std_handle, FALSE, &console_font_info);
    /*修改字体尺寸*/
    console_font_info.dwFontSize.X = font_width;
    console_font_info.dwFontSize.Y = font_height;
    /*设置字体家族*/
    console_font_info.FontFamily = FF_MODERN;
    /*设置新字体*/
    wcscpy_s(console_font_info.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(std_handle, FALSE, &console_font_info);

    return 0;
}

int Console::getScreenSize(int& screen_width, int& screen_height) {
    screen_width = GetSystemMetrics(SM_CXSCREEN);
    screen_height = GetSystemMetrics(SM_CYSCREEN);

    return 0;
}