#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define __gl_h_
#else
#include <GL/glew.h>
#endif

#include "camera.h"
#include "vao.h"
#include "light.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <map>


const float ZNEAR = 10.0f;
const float ZFAR = 10000.0f;
const float FOV = 0.785f;

class Scene
{
public:
	Scene() = default;
    void initScene(int width, int height);

    size_t size() const { return m_vaoList.size(); }

    Vao& operator[](size_t index) { return m_vaoList[index]; }
    int addVaoPiece(std::string model, int team, glm::vec3 pos);
    
    std::vector<int> addVaoPieces(std::vector<std::string> model, std::vector<int> team, std::vector<glm::vec3> pos);

    void slideVAOTo(int vao, glm::vec3 newPos);
    void jumpVAOTo(int vao, glm::vec3 newPos);
    void ejectVAO(int vao);

    void deleteVAO(int vao);

    inline Light& getLight(int index) { return m_lights[index]; }
    size_t getLightCount() const { return m_lights.size(); }
    
    void setPerspective(int width, int height);
    inline GLfloat* getProjectionMatrixArray() { return glm::value_ptr(m_projectionMatrix); }
    inline const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
    inline const glm::mat4& getShadowProjectionMatrix() const { return m_shadowProjectionMatrix; }
    inline const glm::mat4& getBiasMatrix() const { return m_biasMatrix; }

    inline int getShadowSize() const { return m_shadowSize; }

    inline const GLuint& getSkyBox(){ return m_skyBox;}
    GLuint getTexCube();
    void setSelectTex(int value) { m_selectTex = value; }
    const int getSelectTex() { return m_selectTex; }

    // CAMERA
    void setView();
    GLfloat *getNormalMatrixArray(unsigned int vao_index);
    inline GLfloat* getViewMatrixArray() { return glm::value_ptr(m_viewMatrix); }
    
    inline const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
    
    Camera getCamera() { return m_camera; }
    void setCamFW(bool fw) { m_camera.setFW(fw); }
    void setCamBW(bool bw) { m_camera.setBW(bw); }
    void setCamLS(bool ls) { m_camera.setLS(ls); }
    void setCamRS(bool rs) { m_camera.setRS(rs); }
    void setCamZP(bool zp) { m_camera.setZP(zp); }
    void setCamZN(bool zn) { m_camera.setZN(zn); }
    void move(int);
    
    void selectModel(int index);
    void unselect();
    inline int getSelected() const { return m_selectedModel; }
    inline bool selected() const { return m_vaoSelection; }
    inline GLfloat* getSelectectionColor() { return glm::value_ptr(m_selecionColor); }
    
private:
    void initShadow(int light_index);
    void initModels();
    void initLights();
    
    // Skybox's creation
    // Create the cube as a vbo
    void initSkyBox();
	// Create the faces of the SkyBox
    GLuint loadCubemap(std::vector<const GLchar*> faces);

    std::vector<Vao> m_vaoList;
    glm::mat4 m_viewMatrix, m_projectionMatrix, m_normalMatrix;
    Camera m_camera;
    glm::mat4 m_shadowProjectionMatrix, m_biasMatrix;
    int m_shadowSize;
    std::vector<Light> m_lights;
    
    int m_selectedModel;
    glm::vec3 m_selecionColor;
    bool m_vaoSelection = false;
    
    
    std::map<std::string, Vao> m_loadedModeles;

    GLuint m_skyBox;
    GLuint m_texCube;
    GLuint m_texCubeYellow;
    GLuint m_texCubeRed;
    int m_selectTex;
};
