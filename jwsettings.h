#ifndef _JWSETTINGS_H_
#define _JWSETTINGS_H_
#include <libconfig.h++>
#include <X11/Xlib.h>
#include <array>

#define JW_SETTING(Name, Type, Default) \
    Type Name() const { \
    const libconfig::Setting& cat = _m_cfg.getRoot()[_m_category]; \
    Type val; \
    if (!cat.lookupValue(#Name, val)) { \
        return Default; \
    } \
    return val; }

#define BEGIN_CATEGORY(Name) \
    class _Category##Name { \
    private: \
    libconfig::Config& _m_cfg; \
    const char* _m_category = #Name; \
    public: \
    _Category##Name(libconfig::Config& cfg) : _m_cfg(cfg) {} \

#define END_CATEGORY(Name) \
    }; _Category##Name Name = _Category##Name(_m_cfg);

class JWSettings {
public:
    static JWSettings& getInstance();
    BEGIN_CATEGORY(Window)
        JW_SETTING(Width,           int,        1920 )
        JW_SETTING(Height,          int,        1080 )
        JW_SETTING(X,               int,        0    )
        JW_SETTING(Y,               int,        0    )
    END_CATEGORY(Window)

    BEGIN_CATEGORY(HitMarker)
        JW_SETTING(Enabled,         bool,       true )
        JW_SETTING(Width,           float,      1.3  )
        JW_SETTING(Length,          float,      10   )
        JW_SETTING(Time,            int,        10   )
    END_CATEGORY(HitMarker)

    BEGIN_CATEGORY(AimBot)
        JW_SETTING(FOV,             float,      1.7  )
        JW_SETTING(Smoothing,       float,      0.2  )
        JW_SETTING(FriendlyFire,    bool,       false)
        JW_SETTING(MaxBullets,      int,        0    )
        JW_SETTING(DistBasedFOV,    bool,       false)
        JW_SETTING(Bone,            int,        8    )
    END_CATEGORY(AimBot)

    BEGIN_CATEGORY(Visual)
        JW_SETTING(HideHands,       bool,       false)
        JW_SETTING(FOV,             float,      105  )
        JW_SETTING(CrosshairStatic, bool,       false)
        JW_SETTING(CrosshairRCS,    bool,       false)
        JW_SETTING(DrawOverlay,     bool,       true )
        JW_SETTING(FlashMax,        int,        100  )
    END_CATEGORY(Visual)

    BEGIN_CATEGORY(Glow)
        JW_SETTING(Style,           int,        0    )
        JW_SETTING(Bloom,           float,      0    )
        JW_SETTING(Items,           bool,       false)
    END_CATEGORY(Glow)

    BEGIN_CATEGORY(KeyMap)
        JW_SETTING(RCS,     std::string,     "Home"  )
        JW_SETTING(Radar,   std::string,     "F6"    )
        JW_SETTING(Glow,    std::string,     "F7"    )
        JW_SETTING(ESP,     std::string,     "F8"    )
        JW_SETTING(Bhop,    std::string,     "F9"    )
        JW_SETTING(Aim,     std::string,     "End"   )
        JW_SETTING(NoFlash, std::string,     "Delete")
        JW_SETTING(Rage,    std::string,     "Insert")
    END_CATEGORY(KeyMap)

    //Other misc helper functions
    int toKeycode(const std::string& key);
    const std::array<double, 24>& Colors() { return m_colors; }
private:
    JWSettings();
    JWSettings(JWSettings const&) = delete;
    JWSettings& operator=(JWSettings const&) = delete;
    libconfig::Config _m_cfg;
    const char* m_output_file = "settings.cfg";
    Display* m_dpy = NULL;
    std::array<double, 24> m_colors = {};

    void fillColors();
};

#endif
