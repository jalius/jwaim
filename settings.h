#ifndef SETTINGS_H
#define SETTINGS_H
namespace settings {
  namespace window {
    extern int wind_height;
    extern int wind_width;
    extern int wind_x;
    extern int wind_y;
    extern float cofLineTetherY; // multiplied by the screen height to determine
    // where the enemy lines are tethered to
    extern float cofLineTetherX; // multiplied by the screen width to determine
    // where the enemy lines are tethered to
  }
}
#endif // SETTINGS_H
