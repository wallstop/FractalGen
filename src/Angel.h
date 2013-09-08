//////////////////////////////////////////////////////////////////////////////
//  --- Angel.h ---
//   The main header file for all examples from Angel 6th Edition
//   - This file includes all the necessary OpenGL headers 
//   - Defines a couple of helpful macros and constants
//   - Forward-declares function for loading shader files
//   Modified by Jeremy Carter, Spring 2012
//   - Added comments
//   - Changed include guards to pragma once
//   - Removed automatic includes of vec.h, mat.h, and CheckError.h
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>
#include <iostream>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

// --- Include OpenGL header files and helpers (OS-specific paths)---
#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

//  --- Utility functions, macros,  and constants ---

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// Print to cerr, e.g., ErrPrint(myVar);  --->  "myVar = 2"
#define ErrPrint(x)  do { std::cerr << #x " = " << (x) << std::endl; } while(0)

namespace Angel {

    //  Helper function to load vertex and fragment shader files
    GLuint initShader( const char* vertexShaderFile,
                       const char* fragmentShaderFile );

    //  Defined constant for when numbers are too small to be used in the
    //    denominator of a division operation.  
    //  Note: in vec.h and mat.h, this is only used in debug mode
    const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);

    //  Degrees-to-radians constant 
    const GLfloat  DegreesToRadians = GLfloat(M_PI / 180.0);

}  // namespace Angel




