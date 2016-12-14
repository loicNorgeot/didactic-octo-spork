#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Application;
class Window;
class Scene;
class Camera;
class Object;
class Material;
class Texture;

enum MATERIAL{MAT_COLOR, MAT_VERTEX_COLOR, MAT_TEXTURED};

#include "lodepng.h"
#include "texture.h"
#include "material.h"
#include "object.h"
#include "camera.h"
#include "scene.h"
#include "window.h"
#include "application.h"

extern std::string rootDir;
extern bool verbose;
extern int  frames;
extern double chrono;

void exit(std::string message);
void FPS(double *t);
void FPS(double *t, int *fr);

#endif 