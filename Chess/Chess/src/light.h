#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define __gl_h_
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
	class Light
	brief Class that represents a light (position, diffuse and specular color)
*/

class Light
{
public:
    // Empty constructor that creates a default light.
    Light();
    Light(glm::vec3 pos, glm::vec3 dcolor, glm::vec3 scolor) : m_lightPos(pos), m_diffuseColor(dcolor), m_specColor(scolor) { m_viewMatrix = glm::lookAt(pos, glm::vec3(0,0,0), glm::vec3(0,1,0)); }

    inline const glm::vec3& getPos() const{ return m_lightPos;}
    inline const glm::vec3& getDiffuseColor() const{ return m_diffuseColor;}
    inline const glm::vec3& getSpecColor() const{ return m_specColor;}

    inline void setPos(glm::vec3 pos) { m_lightPos = pos; }
    inline void setDiffuseColor(glm::vec3 c) { m_diffuseColor = c; }
    inline void setSpecColor(glm::vec3 c) { m_specColor= c; }
    
    inline const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
    
    inline GLuint& getShadowTextureId() { return m_shadowTexture; }
    inline GLuint& getShadowBufferId() { return m_shadowBuffer; }

private:
    glm::vec3 m_lightPos;
    glm::vec3 m_diffuseColor;
    glm::vec3 m_specColor;
    
    glm::mat4 m_viewMatrix;
    
    GLuint m_shadowTexture = 0, m_shadowBuffer = 0;
};
