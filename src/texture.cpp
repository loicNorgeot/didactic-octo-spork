#include "texture.h"

Texture::Texture(std::string filename){
  unsigned error = lodepng::decode(mImage, mWidth, mHeight, filename.c_str());
  if(error) 
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}