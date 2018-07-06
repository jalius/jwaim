# jwaim - CS:GO multi-cheat for Linux
![Alt text](https://i.imgur.com/xmjycBr.jpg "another screenshot")

This is a cheat for the 64 bit Linux CS:GO. Utilizing system calls and clever algorithms it assists you to increase your accuracy and skill. It also uses QT graphics library to draw helpful information on your screen.
# community
##### join the discord for setup help and other questions
https://discord.gg/59dJJYN
# prerequisites
##### 1. X11 display server

##### 2. compositing window manager 
environments like GNOME and Unity have this included OR you can install a standalone window manager such as xcompmgr (as suggested by [Teklad](https://github.com/Teklad))
# Building
##### 1. Clone the repo
```bash
git clone https://github.com/bluuman/jwaim.git
```
##### 2. Install dependencies
Debian:
```bash
sudo apt-get install cmake libx11-dev libx11-dev:i386 libxtst-dev libconfig++-dev build-essential qt5-default libqt5x11extras5-dev
```
  Arch (thanks eclip):
```bash
sudo pacman -S cmake libx11 libxtst libconfig base-devel qt5 qt5-x11extras
```

##### 3. Prepare build directory
```bash
cd jwaim
mkdir build && cd build
cmake ..
```

##### 4. Compile
```bash
make
```

##### 5. Modify settings.cfg in the build directory.

##### 6. Run jwaim as superuser

```bash 
sudo ./jwaim
```
##### 8. Set up required keybinds (use developer console '~')
```
bind mouse1 +alt2
bind space +alt1
```

##### 7. Set up required keybinds (use developer console '~')
```
bind mouse1 +alt2
bind space +alt1
```


NOTE:
If settings.cfg isn't found in the binary directory jwaim will use sane defaults.
# Cheat Features
**BHop  
Aim Assist  
Glow  
FOV Changer  
RCS  
RCS Crosshair  
HitMarker  
ESP** - Boxes, Health, Line To Player, Name, Spectators, Player Status (Scoped/Defusing)

![Alt text](http://i.imgur.com/g2IU45i.jpg "screenshot")
