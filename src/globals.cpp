#include "globals.h"

std::string rootDir = "/Users/guestaccount/dev/simpleGL/";
bool verbose = true;
int frames  = 0;
double chrono = 0;

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

GLuint loadShaders(std::string vertexShader, std::string fragmentShader, std::string functions){
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
	
	//Rajoute le code des fonctions glsl
	if(functions!=""){
	  std::string functionsCode;
	  std::ifstream functionsStream(functions.c_str(), std::ios::in);
	  if(functionsStream.is_open()){
	    std::string Line = "";
	    while(getline(functionsStream, Line))
	      functionsCode += "\n" + Line;
	    functionsStream.close();
	  }
	  else{exit(144);}
		FragmentShaderCode = FragmentShaderCode + "\n" + functionsCode;
	}

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