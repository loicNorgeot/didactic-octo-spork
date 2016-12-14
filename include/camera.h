#ifndef __CAMERA_H_INCLUDED__
#define __CAMERA_H_INCLUDED__

#include "globals.h"

class Camera{
public:
    glm::vec3 mCam, mLook, mUp, mRight;
    glm::mat4 mProjection, mView;
    float mFov;
    Camera(){
        mFov = 45.0f;
        mCam = glm::vec3(1,1,1);
        mLook = -mCam;//glm::vec3(0,0,0);
        mUp = glm::normalize(glm::vec3(-1,1,-1));
        mRight = glm::cross(-mCam, mUp);
        mProjection = glm::perspective( 1.0f, 4.0f / 3.0f, 0.001f, 100.0f);
        mView = glm::lookAt(mCam, mLook, mUp);
    }
    void updateView();
    void updateProj();
    void rotate(float angle, glm::vec3 axis);
    void zoom(float factor);
};

#endif 