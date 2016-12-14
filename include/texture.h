#ifndef __TEXTURE_H_INCLUDED__
#define __TEXTURE_H_INCLUDED__

#include "globals.h"

class Texture{
public:
	std::vector<unsigned char> mImage;
	unsigned mWidth, mHeight;
	Texture(std::string filename);
};

#endif 