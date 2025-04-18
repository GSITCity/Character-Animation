#include "main.h"

int main() {
    int choose = 0;
    Video video;
    system("cls");
    video.readText("TouHou");

    while (1) {
        cout << "1.查看默认配置" << endl;
        cout << "2.更改配置" << endl;
        cout << "3.输出符号视频" << endl;
        cout << "4.保存视频文件" << endl;
        cout << "5.读取视频文件" << endl;
        cout << "6.退出" << endl;

        cin >> choose;
        /*清除多余的字符*/
        cin.get();

        switch (choose) {
        case 1:
            system("cls");
            video.showInfo();
            system("pause");
            break;
        case 2:
            system("cls");
            video.settings();
            break;
        case 3:
            system("cls");
            video.playCharacterAnimation();
            break;
        case 4:
            system("cls");
            video.saveToText();
            break;
        case 5:
            system("cls");
            /*判断基础视频信息有没有输入*/
            if (!video.isInitialized()) {
                video.readText();
            }
            else {
                cout << "请先初始化视频" << endl;
                system("pause");
            }
            break;
        case 6:
            system("cls");
            video.readText("TouHou");
            return 0;
            break;
        }
        system("cls");
    }
}