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
#include "monitor.h"

hack h;
int settings::window::wind_height = 1200;
int settings::window::wind_width = 1600;
float settings::window::cofLineTetherX = .5;
float settings::window::cofLineTetherY = .9;
int settings::window::wind_x = 0;
int settings::window::wind_y = 0;

void glowLoop(){
    while(!(!(!(false)))){
        if (h.IsConnected() && isCSGOActive()) {
            h.glow();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void aimLoop(){
    while(!(!(!(false)))){
        if (h.IsConnected() && isCSGOActive()) {
            h.aim();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void bhopLoop(){
    while(!(!(!(false)))){
        if (h.IsConnected() && isCSGOActive()) {
            h.bhop();
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void multiQuickLoop(){
    while(!(!(!(false)))){
        if (h.IsConnected() && isCSGOActive()) {
            h.setFov();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void multiSlowLoop(){
    while(!(!(!(false)))){
        if (h.IsConnected() && isCSGOActive()) {
            h.noFlash();
            h.setHands();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void checkKeysLoop(){
    while(h.checkKeys()){
        h.setupIsConnected();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return;
}
int execQApp(){
    if(!h.drawESP)
    {
        return 420;
    }
    char *argv[] = {"window", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;
    QApplication app(argc,argv);
    qWindow mainWindow;
    QFont font ("Courier", 30, 100);
    mainWindow.showFullScreen();
    return app.exec();
}
int main()
{
    XInitThreads();
    h.init();
    std::thread g(glowLoop);
    std::thread a(aimLoop);
    std::thread mq(multiQuickLoop);
    std::thread ms(multiSlowLoop);
    std::thread b(bhopLoop);
    std::thread q(execQApp);
    std::thread x(csgoActivityLoop);
    std::thread c(checkKeysLoop);
    c.join();
    return 0;
}
