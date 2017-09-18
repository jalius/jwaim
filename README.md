# jwaim - csgo multi-cheat for linux
This is a cheat for the 64 bit linux csgo (tested on ubuntu 17.04)  
It utilizes QT graphics library to draw ESP and other info on screen  

I uploaded it here to share with anyone interested, and mainly just so that I don't lose track of it. I hope anyone who finds this can find it useful for learning or for fun.  

If you were wondering, I wrote this cheat nearly entirely from scratch. I used a memory wrapper and glow read/write functionality from github user thisgamesux. 

Development started 7/1/17  

Credits to: 
github.com user thisgamesux

Thanks to every helpful person at unknowncheats!

# installing and running
install additional dependencies:
```bash
apt-get install libboost-all-dev libx11-dev libx11-dev:i386 libxtst-dev libconfig++-dev build-essential qt5-default
```

how to build:

install the dependencies (above).

navigate to the base directory (usually ~/jwaim).

run command:
```bash
qmake ./jwaim.pro
```
run command: 
```bash
make
```
rename settings-example.cfg to settings.cfg and modify settings to your liking.

run jwaim as super user:

```bash 
sudo ./jwaim
```


NOTE:
please make sure you have settings.cfg in the same directory as your current working directory (when you execute the application), otherwise you will experience problems with libconfig.

# Cheat Features
**BHop  
Aim Assist  
Glow  
FOV Changer  
RCS  
RCS Crosshair  
ESP** - Boxes, Health, Line To Player, Name, Spectators, Player Status (Scoped/Defusing)

![Alt text](http://i.imgur.com/g2IU45i.jpg "screenshot")
![Alt text](https://i.imgur.com/xmjycBr.jpg "another screenshot")
