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

#include "lodepng.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*
0 - Code syntax and standards:
	Class names start with a capital: MyClass
	Enums are in capitals and spaced by underscores: MAT_COLOR
	Variables follow the classical capitalization: myVariable
	Pointers are specified near the variable for declaration, or near the type for functions: int *a - foo(int* a)
	The directory strings FINISH with a "/" sign
	Class member start with a m: mClassMember
1 - Code structure:
	The code is OOP, meaning you create classes, and manage pointers of instances of classes.
	- Application
  	- Windows
			- Scene
				- Camera
				- Objects
					- Material
						- Texture
*/


/////////////////////////////////////////
// Helpers, debugging and globals
/////////////////////////////////////////

std::string rootDir = "/Users/guestaccount/dev/OpenGL2017/";
bool verbose = false;
int  frames   = 0;
double chrono;

enum MATERIAL{MAT_COLOR, MAT_VERTEX_COLOR, MAT_TEXTURED};

void exit(std::string message){
	std::cout << message << std::endl;
	exit(1);
}
void FPS(double *t){
	(*t) = glfwGetTime();
}
void FPS(double *t, int *fr){
	if(glfwGetTime() - (*t) > 1.0){
		std::cout << "FPS = " << frames/(glfwGetTime() - (*t)) << std::endl;
		(*t) = glfwGetTime();
		frames = 0;
	}
	frames++;
}


/////////////////////////////////////////
// Material class
/////////////////////////////////////////

class Texture{
public:
	std::vector<unsigned char> mImage;
	unsigned mWidth, mHeight;
	Texture(std::string filename);
};

Texture::Texture(std::string filename){
  unsigned error = lodepng::decode(mImage, mWidth, mHeight, filename.c_str());
  if(error) 
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}


class Material{
public:
	MATERIAL mType;
	GLuint   mId;
	std::vector<Texture*> mTextures;
	Material(MATERIAL type);
	GLuint load(std::string vertexShader, std::string fragmentShader);
	void addTexture(Texture* texture);
};

Material::Material(MATERIAL type){
	mType = type;
	std::string shadersDir = rootDir + "shaders/";
	switch(type){
		case MAT_COLOR:        shadersDir += "01 - plain color/"; break;
		case MAT_TEXTURED:     shadersDir += "02 - plain texture/"; break;
		case MAT_VERTEX_COLOR: shadersDir += "03 - plain vertex color/"; break;
	}
	mId = load(shadersDir + "shader.vert", shadersDir + "shader.frag");
}
GLuint Material::load(std::string vertexShader, std::string fragmentShader){
	//Cree les shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Lit le code du vertex shader à partir du fichier
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertexShader.c_str(), std::ios::in);
  if(VertexShaderStream.is_open()){
    std::string Line = "";
    while(getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else{exit(144);}

  // Lit le code du fragment shader à partir du fichier
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragmentShader.c_str(), std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::string Line = "";
    while(getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }
  else{exit(145);}

	const char* vs_code = VertexShaderCode.c_str();
	const char* fs_code = FragmentShaderCode.c_str();
	
	GLuint program = glCreateProgram();
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fs_code, NULL);
  glCompileShader(fs);
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vs_code, NULL);
  glCompileShader(vs);
	
	//Debug des shaders
	//Vertex
	glGetShaderiv(vs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(vs, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	if(InfoLogLength > 0){
		std::cout << "  WARNING: Vertex Shader compilation:\n    " << &VertexShaderErrorMessage[0] << std::endl;
		exit(1);
	}
	//Fragment
	glGetShaderiv(fs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(fs, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	if(InfoLogLength > 0){
		std::cout << "  WARNING: Fragment Shader compilation:\n    " << &FragmentShaderErrorMessage[0] << std::endl;
		exit(1);
	}
	
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

	return program;
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


/////////////////////////////////////////
// Object class
/////////////////////////////////////////

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


/////////////////////////////////////////
// Camera class
/////////////////////////////////////////

class Camera{
public:
	glm::vec3 mCam, mLook, mUp, mRight;
	glm::mat4 mProjection, mView;
	float mFov;
	Camera(){
		mFov = 45.0f;
		mCam = glm::vec3(1,1,1);
		mLook = -mCam;//glm::vec3(0,0,0);
		mUp = glm::normalize(glm::vec3(-1,1,-1));
		mRight = glm::cross(-mCam, mUp);
		mProjection = glm::perspective( 1.0f, 4.0f / 3.0f, 0.001f, 100.0f);
		mView = glm::lookAt(mCam, mLook, mUp);
	}
	void updateView();
	void updateProj();
	void rotate(float angle, glm::vec3 axis);
	void zoom(float factor);
};

void Camera::updateView(){
	mLook = -mCam;
	mView = glm::lookAt(mCam, mLook, mUp);
}
void Camera::updateProj(){
	int width, height;
	GLFWwindow* win = glfwGetCurrentContext();
	glfwGetFramebufferSize(win, &width, &height);
	mProjection = glm::perspective( 1.0f, (float)width / (float)height, 0.001f, 100.0f);
}
void Camera::rotate(float angle, glm::vec3 axis){
	mCam   = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mCam,1));
	mLook  = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mLook,1));
	mUp    = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mUp,1));
	mRight = glm::vec3(glm::rotate(glm::mat4(1), angle, axis) * glm::vec4(mRight,1));
	mView  = glm::lookAt(mCam, mLook, mUp);
}
void Camera::zoom(float factor){
	if(glm::length(factor * mCam) > 0.1f && glm::length(factor * mCam) < 5.0f){
		mCam   = factor * mCam;
		mLook  = -mCam;
		mView  = glm::lookAt(mCam, mLook, mUp);
	}
}


