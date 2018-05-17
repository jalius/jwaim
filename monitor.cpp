#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

static std::atomic<bool> m_csgoActive;

static bool isFeatureSupported(Display *dpy, const char *feature)
{
    Atom nameAtom = XInternAtom(dpy, "_NET_SUPPORTED", True);
    Atom featureAtom = XInternAtom(dpy, feature, True);
    Atom type;
    Window root;
    int format, status;
    unsigned long nitems, after;
    unsigned char *data;
    
    root = XDefaultRootWindow(dpy);
    status = XGetWindowProperty(dpy, root, nameAtom, 0, 1024, 0,
            XA_ATOM, &type, &format, &nitems, &after, &data);
    
    if (status == Success && data != 0) {
        Atom *results = (Atom *)data; 
        for (auto i = 0UL; i < nitems; i++) {
            if (results[i] == featureAtom) {
                XFree(data);
                return true;
            }
        }
        XFree(data);
    }
    return false;
}

static Display *initializeDisplay()
{
    Display *dpy = XOpenDisplay(NULL);
    if (dpy != NULL) {
        if (isFeatureSupported(dpy, "_NET_WM_PID")) {
            XSelectInput(dpy, XDefaultRootWindow(dpy), SubstructureNotifyMask);
            puts("_NET_WM_PID supported.  Enabling window manager integration.");
        } else {
            XCloseDisplay(dpy);
        }
    }
    return dpy;
}

static void closeDisplay(Display *dpy)
{
    if (dpy != NULL) {
        XCloseDisplay(dpy);
        dpy = NULL;
    }
}

static bool checkCSGOActive(Display *dpy)
{
    Window window;
    int revert;
    Atom propAtom = XInternAtom(dpy, "_NET_WM_PID", True);
    Atom type;
    int format, status;
    long pid = -1;
    unsigned long nitems, after;
    unsigned char* data;

    XGetInputFocus(dpy, &window, &revert);

    status = XGetWindowProperty(dpy, window, propAtom, 0, 1024, 0,
            XA_CARDINAL, &type, &format, &nitems, &after, &data);
    if (status == Success && data != 0) {
        pid = *((long*)data);
        XFree(data);
    }
    
    if (pid == -1) {
        return false;
    }
    
    char pathbuf[512];
    char rlbuf[512];
    ssize_t rlsize;
    
    snprintf(pathbuf, 512, "/proc/%li/exe", pid);
    rlsize = readlink(pathbuf, rlbuf, sizeof(pathbuf)-1);
    if (rlsize == -1) {
        return false;
    }
    rlbuf[rlsize] = '\0';
    return strstr(rlbuf, "csgo_linux64") != NULL;
}

bool isCSGOActive()
{
    return m_csgoActive;
}

void csgoActivityLoop()
{
    Display *dpy = initializeDisplay();
    XEvent ev;
    while (dpy) {
        XNextEvent(dpy, &ev);
        if (ev.type == ConfigureNotify) {
            m_csgoActive = checkCSGOActive(dpy);
        }
    }
    m_csgoActive = true;
    closeDisplay(dpy);
}

