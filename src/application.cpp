#include "application.h"

Application::Application(){
    if(!setupGLFW()){ exit("Failed to initialize GLFW");}
}
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}
int Application::setupGLFW(){
    if (!glfwInit()) {
        return 0;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return 1;
}
Window* Application::addWindow(int width, int height, std::string title){
    Window* win = new Window(width, height, title);
    if(!setupGLEW()){ exit("Failed to initialize GLEW");}
    setupInteractions(win);
    windows.push_back(win);
    return win;
}
int Application::setupGLEW(){
    glewExperimental = GL_TRUE;
    glewInit();
    return 1;
}
bool Application::shouldClose(){
    for ( auto &w : windows ) {
        if(glfwWindowShouldClose(w->mWindow)){
            return true;
        }
    }
    return false;
}
Window* Application::activeWindow(){
    GLFWwindow* active = glfwGetCurrentContext();
    Window* win = NULL;
    for(Window* w : windows){
        if(w->mWindow == active){
            win = w;
        }
    }
    return win;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    app->keyCallback(window, key, scancode, action, mods);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    app->mouseButtonCallback(window, button, action, mods);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    app->mousePositionCallback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xscroll, double yscroll){
    Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    app->scrollCallback(window, xscroll, yscroll);
}
void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS){
        windows.resize(2);
        std::cout << "Window adress = " << window << std::endl;
        for(Window* w : windows){
            if(window == w->mWindow){
                std::cout << "  Scene adress = " << w->mScene << std::endl;
                for(auto &o : w->mScene->mObjects){
                    std::cout << "    Object adress = " << o << std::endl;
                }
            }
        }
    }
}
void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    std::cout << "Pressed the left button!" << std::endl;
}
void Application::mousePositionCallback(GLFWwindow* window, int xpos, int ypos){
    Window *win = activeWindow();
    glm::vec2 depl(xpos - lastX, ypos - lastY);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        win->mScene->mCamera->rotate(-0.01f * depl.y, win->mScene->mCamera->mRight);
        win->mScene->mCamera->rotate(-0.01f * depl.x, win->mScene->mCamera->mUp);
    }
    lastX = xpos;
    lastY = ypos;
}
void Application::scrollCallback(GLFWwindow* window, double xscroll, double yscroll){
    Window *win = activeWindow();
    win->mScene->mCamera->zoom(1-0.01f*yscroll);
}
void Application::setupInteractions(Window* window){
    glfwSetWindowUserPointer(window->mWindow, this);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window->mWindow, key_callback);
    glfwSetMouseButtonCallback(window->mWindow, mouse_button_callback);
    glfwSetCursorPosCallback(window->mWindow, cursor_position_callback);
    glfwSetScrollCallback(window->mWindow, scroll_callback);
}

Object*   Application::addObject(std::string file){
    Object *obj = new Object(file);
    objects.push_back(obj);
    return obj;
}
Material* Application::addMaterial(MATERIAL type){
    Material *mat = new Material(type);
    materials.push_back(mat);
    return mat;
}
Texture*  Application::addTexture(std::string file){
    Texture *tex = new Texture(file);
    textures.push_back(tex);
    return tex;
}
Scene*    Application::addScene(){
    Scene* sce = new Scene();
    scenes.push_back(sce);
    return sce;
}
Camera*   Application::addCamera(){
    Camera* cam = new Camera();
    cameras.push_back(cam);
    return cam;
}