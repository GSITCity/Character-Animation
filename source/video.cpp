#include "Video.h"

 /*全局变量,用以在循环中只执行一次*/
int first_loop = 1;

Video::Video() {
    /*设置日志等级为静默*/
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    getScreenSize(m_screen_width, m_screen_height);

    /*读取设置并判断是否读取成功*/
    if (!readSettings()) {
        cout << "读取设置成功" << endl;
        system("pause");
    }
    else {
        cout << "读取设置失败" << endl;
        system("pause");
    }

    /*清屏*/
    system("cls");
    /*设置初始字体大小*/
    setConsoleFontSize(8, 16);
}

int Video::settings() {
    cout << "请输入视频名称:";
    getline(cin, m_video_name);
    /*读取视频和第一帧*/
    readVideo();
    readFrame();
    /*获取视频的总帧数*/
    m_total_frames = (int)(m_capture.get(CAP_PROP_FRAME_COUNT));
    /*输入视频缩放比*/
    m_rows = m_frame.rows;
    m_cols = m_frame.cols;
    cout << "视频分辨率:" << m_cols << "*" << m_rows << endl;
    cout << "输入列缩放比:";
    cin >> m_cols_ratio;
    cout << endl;
    cout << "输入行缩放比:";
    cout << endl;
    cin >> m_rows_ratio;
    /*计算缩放后的分辨率*/
    m_resized_cols = int(m_cols * m_cols_ratio);
    m_resized_rows = int(m_rows * m_rows_ratio);

    /*释放掉获取的视频和第一帧*/
    m_capture.release();
    m_frame.release();

    /*保存设置*/
    saveSettings();
    return 0;
}

int Video::readSettings() {
    fstream setting_file;
    setting_file.open("../resource/" "setting", ios::in);
    if (!setting_file) {
        return 1;
    }

    /*从文件中获取设置信息*/
    getline(setting_file, m_video_name);
    setting_file >> m_total_frames;
    setting_file >> m_cols;
    setting_file >> m_rows;
    setting_file >> m_cols_ratio;
    setting_file >> m_rows_ratio;

    /*计算缩放后的行与列*/
    m_resized_cols = (int)(m_cols * m_cols_ratio);
    m_resized_rows = (int)(m_rows * m_rows_ratio);

    setting_file.close();
    isInitialized();
    return 0;
}

int Video::isInitialized() {
    /*判断读取是否成功*/
    if (m_video_name.empty()
        || m_total_frames == 0
        || m_cols == 0
        || m_rows == 0
        || m_cols_ratio == 0
        || m_rows_ratio == 0) {
        return 1;
    }
    return 0;
}

int Video::saveSettings() const {
    fstream setting_file;
    setting_file.open("../resource/""setting", ios::out);
    if (!setting_file) {
        cout << "无法打开配置文件" << endl;
        return 1;
    }

    /*保存设置*/
    setting_file << m_video_name << endl;
    setting_file << m_total_frames << endl;
    setting_file << m_cols << endl;
    setting_file << m_rows << endl;
    setting_file << m_cols_ratio << endl;
    setting_file << m_rows_ratio << endl;

    setting_file.close();
    return 0;
}

int Video::showInfo() const {
    cout << left << setw(20) << setfill('-') << "视频的名称:" << right << setw(20) << setfill('-') << m_video_name << endl;
    cout << left << setw(20) << setfill('-') << "视频的总帧数:" << right << setw(20) << setfill('-') << m_total_frames << endl;
    cout << left << setw(20) << setfill('-') << "视频列宽:" << right << setw(20) << setfill('-') << m_cols << endl;
    cout << left << setw(20) << setfill('-') << "视频行宽:" << right << setw(20) << setfill('-') << m_rows << endl;
    cout << left << setw(20) << setfill('-') << "列缩放比:" << right << setw(20) << setfill('-') << m_cols_ratio << endl;
    cout << left << setw(20) << setfill('-') << "行缩放比:" << right << setw(20) << setfill('-') << m_rows_ratio << endl;

    return 0;
}

int Video::showVideo() {
    /*循环读取显示视频中的每一帧*/
    while (1) {
        m_capture >> m_frame;
        if (m_frame.empty()) {
            break;
        }
        imshow(m_video_name, m_frame);
        waitKey(20);
    }
    m_capture.release();
    return 0;
}

int Video::readVideo() {
    m_capture.open("../resource/" + m_video_name + ".mp4");
    if (m_capture.isOpened()) {
        cout << "打开成功" << endl;
    }
    else {
        cout << "打开失败" << endl;
        return 1;
    }

    return 0;
}

