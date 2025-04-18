#pragma once

#include <iostream>
#include <Vector>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "console.h"

constexpr int FONTWIDTH = 8;
constexpr int FONTHEIGHT = 10;

using namespace std;
using namespace cv;

class Video : public Console {
public:
    /*
    * @brief 构造函数
    */
    Video();

    /*
    * @brief 判断是否正确初始化
    * @return 0 正确初始化
    * @return 1 未正确初始化
    */
    int isInitialized();

    /*
    * @brief 更改配置
    * @return 0 函数正常执行
    */
    int settings();

    /*
    * @brief 读取配置
    * @return 0 函数正常执行
    * @return 1 读取失败
    */
    int readSettings();

    /*
    * @brief 保存配置
    * @return 0 函数正常执行
    */
    int saveSettings() const;

    /*
    * @brief 显示视频的名称和帧信息
    * @return 0 函数正常执行
    */
    int showInfo() const;

    /*
    * @brief 显示视频
    * @return 0 函数正常执行
    */
    int showVideo();

    /*
    * @brief 读取视频文件
    * @return 读取成功返回0
    * @return 读取失败返回1
    */
    int readVideo();

    /*
    * @brief 获取视频中的帧
    * @param frame_time 读取的帧数,默认值为1
    * @return 读取成功返回0
    * @return 读取失败返回1
    */
    int readFrame(uint frame_time = 1);

    /*
    * @brief 创造符号帧图
    * @return 0 函数正常执行
    */
    int createCharacterFrame();

    /*
    * @brief 播放字符动画
    * @return 0 函数正常执行
    */
    int playCharacterAnimation();

    /*
    * @brief 压缩符号动画到text文件,使用长度编码压缩方式进行压缩
    * @return 0 函数正常执行
    */
    int saveToText();

    /*
    * @brief 读取压缩的符号动画,使用长度编码压缩方式进行解压
    * @return 0 函数正常执行
    * @return 1 读取失败
    */
    int readText();

    /*
    * @brief 读取压缩的符号动画,使用长度编码压缩方式进行解压
    * @param text_file_name 压缩的符号动画文件名
    * @return 0 函数正常执行
    * @return 1 读取失败
    */
    int readText(string text_file_name);

    /*
    * @brief 析构函数
    */
    ~Video();

private:
    /*屏幕的宽度*/
    int m_screen_width;
    /*屏幕的高度*/
    int m_screen_height;
    /*视频名称*/
    string m_video_name;
    /*原视频的像素值*/
    int m_rows, m_cols;
    /*列的缩放比*/
    float m_cols_ratio;
    /*行的缩放比*/
    float m_rows_ratio;
    /*缩放后的像素值*/
    int m_resized_rows, m_resized_cols;
    /*捕获视频*/
    VideoCapture m_capture;
    /*视频总帧数*/
    int m_total_frames;
    /*数据流*/
    ostringstream* m_buffer = new ostringstream;
    /*视频中的帧图*/
    Mat m_frame;
    /*缩放后的帧图*/
    Mat m_resized_frame;
    /*灰度帧图*/
    Mat m_gray_frame;
    /*二值帧图*/
    Mat m_binary_frame;
    /*符号帧图*/
    vector<string>m_character_frame;
    /*符号地图*/
    const char m_char_map[2] = { ' ','@' };
};