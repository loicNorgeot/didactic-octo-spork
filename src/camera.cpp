#include "camera.h"

void Camera::updateView(){
    mLook = -mCam;
    mView = glm::lookAt(mCam, mLook, mUp);
}
void Camera::updateProj(){
    int width, height;
    GLFWwindow* win = glfwGetCurrentContext();
    glfwGetFramebufferSize(win, &width, &height);
    mProjection = glm::perspective( 1.0f, (float)width / (float)height, 0.001f, 100.0f);
}
void Camera::rotate(float angle, glm::vec3 axis){
    mCam   = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mCam,1));
    mLook  = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mLook,1));
    mUp    = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mUp,1));
    mRight = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mRight,1));
    mView  = glm::lookAt(mCam, mLook, mUp);
}
void Camera::zoom(float factor){
    if(glm::length(factor * mCam) > 0.1f && glm::length(factor * mCam) < 5.0f){
        mCam   = factor * mCam;
        mLook  = -mCam;
        mView  = glm::lookAt(mCam, mLook, mUp);
    }
}