int Video::readFrame(uint frame_time) {
    for (; frame_time > 0; frame_time--) {
        m_capture >> m_frame;
        if (m_frame.empty()) {
            return 1;
        }
    }
    return 0;
}

int Video::createCharacterFrame() {
    /*初始化符号帧图*/
    m_character_frame = vector<string>(m_resized_rows, string(m_resized_cols, ' '));

    /*重新定义大小*/
    resize(m_frame, m_resized_frame, Size(m_resized_cols, m_resized_rows));
    /*转为灰度图*/
    cvtColor(m_resized_frame, m_gray_frame, COLOR_BGR2GRAY);
    /*转为二值化图*/
    threshold(m_gray_frame, m_binary_frame, 128, 255, THRESH_BINARY);

    for (int y = 0; y < m_resized_rows; y++) {
        for (int x = 0; x < m_resized_cols; x++) {
            /*将二值化帧图的像素映射到符号帧图*/
            m_character_frame[y][x] = m_char_map[m_binary_frame.at<uchar>(y, x) & 0x01];
        }
    }
    return 0;
}

int Video::playCharacterAnimation() {
    setConsoleFontSize(FONTWIDTH, FONTHEIGHT);
    int x_bias = (int)((m_screen_width - m_resized_cols * FONTWIDTH) * 0.5f * (1.0f / FONTWIDTH) + m_resized_cols);
    system("cls");
    maxScreen();
    hideCursor();

    readVideo();
    /*将视频转化为符号动画输出到控制台中*/
    while (1) {
        moveCursor();

        readFrame();
        /*若帧为空则退出循环*/
        if (m_frame.empty()) {
            break;
        }
        createCharacterFrame();
        /*显示原视频*/
        imshow(m_video_name, m_frame);

        /*循环读取符号帧图的行数据,将行拼接为帧存放到m_buffer中*/
        for (const auto& row : m_character_frame) {
            *m_buffer << setw(x_bias) << row << "\n";
        }
        cout << m_buffer->str();
        /*清空内容*/
        m_buffer->str("");
        /*重置状态*/
        m_buffer->clear();
        /*回到开头*/
        m_buffer->seekp(0);

        waitKey(20);
    }

    /*释放掉获取的视频*/
    m_capture.release();
    /*关闭视频窗口*/
    destroyAllWindows();
    /*重新设置字体大小*/
    setConsoleFontSize(8, 16);
    return 0;
}

int Video::saveToText() {
    /*打开文件*/
    ofstream  video_text_file("../resource/" + m_video_name + ".txt");
    readVideo();
    /*行程长度*/
    int num_data;
    /*存放符号*/
    char char_data;
    /*计数,判断5%的倍数进度*/
    int count = 1;

    system("cls");
    hideCursor();

    /*画出进度条*/
    cout << setw(15) << setfill(' ') << "正在保存" << endl;
    cout << '[' << setfill(' ') << setw(21) << ']';

    for (int i = 0; i < m_total_frames; i++) {
        readFrame();
        createCharacterFrame();

        /*根据进度更新进度条*/
        if (i > m_total_frames * 0.05f * count) {
            /*光标回到进度条开始位置*/
            moveCursor(1, 1);
            /*输出进度条指示符号*/
            cout << string(count - 1, '=') << '>';
            count++;
        }

        /*循环读取符号帧图的行数据,按照行程长度压缩方式压缩*/
        for (const auto& row : m_character_frame) {
            char_data = row[0];
            num_data = 1;
            for (int j = 1; j <= m_resized_cols; j++) {
                /*如果符号发生变化或到行末,更换符号将之前符号的长度保存*/
                if ((char_data != row[j]) || (j == m_resized_cols)) {
                    video_text_file << num_data << char_data;
                    char_data = row[j];
                    num_data = 0;
                    /*如果到了行末,跳出循环*/
                    if (j == m_resized_cols) {
                        break;
                    }
                }
                num_data++;
            }
            video_text_file << "\n";
        }

        /*每帧末尾添加符号地图大小2倍的'-'*/
        video_text_file << string(sizeof(m_char_map) * 2, '-') << endl;
    }

    m_capture.release();
    video_text_file.close();

    return 0;
}

