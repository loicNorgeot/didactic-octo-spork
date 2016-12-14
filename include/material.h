#ifndef __MATERIAL_H_INCLUDED__
#define __MATERIAL_H_INCLUDED__

#include "globals.h"

class Material{
public:
	MATERIAL mType;
	GLuint   mId;
	std::vector<Texture*> mTextures;
	Material(MATERIAL type);
	GLuint load(std::string vertexShader, std::string fragmentShader);
	void addTexture(Texture* texture);
};

#endif 