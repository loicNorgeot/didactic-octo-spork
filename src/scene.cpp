#include "scene.h"

Scene::Scene(){
	active=NULL;
}
void Scene::addCamera(Camera* cam){
    mCamera = cam;
}
void Scene::addObject(Object* obj){
    mObjects.push_back(obj);
}
void Scene::clear(glm::vec3 color){
    glClearColor(color.x, color.y, color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Scene::render(){
    for(int i = 0 ; i < mObjects.size() ; i++){
        Object* obj = mObjects[i];
        //obj->mModel = glm::rotate(obj->mModel, 00.0025f, glm::vec3(0,1,0));
        glm::mat4 MVP =  mCamera->mProjection * mCamera->mView * obj->mModel;
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
        //color = glm::rotate(color, 0.01f, glm::vec3(0,1,1));
        glUniform3f( glGetUniformLocation(obj->mMaterial->mId, "col"), color.x, color.y, color.z);
        glUniformMatrix4fv( glGetUniformLocation(obj->mMaterial->mId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv( glGetUniformLocation(obj->mMaterial->mId, "M"), 1, GL_FALSE, &(obj->mModel)[0][0]);
        glUniformMatrix4fv( glGetUniformLocation(obj->mMaterial->mId, "V"), 1, GL_FALSE, &(mCamera->mView)[0][0]);
        obj->render();
    }
}