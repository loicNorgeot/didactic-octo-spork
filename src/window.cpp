#include "window.h"

Window::Window(int width, int height, std::string title){
    mWidth = width, mHeight = height, mTitle = title;
    mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if(!mWindow) {
        exit("Can not create GLFW window");
    }
    glfwMakeContextCurrent(mWindow);
    mScene = NULL;
}
void Window::addScene(Scene* scene){
    mScene = scene;
    std::cout << scene << " " << mScene << std::endl;
};