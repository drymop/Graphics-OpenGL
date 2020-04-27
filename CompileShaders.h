////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Utilities for compiling shaders
////////////////////////////////////////////////////////////////////////////////
#ifndef COMPILE_SHADERS_H_
#define COMPILE_SHADERS_H_

// STL
#include <string>

// GL
#include "GLInclude.h"


////////////////////////////////////////////////////////////////////////////////
/// @brief Compile a vertex shader, tessellation eval shader, and fragment 
/// shader together
/// @param _vertexShader   Filename of vertex shader
/// @param _tessShader     Filename of fragment shader
/// @param _fragmentShader Filename of fragment shader
/// @return GL program identifier
GLuint compileProgram(const std::string& _vertexShader,
                      const std::string& _tessEvalShader,
                      const std::string& _fragmentShader);


#endif // COMPILE_SHADERS_H_
