#pragma once

#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

const float M_PI = 3.14159265359f;

class Vao
{
public:
	Vao() = default;
    Vao(const Vao& vao, const glm::vec3& color) { m_id = vao.m_id; m_vertexCount = vao.m_vertexCount; m_diffuseColor = color; m_textureEnabled = vao.m_textureEnabled; }
    GLuint getId() const { return m_id; }
    GLsizei getVertexCount() const { return m_vertexCount; }
    
    const GLfloat* getModelMatrixArray() const { return glm::value_ptr(m_modelMatrix); }
    const GLfloat* getDiffuseColorArray() const { return glm::value_ptr(m_diffuseColor); }
    const glm::mat4& getModelMatrix() const { return m_modelMatrix; }
    
    static Vao getCube();
    // Creates the cube encompassing the scene on which the "sky" will be painted
    static Vao getSkyBoxCube();
    
    static Vao loadObj(std::string filename, glm::vec3 color);
    static Vao loadObj(std::string filename, glm::vec3 color, std::string texture_filename);

    
    void translate(const glm::vec3& vector);
    void rotate(float angle, const glm::vec3 &vector);
    void scale(const glm::vec3 &vector);
    
    bool isTextureEnabled() const { return m_textureEnabled; }
    GLuint getTextureId() const { return m_textureID; }
    
    void requestMovement(glm::vec3 pos_end);
    void requestJumpMovement(glm::vec3 pos_end);
    void requestEjectMovement();
    inline bool isMovementRequested() const { return m_movementRequested; }
    inline bool isJumpMovementRequested() const { return m_jumpMovementRequested; }
    inline bool isEjectMovementRequested() const { return m_ejectMovementRequested; }
    inline const glm::vec3& getMovementDirection() const { return m_movementDirection; }
    inline double getMovementStartTime() const { return m_movementStartTime; }
    inline float getMovementLength() const { return m_movementLength; }
    void updateMovement();
    void endMovement();
    
    inline bool isRotated90() { return m_rotated; }
    
    void rotate90() { rotate(M_PI, glm::vec3(0.0f, 1.0f, 0.0f));  m_rotated = true;}

private:
    GLuint m_id;
    GLuint m_textureID = 0;
    GLsizei m_vertexCount;
    glm::mat4 m_modelMatrix;
    glm::vec3 m_diffuseColor;
    GLboolean m_textureEnabled;
    
    bool m_movementRequested = false;
    bool m_jumpMovementRequested = false;
    bool m_ejectMovementRequested = false;
    float a, b; // Variables useful for jump calculation
    float m_movementLength;
    glm::vec3 m_positionStart, m_positionEnd, m_movementDirection;
    double m_movementStartTime;
    glm::mat4 m_modelMatrixBeforeMovement;
    bool m_rotated = false;
};