int Video::readText() {
    ifstream video_text_file("../resource/" + m_video_name + ".txt");
    if (video_text_file.fail()) {
        return 1;
    }
    /*存放压缩的数据*/
    string compressed_line;
    /*存放解压后的数据*/
    string decompressed_line = {};
    /*存放char型数字转化成的int型数据*/
    int num_data = 0;
    /*控制台中X坐标的偏置,以达到居中的效果*/
    int x_bias = 0;
    /*设置循环执行一次的标签*/
    first_loop = 1;

    setConsoleFontSize(FONTWIDTH, FONTHEIGHT);
    system("cls");
    maxScreen();
    hideCursor();

    while (1) {
        if (video_text_file.eof()) {
            break;
        }

        /*从文件中读取数据,解码*/
        while (1) {
            getline(video_text_file, compressed_line);
            if ((compressed_line[0] == '-') || (compressed_line.empty())) {
                decompressed_line = {};
                break;
            }
            /*屏幕宽度的一半再加上符号帧图的一半*/

            /*解码一行数据*/
            for (int i = 0; i < compressed_line.length(); i++) {
                /*如果是数字,则转化为int型数据,若不是,则根据符号及其数量解码一行存放到decompressed_line中*/
                if ((unsigned char)(compressed_line[i] - '0') < 10) {
                    /*第二位char型数字开始进位*/
                    if (num_data > 0) {
                        num_data *= 10;
                    }
                    /*将char型数字转为int型数字*/
                    num_data += (int)(compressed_line[i] - '0');
                }
                else {
                    if (first_loop) {
                        x_bias = (int)(m_screen_width * (1.0f / FONTWIDTH) * 0.5f + num_data * 0.5f);
                        first_loop = 0;
                    }
                    decompressed_line += string(num_data, compressed_line[i]);
                    num_data = 0;
                }
            }
            /*将每行decompressed_line数据存放到m_buffer中形成帧*/
            *m_buffer << setw(x_bias) << decompressed_line << "\n";
            decompressed_line = {};
        }

        /*光标移动到初始位置*/
        moveCursor();
        m_buffer->width(312);
        cout << m_buffer->str();
        m_buffer->str("");
        /*清空内容*/
        m_buffer->str("");
        /*重置状态*/
        m_buffer->clear();
        /*回到开头*/
        m_buffer->seekp(0);
        /*延迟*/
        waitKey(20);
    }

    /*还原字体大小*/
    setConsoleFontSize(8, 16);
    /*关闭文件*/
    video_text_file.close();
    return 0;
}

int Video::readText(string text_file_name) {
    ifstream video_text_file("../resource/" + text_file_name + ".txt");
    if (video_text_file.fail()) {
        return 1;
    }
    /*存放压缩的数据*/
    string compressed_line;
    /*存放解压后的数据*/
    string decompressed_line = {};
    /*存放char型数字转化成的int型数据*/
    int num_data = 0;
    /*控制台中X坐标的偏置,以达到居中的效果*/
    int x_bias = 0;
    /*设置循环执行一次的标签*/
    first_loop = 1;

    setConsoleFontSize(FONTWIDTH, FONTHEIGHT);
    system("cls");
    maxScreen();
    hideCursor();

    while (1) {
        if (video_text_file.eof()) {
            break;
        }

        /*从文件中读取数据,解码*/
        while (1) {
            getline(video_text_file, compressed_line);


            if ((compressed_line[0] == '-') || (compressed_line.empty())) {
                decompressed_line = {};
                break;
            }
            /*屏幕宽度的一半再加上符号帧图的一半*/

            /*解码一行数据*/
            for (int i = 0; i < compressed_line.length(); i++) {
                /*如果是数字,则转化为int型数据,若不是,则根据符号及其数量解码一行存放到decompressed_line中*/
                if ((unsigned char)(compressed_line[i] - '0') < 10) {
                    /*第二位char型数字开始进位*/
                    if (num_data > 0) {
                        num_data *= 10;
                    }
                    /*将char型数字转为int型数字*/
                    num_data += (int)(compressed_line[i] - '0');
                }
                else {
                    if (first_loop) {
                        x_bias = (int)(m_screen_width * (1.0f / FONTWIDTH) * 0.5f + num_data * 0.5f);
                        first_loop = 0;
                    }
                    decompressed_line += string(num_data, compressed_line[i]);
                    num_data = 0;
                }
            }
            /*将每行decompressed_line数据存放到m_buffer中形成帧*/
            *m_buffer << setw(x_bias) << decompressed_line << "\n";
            decompressed_line = {};
        }

        /*光标移动到初始位置*/
        moveCursor();
        m_buffer->width(312);
        cout << m_buffer->str();
        m_buffer->str("");
        /*清空内容*/
        m_buffer->str("");
        /*重置状态*/
        m_buffer->clear();
        /*回到开头*/
        m_buffer->seekp(0);
        /*延迟*/
        waitKey(20);
    }

    /*还原字体大小*/
    setConsoleFontSize(8, 16);
    /*关闭文件*/
    video_text_file.close();
    return 0;
}

Video::~Video() {
    /*保存配置*/
    saveSettings();
    /*释放内存*/
    delete[] m_buffer;
}