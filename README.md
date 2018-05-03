![Alt text](https://i.imgur.com/xmjycBr.jpg "another screenshot")
# jwaim - csgo multi-cheat for linux
This is a cheat for the 64 bit linux csgo (tested on ubuntu 17.04)  
It utilizes QT graphics library to draw ESP and other info on screen  

I uploaded it here to share with anyone interested, and mainly just so that I don't lose track of it. I hope anyone who finds this can find it useful for learning or for fun.  

If you were wondering, I wrote this cheat almost entirely from scratch. I used a memory wrapper and glow read/write functionality from github user thisgamesux/s0beit.  

Development started 7/1/17  

# community
join the discord for setup help and other questions

https://discord.gg/59dJJYN
# installing and running
1. clone the repo
```bash
git clone https://github.com/bluuman/jwaim/
```
2. install dependencies

Debian:
```bash
sudo apt-get install libboost-all-dev libx11-dev libx11-dev:i386 libxtst-dev libconfig++-dev build-essential qt5-default
```
Arch (thanks eclip):
```bash
sudo pacman -S boost-libs libx11 libxtst libconfig base-devel qt5
```

3. navigate to the base directory (usually ~/jwaim).
```bash
cd jwaim
```

4. setup makefile with qmake
```bash
qmake ./jwaim.pro
```

5. compile program
```bash
make
```
6. rename settings_example.cfg to settings.cfg and modify settings to your liking.

7. run jwaim as superuser

```bash 
sudo ./jwaim
```
8. setup required keybinds (developer console: ~)
```
bind mouse1 +alt2
bind space +alt1
```


NOTE:
please make sure you have settings.cfg in the same directory as your current working directory when you run jwaim (step 6), otherwise you will experience problems with libconfig not being able to find settings.cfg.

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
