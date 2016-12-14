#include "object.h"

GLuint setupArrayBuffer(std::vector<float> *array){
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, array->size()*sizeof(float), &(*array)[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(verbose){
        std::cout << "Initializing buffer for an array:" << std::endl;
        std::cout << "  Size of array = " << array->size() << std::endl;
        std::cout << "  Buffer ID     = " << buffer << std::endl;
        std::cout << "  Array adress  = " << &(*array)[0] << std::endl;
    }
    return buffer;
}
GLuint setupElementsBuffer(std::vector<unsigned int> *array){
    GLuint buffer;
    glGenBuffers( 1, &buffer);
    if(verbose){
        std::cout << "Initializing buffer for the indices:" << std::endl;
        std::cout << "  Size of array = " << array->size() << std::endl;
        std::cout << "  Buffer ID = " << buffer << std::endl;
        std::cout << "  Array adress = " << array << std::endl;
    }
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, array->size()*sizeof(unsigned int), &(*array)[0], GL_STATIC_DRAW);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
    return buffer;
}
void linkAttribute(std::string name, GLuint buffer, GLuint program, int dimension, int index){
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray( index);
    glVertexAttribPointer(     index, dimension, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(      program, index, name.c_str());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
Object::Object(std::string file){
    const struct aiScene* scene = NULL;
    scene = aiImportFile(file.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
    if(!scene){ exit("No scene found"); }
    if(scene->HasMeshes()){
        aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[0]];
        if(mesh->HasPositions()){
            mIndices.resize(3*mesh->mNumFaces);
            for(int i = 0 ; i < mesh->mNumFaces ; i++){
                for(int j = 0 ; j < 3 ; j++){
                    mIndices[3*i + j] = mesh->mFaces[i].mIndices[j];
                }
            }
            mVertices.resize(3*mesh->mNumVertices);
            for(int i = 0 ; i < mesh->mNumVertices ; i++){
                for(int j = 0 ; j < 3 ; j++){
                    mVertices[3*i + j] = mesh->mVertices[i][j];
                }
            }
            glm::vec3 mini(1e6,1e6,1e6),maxi(-1e6,-1e6,-1e6),center;
            for(int i = 0 ; i < mesh->mNumVertices ; i++){
                mini.x = std::min(mini.x, mVertices[3*i + 0]);
                mini.y = std::min(mini.y, mVertices[3*i + 1]);
                mini.z = std::min(mini.z, mVertices[3*i + 2]);
                maxi.x = std::max(maxi.x, mVertices[3*i + 0]);
                maxi.y = std::max(maxi.y, mVertices[3*i + 1]);
                maxi.z = std::max(maxi.z, mVertices[3*i + 2]);
            }
            center = (maxi + mini)/2.0f;
            float scale  = 1.5f/std::max(std::max(maxi.x - mini.x , maxi.y - mini.y), maxi.z - mini.z);
            for(int i = 0 ; i < mesh->mNumVertices ; i++){
                mVertices[3*i + 0] = scale * (mVertices[3*i + 0] - center.x);
                mVertices[3*i + 1] = scale * (mVertices[3*i + 1] - center.y);
                mVertices[3*i + 2] = scale * (mVertices[3*i + 2] - center.z);
            }
            if(mesh->HasTextureCoords(0)){
                mUvCoordinates.resize(2*mesh->mNumVertices);
                for(int i = 0 ; i < mesh->mNumVertices ; i++){
                    mUvCoordinates[2*i + 0] = mesh->mTextureCoords[0][i].x;
                    mUvCoordinates[2*i + 1] = mesh->mTextureCoords[0][i].y;
                }
            }
            if(mesh->HasNormals()){
                mNormals.resize(3*3*mesh->mNumVertices);
                for(int i = 0 ; i < mesh->mNumVertices ; i++){
                    for(int j = 0 ; j < 3 ; j++){
                        mNormals[3*i + j] = mesh->mNormals[i][j];
                    }
                }
            }
            if(mesh->HasVertexColors(0)){
                mVertexColors.resize(3*mesh->mNumVertices);
                for(int i = 0 ; i < mesh->mNumVertices ; i++){
                    for(int j = 0 ; j < 3 ; j++){
                        mVertexColors[3*i + j] = mesh->mColors[0][i][j];
                    }
                }
            }
            mModel = glm::rotate(glm::mat4(1.0f), 3.14157f,glm::vec3(1,0,0));
        }
        else{
            exit("No vertices or indices found in the mesh");
        }
    }
    else{
        exit("No valid scene in the file");
    }
}
void Object::addMaterial(Material *mat){
    mMaterial = mat;
    
    glUseProgram(mat->mId);
    
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);
    
    mVerticesBuffer = setupArrayBuffer(&mVertices);
    linkAttribute("vertexPosition", mVerticesBuffer, mat->mId, 3, 0);
    mIndicesBuffer = setupElementsBuffer(&mIndices);
    if(mVertexColors.size()>0){
        mVertexColorsBuffer = setupArrayBuffer(&mVertexColors);
        linkAttribute("vertexColor", mVertexColorsBuffer, mat->mId, 3, 1);
    }
    if(mUvCoordinates.size()>0){
        mUvCoordinatesBuffer = setupArrayBuffer(&mUvCoordinates);
        linkAttribute("vertexUV", mUvCoordinatesBuffer, mat->mId, 2, 2);
    }
    if(mNormals.size()>0){
        mNormalsBuffer = setupArrayBuffer(&mNormals);
        linkAttribute("vertexNormal", mNormalsBuffer, mat->mId, 3, 3);
    }
}
void Object::render(){
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(mMaterial->mId);
    glBindVertexArray(mVao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);			
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, (void*)0);		
    glBindVertexArray(0);
}