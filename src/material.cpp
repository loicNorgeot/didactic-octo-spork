#include "material.h"

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