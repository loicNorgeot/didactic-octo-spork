#ifndef __OBJECT_H_INCLUDED__
#define __OBJECT_H_INCLUDED__

#include "globals.h"

class Object{
public:
    GLuint mVao;
    std::vector<float> mVertices, mVertexColors, mUvCoordinates, mNormals;
    std::vector<unsigned int> mIndices;
    GLuint mVerticesBuffer, mVertexColorsBuffer, mUvCoordinatesBuffer, mNormalsBuffer, mIndicesBuffer;
    Material* mMaterial;
    glm::mat4 mModel;
    Object(std::string file);
    void addMaterial(Material *mat);
    void render();
};

#endif 