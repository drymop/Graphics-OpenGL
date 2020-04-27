#include "CompileShaders.h"

// STL
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

string
parseShader(const string& _shader) {
  ifstream ifs(_shader);
  ostringstream oss;
  oss << ifs.rdbuf();
  return oss.str();
}

GLuint
compileSingleShader(const string& shaderFile, GLenum shaderType, char* infoLog) {
  string shaderFromFile = parseShader(shaderFile);
  const char* prog = shaderFromFile.c_str();

  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &prog, NULL);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    cerr << "Error compiling shader " << shaderFile
      << "'\n" << infoLog << endl;
    exit(1);
  }
  return shader;
}

GLuint
compileProgram(const string& _vertexShader,
               const string& _tessEvalShader,
               const string& _fragmentShader) {
  int success;
  char infoLog[512];

  // Compile each single shader
  GLuint vertexShader = compileSingleShader(
      _vertexShader, GL_VERTEX_SHADER, infoLog);
  // GLuint tessEvalShader = compileSingleShader(
  //     _tessEvalShader, GL_TESS_EVALUATION_SHADER, infoLog);
  GLuint fragmentShader = compileSingleShader(
      _fragmentShader, GL_FRAGMENT_SHADER, infoLog);

  // Link the shaders into a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  // glAttachShader(shaderProgram, tessEvalShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    cerr << "Error linking programs '" << _vertexShader
      << "' and '" << _fragmentShader << "'\n" << infoLog << endl;
    exit(1);
  }

  glDeleteShader(vertexShader);
  // glDeleteShader(tessEvalShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
