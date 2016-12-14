#ifndef __SCENE_H_INCLUDED__
#define __SCENE_H_INCLUDED__

#include "globals.h"

class Scene{
public:
    Camera* mCamera;
    std::vector<Object*> mObjects;
    Scene();
    void addCamera(Camera* cam);
    void addObject(Object* obj);
    void clear(glm::vec3 color);
    void render();
};

#endif 