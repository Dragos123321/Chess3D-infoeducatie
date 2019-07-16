#include "vao.h"
#include "tga.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

static float const jumpHigh = 150.0f;
static float const speed = 1.0f;

void Vao::endMovement()
{
    m_movementRequested = false;
}

void Vao::updateMovement()
{
    double elaspedTime = glfwGetTime() - m_movementStartTime;

    float movementLength = static_cast<float>(elaspedTime) * speed * getMovementLength();
    glm::vec3 translation = movementLength * m_movementDirection;
    float y;

    if(m_jumpMovementRequested || m_ejectMovementRequested)
	{
        y = a * pow(movementLength, 2.0f) + b * movementLength;
        translation += glm::vec3(0.0f, y, 0.0f);
    }

    if(m_movementLength <= getMovementLength() || m_ejectMovementRequested)
    {
        m_modelMatrix = glm::translate(m_modelMatrixBeforeMovement, translation);
        if (m_rotated)
            rotate90();
    }
    
    if (elaspedTime >= 1.0f / speed && !m_ejectMovementRequested)
    {
        m_movementRequested = false;
        m_jumpMovementRequested = false;
        
        m_modelMatrix = glm::mat4(1.0f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_positionEnd);
        
        if (m_rotated)
            rotate90();
    }

    if (elaspedTime >= 10.0f / speed && m_ejectMovementRequested) 
	{
        m_ejectMovementRequested = false;
    }
}

void Vao::requestMovement(glm::vec3 pos_end)
{
    m_positionStart = glm::vec3(m_modelMatrix * glm::vec4(0, 0, 0, 1));

    m_modelMatrixBeforeMovement = glm::mat4(1.f);
	m_modelMatrixBeforeMovement = glm::translate(m_modelMatrixBeforeMovement, m_positionStart);

    m_movementRequested = true;
    m_positionEnd = pos_end;
    m_movementLength = glm::length(m_positionEnd - m_positionStart);
    m_movementDirection = m_positionEnd - m_positionStart;
	m_movementDirection = glm::normalize(m_movementDirection);
    m_movementStartTime = glfwGetTime();
}

void Vao::requestJumpMovement(glm::vec3 pos_end)
{    
    m_positionStart = glm::vec3(m_modelMatrix * glm::vec4(0, 0, 0, 1));
    
    m_modelMatrixBeforeMovement = glm::mat4(1.0f);
	m_modelMatrixBeforeMovement = glm::translate(m_modelMatrixBeforeMovement, m_positionStart);
    
    m_jumpMovementRequested = true;
    m_positionEnd = pos_end;
    m_movementLength = glm::length(m_positionEnd - m_positionStart);
    m_movementDirection = m_positionEnd - m_positionStart;
    m_movementDirection = glm::normalize(m_movementDirection);
    m_movementStartTime = glfwGetTime();
    float alpha = getMovementLength();
    b =  4 * jumpHigh / alpha; 
	a = -4 * jumpHigh / static_cast<float>(powf(alpha, 2.0f));
}

void Vao::requestEjectMovement() {
    m_positionStart = glm::vec3(m_modelMatrix * glm::vec4(0, 0, 0, 1));

    m_modelMatrixBeforeMovement = glm::mat4(1.0f);
	m_modelMatrixBeforeMovement = glm::translate(m_modelMatrixBeforeMovement, m_positionStart);

    m_ejectMovementRequested = true;
    m_positionEnd = glm::vec3(500.0f, 0.0f, 0.0f);
    m_movementLength = glm::length(m_positionEnd - m_positionStart);
    m_movementDirection = m_positionEnd - m_positionStart;
	m_movementDirection = glm::normalize(m_movementDirection);
    m_movementStartTime = glfwGetTime();
    float alpha = getMovementLength();
    b =  4 * jumpHigh / alpha; 
	a = -4 * jumpHigh / static_cast<float>(powf(alpha, 2.0f));
}

Vao Vao::loadObj(std::string filename, glm::vec3 color)
{

    return Vao::loadObj(filename, color, "");
}

