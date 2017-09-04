#include <qtextstream.h>
#include <iostream>

#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <QThread>

#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/extensions/XTest.h>

#include <unistd.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <libconfig.h++>

#include "hack.h"
#include "window.h"
#include "settings.h"
hack h;
int settings::wind_height = 1200;
int settings::wind_width = 1600;
float settings::cofLineTetherX = .5;
float settings::cofLineTetherY = .9;
int settings::wind_x = 0;
int settings::wind_y = 0;

void glowLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.glow();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
void aimLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.aim();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
void bhopLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.bhop();
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}
/*void multiQuickLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            //h.trigger();
            //h.setFov();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}*/
void multiSlowLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.noFlash();
            h.setHands();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
void checkKeysLoop(){
    while(h.checkKeys()){
        h.setIsConnected();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return;
}
int execQApp(){
    cout<<"exec q app"<<endl;
    char *argv[] = {"window", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;
    QApplication app(argc,argv);
    qWindow mainWindow;
    QFont font ("Courier", 30, 100);
    mainWindow.showFullScreen();// Create an instance of your woker
    return app.exec();
}
int main()
{
    XInitThreads();
    h.init();
    std::thread g(glowLoop);
    std::thread a(aimLoop);
    //std::thread mq(multiQuickLoop);
    std::thread ms(multiSlowLoop);
    std::thread b(bhopLoop);
    std::thread c(checkKeysLoop);
    std::thread q(execQApp);
    c.join();
    return 0;
}