/////////////////////////////////////////
// Scene class
/////////////////////////////////////////

class Scene{
public:
	Camera* mCamera;
	std::vector<Object*> mObjects;
	Scene();
	void addCamera(Camera* cam);
	void addObject(Object* obj);
	void clear(glm::vec3 color);
	void render();
};

Scene::Scene(){}
void Scene::addCamera(Camera* cam){
	mCamera = cam;
}
void Scene::addObject(Object* obj){
	mObjects.push_back(obj);
}
void Scene::clear(glm::vec3 color){
	glClearColor(color.x, color.y, color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Scene::render(){
	for(int i = 0 ; i < mObjects.size() ; i++){
		Object* obj = mObjects[i];
		//obj->mModel = glm::rotate(obj->mModel, 00.0025f, glm::vec3(0,1,0));
		glm::mat4 MVP =  mCamera->mProjection * mCamera->mView * obj->mModel;
		glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
		//color = glm::rotate(color, 0.01f, glm::vec3(0,1,1));
		glUniform3f( glGetUniformLocation(obj->mMaterial->mId, "col"), color.x, color.y, color.z);
		glUniformMatrix4fv( glGetUniformLocation(obj->mMaterial->mId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv( glGetUniformLocation(obj->mMaterial->mId, "M"), 1, GL_FALSE, &(obj->mModel)[0][0]);
		glUniformMatrix4fv( glGetUniformLocation(obj->mMaterial->mId, "V"), 1, GL_FALSE, &(mCamera->mView)[0][0]);
		obj->render();
	}
}


/////////////////////////////////////////
// Window class
/////////////////////////////////////////

class Window{
public:
	GLFWwindow* mWindow;
	Scene* mScene;
	int mWidth, mHeight;
	std::string mTitle;
	Window(int width, int height, std::string title);
	void addScene(Scene* scene);
};

Window::Window(int width, int height, std::string title){
	mWidth = width, mHeight = height, mTitle = title;
	mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if(!mWindow) {
    exit("Can not create GLFW window");
  }
	glfwMakeContextCurrent(mWindow);
	mScene = NULL;
}
void Window::addScene(Scene* scene){
	mScene = scene;
	std::cout << scene << " " << mScene << std::endl;
};


/////////////////////////////////////////
// Application class
/////////////////////////////////////////

class Application{
public:
	bool verbose;
	std::vector<Window*>     windows;
	std::vector<Object*>     objects;
	std::vector<Material*>   materials;
	std::vector<Texture*>    textures;
	std::vector<Scene*>      scenes;
	std::vector<Camera*>     cameras;
	int                      lastX, lastY;
	
	Application();
	
	int setupGLFW();
	//GLFWwindow* addWindow();
	Window* addWindow(int width, int height, std::string title);
	Window* activeWindow();
	int setupGLEW();
	
	void setupInteractions(Window* window);
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void mousePositionCallback(GLFWwindow* window, int xpos, int ypos);
	void scrollCallback(GLFWwindow* window, double xscroll, double yscroll);
	bool shouldClose();
	
	Object*   addObject(std::string file);
	Material* addMaterial(MATERIAL mat);
	Texture*  addTexture(std::string file);
	Scene*    addScene();
	Camera*   addCamera();
};

Application::Application(){
	if(!setupGLFW()){ exit("Failed to initialize GLFW");}
}
void error_callback(int error, const char* description) {
  fputs(description, stderr);
}
int Application::setupGLFW(){
  if (!glfwInit()) {
  	return 0;
  }
	glfwSetErrorCallback(error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return 1;
}
Window* Application::addWindow(int width, int height, std::string title){
	Window* win = new Window(width, height, title);
	if(!setupGLEW()){ exit("Failed to initialize GLEW");}
	setupInteractions(win);
	windows.push_back(win);
	return win;
}
int Application::setupGLEW(){
	glewExperimental = GL_TRUE;
  glewInit();
	return 1;
}
bool Application::shouldClose(){
	for ( auto &w : windows ) {
		if(glfwWindowShouldClose(w->mWindow)){
			return true;
		}
	}
	return false;
}
Window* Application::activeWindow(){
	GLFWwindow* active = glfwGetCurrentContext();
	Window* win = NULL;
	for(Window* w : windows){
		if(w->mWindow == active){
			win = w;
		}		
	}
	return win;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
  app->keyCallback(window, key, scancode, action, mods);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
  Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
  app->mouseButtonCallback(window, button, action, mods);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
  Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
  app->mousePositionCallback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xscroll, double yscroll){
  Application *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
  app->scrollCallback(window, xscroll, yscroll);
}
void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (key == GLFW_KEY_D && action == GLFW_PRESS){
		windows.resize(2);
    std::cout << "Window adress = " << window << std::endl;
		for(Window* w : windows){
			if(window == w->mWindow){
				std::cout << "  Scene adress = " << w->mScene << std::endl;
				for(auto &o : w->mScene->mObjects){
					std::cout << "    Object adress = " << o << std::endl;
				}
			}
		}
  }
}
void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		std::cout << "Pressed the left button!" << std::endl;
}
void Application::mousePositionCallback(GLFWwindow* window, int xpos, int ypos){
	Window *win = activeWindow();
	glm::vec2 depl(xpos - lastX, ypos - lastY);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		win->mScene->mCamera->rotate(-0.01f * depl.y, win->mScene->mCamera->mRight);
		win->mScene->mCamera->rotate(-0.01f * depl.x, win->mScene->mCamera->mUp);
	}
	lastX = xpos;
	lastY = ypos;
}
void Application::scrollCallback(GLFWwindow* window, double xscroll, double yscroll){
	Window *win = activeWindow();
	win->mScene->mCamera->zoom(1-0.01f*yscroll);
}
void Application::setupInteractions(Window* window){
	glfwSetWindowUserPointer(window->mWindow, this);
  glfwSetErrorCallback(error_callback);
  glfwSetKeyCallback(window->mWindow, key_callback);
	glfwSetMouseButtonCallback(window->mWindow, mouse_button_callback);
	glfwSetCursorPosCallback(window->mWindow, cursor_position_callback);
	glfwSetScrollCallback(window->mWindow, scroll_callback);
}

Object*   Application::addObject(std::string file){
	Object *obj = new Object(file);
	objects.push_back(obj);
	return obj;
}
Material* Application::addMaterial(MATERIAL type){
	Material *mat = new Material(type);
	materials.push_back(mat);
	return mat;
}
Texture*  Application::addTexture(std::string file){
	Texture *tex = new Texture(file);
	textures.push_back(tex);
	return tex;
}
Scene*    Application::addScene(){
	Scene* sce = new Scene();
	scenes.push_back(sce);
	return sce;
}
Camera*   Application::addCamera(){
	Camera* cam = new Camera();
	cameras.push_back(cam);
	return cam;
}


/////////////////////////////////////////
// Main Program
/////////////////////////////////////////

int main() {

	Application *app = new Application();

	Window *win = app->addWindow(640, 480, "Test application");

	Object* obj = app->addObject("/Users/guestaccount/3D/Philippe_walter/AurigeDelphesLowRes.ply");
	Material* mat = app->addMaterial(MAT_VERTEX_COLOR);
	Texture* tex = app->addTexture("/Users/guestaccount/texture.png");
	mat->addTexture(tex);
	obj->addMaterial(mat);

	Scene* sce = app->addScene();
	Camera* cam = app->addCamera();
	sce->addCamera(cam);
	sce->addObject(obj);
	win->addScene(sce);
	/*
	Window *win2 = app->addWindow(320, 480, "Test application 2");

	Object* obj2 = app->addObject("/Users/guestaccount/dev/OpenGL/assets/3260.ply");
	Material* mat2 = app->addMaterial(MAT_VERTEX_COLOR);
	Texture* tex2 = app->addTexture("/Users/guestaccount/texture.png");
	mat2->addTexture(tex2);
	obj2->addMaterial(mat2);

	Scene* sce2 = app->addScene();
	Camera* cam2 = app->addCamera();
	sce2->addCamera(cam2);
	sce2->addObject(obj2);
	win2->addScene(sce2);
	*/
	FPS(&chrono);
	
	while(!app->shouldClose()){
		
		FPS(&chrono, &frames);
		
		for(auto &w : app->windows){
			glfwMakeContextCurrent(w->mWindow);
			w->mScene->mCamera->updateProj();
			w->mScene->clear(glm::vec3(0.05,0.05,0.05));
			w->mScene->render();
			glfwSwapBuffers(w->mWindow);
			glFlush();
		}
		
		glfwWaitEvents();
	};

  //glDeleteVertexArrays(1, &(obj->mVao));
  //glDeleteProgram(mat->mId);
	glfwTerminate();
}