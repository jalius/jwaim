#include "jwsettings.h"

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

using namespace libconfig;

JWSettings::JWSettings()
{
    _m_cfg.setOptions(Setting::OptionSemicolonSeparators
            | Setting::OptionColonAssignmentForGroups
            | Setting::OptionOpenBraceOnSeparateLine);

    try {
        _m_cfg.readFile(m_output_file);
    } catch(const FileIOException &fioex) {
        std::cerr << "I/O error while reading file.\n";
    } catch(const ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
            << " - " << pex.getError() << '\n';
    }
    m_dpy = XOpenDisplay(NULL);
    fillColors();
}

JWSettings& JWSettings::getInstance()
{
    static JWSettings instance;
    return instance;
}

int JWSettings::toKeycode(const std::string &key)
{
    if (m_dpy == NULL)
        return 0;
    return XKeysymToKeycode(m_dpy, XStringToKeysym(key.c_str()));
}

void JWSettings::fillColors()
{
    const Setting& root = _m_cfg.getRoot();
    const Setting& colors = root["Colors"];
    
    int index = 0;

    for (double d : colors["Enemy"]) {
        m_colors[index++] = d;
    }
    index = 4;
    for (double d : colors["InCrosshair"]) {
        m_colors[index++] = d;
    }
    index = 8;
    for (double d : colors["Ally"]) {
        m_colors[index++] = d;
    }
    index = 12;
    for (double d : colors["EnemyESP"]) {
        m_colors[index++] = d;
    }
    index = 16;
    for (double d : colors["AllyESP"]) {
        m_colors[index++] = d;
    }
    index = 20;
    for (double d : colors["SecondaryESP"]) {
        m_colors[index++] = d;
    }

    m_colors[20] *= 255;
    m_colors[21] *= 255;
    m_colors[22] *= 255;
    m_colors[23] *= 255;
}
