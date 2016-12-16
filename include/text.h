#ifndef __TEXT_H_INCLUDED__
#define __TEXT_H_INCLUDED__

#include "globals.h"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint TextureID;    
    glm::ivec2 Size;     
    glm::ivec2 Bearing;  
    GLuint Advance;      
};

class GUI{
public:
  GLuint VAO, VBO, fontID;
  std::map<GLchar, Character> Characters;
  GUI(std::string vertPath, std::string fragPath, std::string font);
  void text(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif