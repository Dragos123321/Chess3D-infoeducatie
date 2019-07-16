#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define __gl_h_
#else
#include <GL/glew.h>
#endif

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Shader 
{
public:
    Shader (const std::string& name, GLuint type);
    ~Shader();

    void loadFromFile(const std::string &);
    void setSource (const std::string & source) { m_source = source; }

    GLuint getId() { return m_id; }
    void compile ();
    void reload();
private:
    GLuint m_id;
    std::string m_name;
    GLuint m_type;
    std::string m_filename;
    std::string m_source;
};
