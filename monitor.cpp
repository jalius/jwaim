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
    } else if (data != 0) {
        XFree(data);
    }
    return false;
}

static int getActiveWindow(Display *dpy, Window *window_ret)
{   
    Atom nameAtom = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", True);
    Atom type;
    Window root;
    int format, status;
    unsigned long nitems, after;
    unsigned char *data;
    
    root = XDefaultRootWindow(dpy);
    status = XGetWindowProperty(dpy, root, nameAtom, 0, 1024, 0,
            XA_WINDOW, &type, &format, &nitems, &after, &data);
    
    if (status == Success && data != 0) {
        *window_ret = *((Window*)data);
        XFree(data);
    } else {
        *window_ret = 0;
    }
    
    return nitems > 0 ? 1 : 0;
}

static long getWindowPID(Display *dpy, Window window)
{
    Atom nameAtom = XInternAtom(dpy, "_NET_WM_PID", True);
    Atom type;
    int format, status;
    long pid = -1;
    unsigned long nitems, after;
    unsigned char *data;

    status = XGetWindowProperty(dpy, window, nameAtom, 0, 1024, 0,
            XA_CARDINAL, &type, &format, &nitems, &after, &data);
    if (status == Success && data != 0) {
        pid = *((long*)data);
        XFree(data);
    }
    return pid;
}

static Display *initializeDisplay()
{
    Display *dpy = XOpenDisplay(NULL);
    if (dpy != NULL) {
        if (isFeatureSupported(dpy, "_NET_WM_PID") &&
                isFeatureSupported(dpy, "_NET_ACTIVE_WINDOW")) {
            XSelectInput(dpy, XDefaultRootWindow(dpy), SubstructureNotifyMask);
            puts("_NET_WM_PID and _NET_ACTIVE_WINDOW supported.  Enabling window manager integration.");
            return dpy;
        } else {
            XCloseDisplay(dpy);
        }
    }

    return NULL;
}

static void closeDisplay(Display *dpy)
{
    if (dpy != NULL) {
        XCloseDisplay(dpy);
        dpy = NULL;
    }
}

/**
 * @brief Checks whether CSGO is being actively used.
 *
 * @param dpy
 *
 * @return bool
 */
static bool checkCSGOActive(Display *dpy)
{
    Window activeWindow;
    int pid = -1;
    ssize_t rlsize;
    char pathbuf[512];
    char outbuf[512];

    if (getActiveWindow(dpy, &activeWindow)) {
        pid = getWindowPID(dpy, activeWindow);
    }

    if (pid == -1) {
        return false;
    }

    snprintf(pathbuf, 512, "/proc/%i/exe", pid);
    rlsize = readlink(pathbuf, outbuf, sizeof(pathbuf)-1);
    if (rlsize == -1) {
        return false;
    }
    outbuf[rlsize] = '\0';
    return strstr(outbuf, "csgo_linux64") != NULL;
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
        switch (ev.type) {
            case ConfigureNotify:
                m_csgoActive = checkCSGOActive(dpy);
                break;
        }
    }
    m_csgoActive = true;
    closeDisplay(dpy);
}

