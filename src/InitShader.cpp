//////////////////////////////////////////////////////////////////////////////
//  --- Initshader.cpp ---
//   Shader initialization function from Angel 6th Edition
//   - This file reads 2 text files and attempts to compile and link
//     them into a shader program (vertex/fragment shader pair).
//   Modified by Jeremy Carter, Spring 2012
//   - Added comments and changed spacing
//   - !!! changed the fopen parameter from "r" to "rb" 
//     This handles Windows vs. *nix newline   
//////////////////////////////////////////////////////////////////////////////


#include "Angel.h"

namespace Angel {
// Read the contents of the file into a C-style string (NULL-terminated character array)
static char* readShaderSource(const char* shaderFile)
{
    // Open the file for reading
    FILE* fp = fopen(shaderFile, "rb");     // maybe just "r" on *nix environments
    if ( fp == NULL ) { 
        return NULL; 
    }

    // Zip the file pointer to the end of the file to see how big it is, then reset.
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // Allocate a character array to hold the program, and slurp it in.
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);

    // Terminate the array with a null character byte
    buf[size] = '\0';
    fclose(fp);
    return buf;
}


// Create a GLSL program object from vertex and fragment shader files
GLuint initShader(const char* vShaderFile, const char* fShaderFile)
{
    // Function-local class for holding shader information
    struct Shader {
        const char*  filename;
        GLenum       type;
        GLchar*      source;
    }  shaders[2] = {
        { vShaderFile, GL_VERTEX_SHADER, NULL },
        { fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };

    // create a program object and get a UID to it
    GLuint program = glCreateProgram();

    // Read and then compile each shader
    for ( int i = 0; i < 2; ++i ) {

        // Read in and check for parse errors
        Shader& s = shaders[i];
        s.source = readShaderSource( s.filename );
        if ( shaders[i].source == NULL ) {
            std::cerr << "Failed to read " << s.filename << std::endl;
			;
            exit( EXIT_FAILURE );
        }

        // create a shader object and get a UID to it
        GLuint shader = glCreateShader( s.type );

        // Attach the program string to the shader object (on the card)
        glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );

        // Attempt to compile the shader and check for errors
        glCompileShader( shader );
        GLint  compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            std::cerr << s.filename << " failed to compile:" << std::endl;
            GLint  logSize;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
            char* logMsg = new char[logSize];
            glGetShaderInfoLog( shader, logSize, NULL, logMsg );
            std::cerr << logMsg << std::endl;
            delete [] logMsg;
			;
            exit( EXIT_FAILURE );
        }

        // Clean up memory allocated in readShaderSource() 
        delete [] s.source;

        // Plug the shader into the program object
        glAttachShader( program, shader );
    }

    // link program and check for errors
    glLinkProgram(program);
    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
        std::cerr << "Shader program failed to link" << std::endl;
        GLint  logSize;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog( program, logSize, NULL, logMsg );
        std::cerr << logMsg << std::endl;
        delete [] logMsg;
		;
        exit( EXIT_FAILURE );
    }

    // activate the program object
    glUseProgram(program);
    return program;
}

} // end namespace Angel
