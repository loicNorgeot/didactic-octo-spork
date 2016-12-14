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

GLuint setupArrayBuffer(std::vector<float> *array);
GLuint setupElementsBuffer(std::vector<unsigned int> *array);
void linkAttribute(std::string name, GLuint buffer, GLuint program, int dimension, int index);

#endif 