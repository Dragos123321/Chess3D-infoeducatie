#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define __gl_h_
#else
#include <GL/glew.h>
#endif

#include "shader.h"

class Program 
{
public:
    void init();
    void initForShadowMap();
    void initForSelection();
    void attach (Shader* shader);
    void link ();
    GLuint getId() const { return m_id; }

    void detach(Shader* shader);
    void use();
    void stop();
    void reload();

private:
    GLuint m_id;
    std::string m_name;
    std::vector<Shader*> m_shaders;
};
