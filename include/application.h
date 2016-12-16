#ifndef __APPLICATION_H_INCLUDED__
#define __APPLICATION_H_INCLUDED__

#include "globals.h"

class Application{
public:
    bool verbose;
    std::vector<Window*>     windows;
    std::vector<Object*>     objects;
    std::vector<Material*>   materials;
    std::vector<Texture*>    textures;
    std::vector<Scene*>      scenes;
    std::vector<Camera*>     cameras;
    int                      lastX, lastY;
    
    Application();
    
    int setupGLFW();
    Window* addWindow(int width, int height, std::string title);
    Window* activeWindow();
    int setupGLEW();
    
    void setupInteractions(Window* window);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mousePositionCallback(GLFWwindow* window, int xpos, int ypos);
    void scrollCallback(GLFWwindow* window, double xscroll, double yscroll);
    bool shouldClose();
    
    Object*   addObject(std::string file);
    Material* addMaterial(MATERIAL mat);
    Texture*  addTexture(std::string file);
    Scene*    addScene();
    Camera*   addCamera();
};

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xscroll, double yscroll);

#endif 