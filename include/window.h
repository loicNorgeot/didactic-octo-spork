#ifndef __WINDOW_H_INCLUDED__
#define __WINDOW_H_INCLUDED__

#include "globals.h"

class Window{
public:
    GLFWwindow* mWindow;
    Scene* mScene;
    int mWidth, mHeight;
    std::string mTitle;
		GUI*                     mGUI;
    Window(int width, int height, std::string title);
    void addScene(Scene* scene);
};

#endif 