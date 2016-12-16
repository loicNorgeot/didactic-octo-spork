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
class GUI;

enum MATERIAL{MAT_COLOR, MAT_VERTEX_COLOR, MAT_TEXTURED};

#include "lodepng.h"
#include "text.h"
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

GLuint setupArrayBuffer(std::vector<float> *array);
GLuint setupElementsBuffer(std::vector<unsigned int> *array);
void linkAttribute(std::string name, GLuint buffer, GLuint program, int dimension, int index);

GLuint loadShaders(std::string vertexShader, std::string fragmentShader, std::string functions="");

#endif 