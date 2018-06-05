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
git clone https://github.com/Teklad/jwaim.git
```
##### 2. Install dependencies
Debian:
```bash
sudo apt-get install libx11-dev libx11-dev:i386 libxtst-dev libconfig++-dev build-essential qt5-default libqt5x11extras5-dev
```
  Arch (thanks eclip):
```bash
sudo pacman -S libx11 libxtst libconfig base-devel qt5 qt5-x11extras
```

##### 3. Navigate to the base directory (usually ~/jwaim).
```bash
cd jwaim
```

##### 4. Prepare build directory
```bash
mkdir build && cd build
cmake ..
```

##### 5. Compile
```bash
make
```
##### 6. Modify settings.cfg in the build directory.

##### 7. Run jwaim as superuser

```bash 
sudo ./jwaim
```

##### 8. Set up required keybinds (use developer console '~')
```
bind mouse1 +alt2
bind space +alt1
```


NOTE:
Please make sure you have settings.cfg in the same directory as your current working directory when you run jwaim (Step 6), otherwise libconfig will not be able to find settings.cfg.
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
