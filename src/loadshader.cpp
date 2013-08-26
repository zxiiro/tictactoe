/**********************************************************************

    Copyright (c) 2013 Thanh Ha

    This file is part of sdl-tictactoe.

    sdl-tictactoe is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sdl-tictactoe is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sdl-tictactoe.  If not, see <http://www.gnu.org/licenses/>.

**********************************************************************/

#include "loadshader.h"

/**
 * Load shaders and create a program object out of it.
 *
 * @param vertex_file_path path to where the vertex shader is located.
 * @param fragment_file_path path to where the fragment shader is located.
 * @param geometry_file_path path to where the geometry shader is located.
 *
 * @return returns a program object.
 */
GLuint Program::Load(
        const char *vertex_file_path,
        const char *fragment_file_path,
        const char *geometry_file_path)
{
    GLuint program = glCreateProgram();

    // Parse vertex shader
    if (vertex_file_path != NULL) {
        std::string shader_source;
        std::ifstream shader_stream(vertex_file_path);

        if (shader_stream.is_open()) {
            std::string line = "";

            while(getline(shader_stream, line))
                shader_source += "\n" + line;

            shader_stream.close();
        } else {
            printf("Error: Unable to read Vertex Shader file.\n");
        }

        AttachShader(program, GL_VERTEX_SHADER, shader_source.c_str());
    }

    // Parse fragment shader
    if (fragment_file_path != NULL) {
        std::string shader_source;
        std::ifstream shader_stream(fragment_file_path);

        if (shader_stream.is_open()) {
            std::string line = "";

            while(getline(shader_stream, line))
                shader_source += "\n" + line;

            shader_stream.close();
        } else {
            printf("Error: Unable to read Vertex Shader file.\n");
        }

        AttachShader(program, GL_FRAGMENT_SHADER, shader_source.c_str());
    }

    // Parse geometry shader
    if (geometry_file_path != NULL) {
        std::string shader_source;
        std::ifstream shader_stream(geometry_file_path);

        if (shader_stream.is_open()) {
            std::string line = "";

            while(getline(shader_stream, line))
                shader_source += "\n" + line;

            shader_stream.close();
        } else {
            printf("Error: Unable to read Vertex Shader file.\n");
        }

        AttachShader(program, GL_GEOMETRY_SHADER, shader_source.c_str());
    }

    glLinkProgram(program);
    CheckStatus(program);
    return program;
}

/**
 * Attaches shader to current program.
 *
 * @param program the program object that you want to attach the shader to.
 * @param type the type of shader it is, vertex, fragment or geometry.
 * @param src the shader's contents.
 *
 */
void Program::AttachShader(GLuint program, GLenum type, const char *src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    CheckStatus(shader);

    glAttachShader(program, shader);
    glDeleteShader(shader);
}

/**
 * Checks the status of the shader or program, whether it has an error or not.
 *
 * @param obj a shader or program may be passed in to check its status.
 *
 */
void Program::CheckStatus(GLuint obj) {
    GLint status = GL_FALSE;
    int length = 0;

    if (glIsShader(obj)) {
        glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
    } else if (glIsProgram(obj)) {
        glGetProgramiv(obj, GL_LINK_STATUS, &status);
    }

    if (status == GL_TRUE)
        return;

    if (glIsShader(obj)) {
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
    } else if (glIsProgram(obj)) {
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
    }

    std::vector<char> log(length);
    if (glIsShader(obj)) {
        glGetShaderInfoLog(obj, length, NULL, &log.front());
    } else if (glIsProgram(obj)) {
        glGetProgramInfoLog(obj, length, NULL, &log.front());
    }

    printf("%s", &log.front());
}