Vao Vao::loadObj(std::string filename, glm::vec3 color, std::string texture_filename)
{

	std::cout << "Loading..." << std::endl;
    Vao vao;
    
    vao.m_diffuseColor = color;

    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> textures;

    std::vector<unsigned int> vertex_indices, normal_indices, texture_indices;

    std::ifstream inputfile(filename);

    std::string str;

    while (std::getline(inputfile, str))
    {
        if (str.size() > 2)
        {
            std::istringstream iss(str);
            std::string token;
            float vec[3];

            if (str[0] == 'v' && str[1] == ' ')
            {
                getline(iss, token, ' ');

                for (int i = 0; i < 3; ++i) {
                    getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }

                vertices.push_back(glm::vec3(vec[0], vec[1], vec[2]));
            }
            else if (str[0] == 'v' && str[1] == 'n')
            {
                getline(iss, token, ' ');

                for (int i = 0; i < 3; ++i) {
                    getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }

                normals.push_back(glm::vec3(vec[0], vec[1], vec[2]));
            }
            else if (str[0] == 'v' && str[1] == 't')
            {
                getline(iss, token, ' ');
                
                for (int i = 0; i < 2; ++i) {
                    getline(iss, token, ' ');
                    vec[i] = std::stof(token);
                }
                
                textures.push_back(glm::vec2(vec[0], vec[1]));
            }
            else if (str[0] == 'f' && str[1] == ' ')
            {
                getline(iss, token, ' ');

                for (int i = 0; i < 3; ++i) {
                    getline(iss, token, ' ');
                    size_t index;

                    if ((index = token.find("//")) != std::string::npos)
                    {
                        std::string a = token.substr(0, index);
                        std::string b = token.substr(index + 2, token.size() - index);

                        vertex_indices.push_back(std::stoi(a) - 1);
                        normal_indices.push_back(std::stoi(b) - 1);
                    }
                    else
                    {
                        /* correct */
                        index = token.find("/");
                        size_t index2 = token.find("/", index + 1);
                        std::string a = token.substr(0, index);
                        std::string b = token.substr(index + 1, index2 - index - 1);
                        std::string c = token.substr(index2 + 1, token.size() - index2 + 1);
                        

                        vertex_indices.push_back(std::stoi(a) - 1);
                        texture_indices.push_back(std::stoi(b) - 1);
                        normal_indices.push_back(std::stoi(c) - 1);

                    }
                }

            }
        }
    }

    std::vector<glm::vec3> vertices_new, normals_new;
    std::vector<glm::vec2> textures_new;
    
    vao.m_textureEnabled = textures.size() > 0 && texture_indices.size() > 0 && texture_filename.size() > 0;


    for (unsigned int i = 0; i < vertex_indices.size(); ++i)
    {
        vertices_new.push_back(vertices[vertex_indices[i]]);
        normals_new.push_back(normals[normal_indices[i]]);
        
        if (vao.m_textureEnabled)
            textures_new.push_back(textures[texture_indices[i]]);
    }

    vao.m_vertexCount = (GLuint)vertices_new.size();
    vao.m_modelMatrix = glm::mat4(1.f);

    glGenVertexArrays(1, &vao.m_id);
    glBindVertexArray(vao.m_id);

    GLuint vbo_positions;
    glGenBuffers(1, &vbo_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, vao.m_vertexCount * sizeof(glm::vec3), &vertices_new[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint vbo_normals;
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, vao.m_vertexCount * sizeof(glm::vec3), &normals_new[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    if (vao.m_textureEnabled)
    {
        glGenTextures(1, &vao.m_textureID);
        glBindTexture(GL_TEXTURE_2D, vao.m_textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        

        Tga tga = Tga::LoadTGAFile(texture_filename.c_str());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.imageWidth, tga.imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tga.imageData);

        
        GLuint vbo_textures;
        glGenBuffers(1, &vbo_textures);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_textures);
        glBufferData(GL_ARRAY_BUFFER, vao.m_vertexCount * sizeof(glm::vec2), &textures_new[0], GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);
        
    }

    glBindVertexArray(0);

    return vao;
}

void Vao::translate(const glm::vec3 &vector)
{
    m_modelMatrix = glm::translate(m_modelMatrix, vector);
}

void Vao::rotate(float angle, const glm::vec3 &vector)
{
	m_modelMatrix = glm::rotate(m_modelMatrix, angle, vector);
}

void Vao::scale(const glm::vec3 &vector)
{
	m_modelMatrix = glm::scale(m_modelMatrix, vector);
}

Vao Vao::getSkyBoxCube(){
    float points[] = {
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,

        -1000.0f, -1000.0f,  1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
        -1000.0f, -1000.0f,  1000.0f,

        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,

        -1000.0f, -1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f, -1000.0f,  1000.0f,
        -1000.0f, -1000.0f,  1000.0f,

        -1000.0f,  1000.0f, -1000.0f,
        1000.0f,  1000.0f, -1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f, -1000.0f,

        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f,  1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f,  1000.0f,
        1000.0f, -1000.0f,  1000.0f
    };
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

    GLuint vaoID = 0;
    Vao vao;
    vao.m_id = vaoID;
    vao.m_vertexCount = 6 * 2 * 3;
    vao.m_modelMatrix = glm::mat4(1.0f);
    vao.m_diffuseColor = glm::vec3(0.0f, 0.0f, 1.0f);

    glGenVertexArrays (1, &vaoID);
    glBindVertexArray (vaoID);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    return vao;
}

Vao Vao::getCube()
{
    Vao vao;

    vao.m_vertexCount = 6 * 2 * 3;
    vao.m_modelMatrix = glm::mat4(1.0f);

    float vertexPositions[] = {
        -0.5f,  0.5f,  -0.5f,
        0.5f, -0.5f,  -0.5f,
        -0.5f, -0.5f,  -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,

        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        -0.5f,  0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    float vertexColors[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,

        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &vao.m_id);
    glBindVertexArray(vao.m_id);

    GLuint vbo_positions;
    glGenBuffers(1, &vbo_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, 12 * 9 * sizeof(float), vertexPositions, GL_STATIC_DRAW);

    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint vbo_colors;
    glGenBuffers(1, &vbo_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, 12 * 9 * sizeof(float), vertexColors, GL_STATIC_DRAW);

    glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return vao;
}



