#include "material.h"

Material::Material(MATERIAL type){
	mType = type;
	std::string shadersDir = rootDir + "shaders/";
	switch(type){
		case MAT_COLOR:        shadersDir += "01 - plain color/"; break;
		case MAT_TEXTURED:     shadersDir += "02 - plain texture/"; break;
		case MAT_VERTEX_COLOR: shadersDir += "03 - plain vertex color/"; break;
	}
	mId = loadShaders(shadersDir + "shader.vert", shadersDir + "shader.frag");//, rootDir + "shaders/shader.functions");
}
void Material::addTexture(Texture* texture){
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, texture->mWidth, texture->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(texture->mImage)[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(mId, "myTextureSampler"), 0);
	mTextures.push_back(texture);
}