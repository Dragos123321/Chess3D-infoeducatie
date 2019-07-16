#include "scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <map>
#include <iostream>

#include "stb_image.h"

static const glm::vec3 color1(1.0f, 1.0f, 1.0f);
static const glm::vec3 color2(0.2f, 0.2f, 0.2f);

void Scene::selectModel(int index) 
{
    m_selectedModel = index;
    m_vaoSelection = true;
}

void Scene::unselect() {
	m_vaoSelection = false;
}

void Scene::initScene(int width, int height)
{
    m_selectTex = 0;

    initLights();

    initModels();

    for (unsigned int i = 0; i < m_lights.size(); ++i)
        initShadow(i);
    
    m_selecionColor = glm::vec3(1.0, 1.0, 0.0);

    m_projectionMatrix = glm::mat4(1.0f);
    m_camera = Camera(static_cast<float>(width), static_cast<float>(height));

    initSkyBox();
}

void Scene::initLights()
{
    m_lights.push_back(Light(glm::vec3(400.f, 400.f, 400.f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f)));
    m_lights.push_back(Light(glm::vec3(-400.f, 400.f, -400.f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f)));
}

void Scene::initShadow(int light_index)
{
    m_shadowSize = 4096;
    
    // shadow map
    Light &current_light = m_lights[light_index];
    
    glGenTextures(1, &current_light.getShadowTextureId());
    glBindTexture(GL_TEXTURE_2D, current_light.getShadowTextureId());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowSize, m_shadowSize, 0, GL_DEPTH_COMPONENT , GL_FLOAT , NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D , 0);

    // shadow frame buffer
    glGenFramebuffers(1, &current_light.getShadowBufferId());
    glBindFramebuffer(GL_FRAMEBUFFER , current_light.getShadowBufferId());

    glFramebufferTexture2D(GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_TEXTURE_2D , current_light.getShadowTextureId(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
        printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
    
    glBindFramebuffer(GL_FRAMEBUFFER , 0);
    
    m_shadowProjectionMatrix = glm::perspective<float>(1.4f, 1.f, 20.f, 10000.f);
    
    m_biasMatrix = glm::mat4(
					0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0
					);
}

void Scene::initModels()
{
    glm::mat4 rotation_180(1.0f);
    rotation_180 = glm::rotate(rotation_180, (float)M_PI, glm::vec3(0.0f, 1.0f, 0.0f));
    
    m_loadedModeles["models/bishop.obj"] = Vao::loadObj("models/fou.obj", glm::vec3(1.0));
    m_loadedModeles["models/king.obj"] = Vao::loadObj("models/roi.obj", glm::vec3(1.0));
    m_loadedModeles["models/knight.obj"] = Vao::loadObj("models/cavalier.obj", glm::vec3(1.0));
    m_loadedModeles["models/pawn.obj"] = Vao::loadObj("models/pion.obj", glm::vec3(1.0));
    m_loadedModeles["models/queen.obj"] = Vao::loadObj("models/dame.obj", glm::vec3(1.0));
    m_loadedModeles["models/rook.obj"] = Vao::loadObj("models/tour.obj", glm::vec3(1.0));

    Vao plateau = Vao::loadObj("models/plane.obj", glm::vec3(0.f, 0.f, 0.f), "textures/board.tga");
    
    m_vaoList.push_back(plateau);
}

void Scene::setPerspective(int width, int height)
{
    m_projectionMatrix = glm::mat4(1.0f);
	m_projectionMatrix = glm::perspective(FOV, (float)width/height, ZNEAR, ZFAR);
}

void Scene::setView() {
    m_viewMatrix = glm::mat4 (1.0f);
	m_viewMatrix = glm::lookAt(m_camera.getPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Scene::move(int fps) 
{
    m_camera.move(fps);
}

GLfloat* Scene::getNormalMatrixArray(unsigned int vao_index)
{
    const glm::mat4 &model_matrix = m_vaoList[vao_index].getModelMatrix();

    m_normalMatrix = glm::transpose(glm::inverse(m_viewMatrix * model_matrix));

    return glm::value_ptr(m_normalMatrix);
}

void Scene::initSkyBox()
{
	float points[] = {
		-4000.0f,  4000.0f, -4000.0f,
		-4000.0f, -4000.0f, -4000.0f,
		4000.0f, -4000.0f, -4000.0f,
		4000.0f, -4000.0f, -4000.0f,
		4000.0f,  4000.0f, -4000.0f,
		-4000.0f,  4000.0f, -4000.0f,

		-4000.0f, -4000.0f,  4000.0f,
		-4000.0f, -4000.0f, -4000.0f,
		-4000.0f,  4000.0f, -4000.0f,
		-4000.0f,  4000.0f, -4000.0f,
		-4000.0f,  4000.0f,  4000.0f,
		-4000.0f, -4000.0f,  4000.0f,

		4000.0f, -4000.0f, -4000.0f,
		4000.0f, -4000.0f,  4000.0f,
		4000.0f,  4000.0f,  4000.0f,
		4000.0f,  4000.0f,  4000.0f,
		4000.0f,  4000.0f, -4000.0f,
		4000.0f, -4000.0f, -4000.0f,

		-4000.0f, -4000.0f,  4000.0f,
		-4000.0f,  4000.0f,  4000.0f,
		4000.0f,  4000.0f,  4000.0f,
		4000.0f,  4000.0f,  4000.0f,
		4000.0f, -4000.0f,  4000.0f,
		-4000.0f, -4000.0f,  4000.0f,

		-4000.0f,  4000.0f, -4000.0f,
		4000.0f,  4000.0f, -4000.0f,
		4000.0f,  4000.0f,  4000.0f,
		4000.0f,  4000.0f,  4000.0f,
		-4000.0f,  4000.0f,  4000.0f,
		-4000.0f,  4000.0f, -4000.0f,

		-4000.0f, -4000.0f, -4000.0f,
		-4000.0f, -4000.0f,  4000.0f,
		4000.0f, -4000.0f, -4000.0f,
		4000.0f, -4000.0f, -4000.0f,
		-4000.0f, -4000.0f,  4000.0f,
		4000.0f, -4000.0f,  4000.0f
	};

	glGenVertexArrays(1, &m_skyBox);
	glBindVertexArray(m_skyBox);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), points, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	std::string textName = "sky";
	std::string path = "textures/" + textName + "_";
	std::string right = path + "right.jpg";
	std::string left = path + "left.jpg";
	std::string top = path + "top.jpg";
	std::string bottom = path + "bottom.jpg";
	std::string back = path + "back.jpg";
	std::string front = path + "front.jpg";
	std::vector<const GLchar*> faces;
	faces.push_back(right.c_str());
	faces.push_back(left.c_str());
	faces.push_back(top.c_str());
	faces.push_back(bottom.c_str());
	faces.push_back(back.c_str());
	faces.push_back(front.c_str());
	m_texCube = loadCubemap(faces);


	textName = "yellow_sky";
	path = "textures/" + textName + "_";
	right = path + "right.jpg";
	left = path + "left.jpg";
	top = path + "top.jpg";
	bottom = path + "bottom.jpg";
	back = path + "back.jpg";
	front = path + "front.jpg";
	std::vector<const GLchar*> facesY;
	facesY.push_back(right.c_str());
	facesY.push_back(left.c_str());
	facesY.push_back(top.c_str());
	facesY.push_back(bottom.c_str());
	facesY.push_back(back.c_str());
	facesY.push_back(front.c_str());
	m_texCubeYellow = loadCubemap(facesY);


	textName = "red_sky";
	path = "textures/" + textName + "_";
	right = path + "right.jpg";
	left = path + "left.jpg";
	top = path + "top.jpg";
	bottom = path + "bottom.jpg";
	back = path + "back.jpg";
	front = path + "front.jpg";
	std::vector<const GLchar*> facesR;
	facesR.push_back(right.c_str());
	facesR.push_back(left.c_str());
	facesR.push_back(top.c_str());
	facesR.push_back(bottom.c_str());
	facesR.push_back(back.c_str());
	facesR.push_back(front.c_str());
	m_texCubeRed = loadCubemap(facesR);
}

GLuint Scene::loadCubemap(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE5);

    int width,height,n;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    image = stbi_load (faces[0], &width, &height, &n, 4);
    if (!image) { fprintf (stderr, "ERROR: could not load %s\n", faces[0]); }
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    image = stbi_load (faces[1], &width, &height, &n, 4);
    if (!image) { fprintf (stderr, "ERROR: could not load %s\n", faces[1]); }
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    image = stbi_load (faces[2], &width, &height, &n, 4);
    if (!image) { fprintf (stderr, "ERROR: could not load %s\n", faces[2]); }
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    image = stbi_load (faces[3], &width, &height, &n, 4);
    if (!image) { fprintf (stderr, "ERROR: could not load %s\n", faces[3]); }
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    image = stbi_load (faces[4], &width, &height, &n, 4);
    if (!image) { fprintf (stderr, "ERROR: could not load %s\n", faces[4]); }
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    image = stbi_load (faces[5], &width, &height, &n, 4);
    if (!image) { fprintf (stderr, "ERROR: could not load %s\n", faces[5]); }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

int Scene::addVaoPiece(std::string model, int team, glm::vec3 pos){

    // Create a vao, add it to the vao list and return its index in the list

    Vao piece ;
    if(team == 1) 
	{
        piece = Vao::loadObj(model, color1);
    }
	
	else
	{
        piece = Vao::loadObj(model, color2);
    }

    piece.translate(pos);
    m_vaoList.push_back(piece);
    return ((int)m_vaoList.size());

}

std::vector<int> Scene::addVaoPieces(std::vector<std::string> model, std::vector<int> team, std::vector<glm::vec3> pos){

    // Create a vao list without having to reload the same model multiple times

    Vao plateau = m_vaoList[0];
    m_vaoList.clear();
	m_vaoList.push_back(plateau);
    
    std::map<std::string, int> loadedModeles;
    std::vector<int> indices;

    for (int i = 0; i < static_cast<int>(model.size()); i++)
	{
        Vao piece;
        std::map<std::string, int>::iterator it = loadedModeles.find(model[i]);
        if(it ==loadedModeles.end())
		{
            if(team[i] == 1) 
			{
                piece = Vao::loadObj(model[i], color1);
            }
			
			else
			{
                piece = Vao::loadObj(model[i], color2);
            }
            loadedModeles[model[i]] = (int)m_vaoList.size();
        }
		else
		{
            if(team[i] == 1) 
			{
                piece = Vao(m_vaoList[it->second], color1);
            }
			
			else
			{
                piece = Vao(m_vaoList[it->second], color2);
            }
        }
        
        piece.translate(pos[i]);
		m_vaoList.push_back(piece);
        indices.push_back(static_cast<int>(m_vaoList.size()));
        
        if (team[i] == 2 && (model[i] == "models/cavalier.obj" || model[i] == "models/fou.obj"))
            m_vaoList[i + 1].rotate90();
    }

    return indices;
}


void Scene::slideVAOTo(int vao, glm::vec3 newPos){
    m_vaoList[vao].requestMovement(newPos);
}

void Scene::jumpVAOTo(int vao, glm::vec3 newPos){
	m_vaoList[vao].requestJumpMovement(newPos);
}

void Scene::ejectVAO(int vao) {
	m_vaoList[vao].requestEjectMovement();
}

void Scene::deleteVAO(int vao) 
{
	m_vaoList.erase(m_vaoList.begin() + vao);
}

GLuint Scene::getTexCube() 
{
     if (m_selectTex == 0) 
	 {
          return m_texCube;
     } 
	 
	 else if (m_selectTex == 1) 
	 {
          return m_texCubeYellow;
     } 
	 
	 else if (m_selectTex == 2)
	 {
          return m_texCubeRed;
     }

	 return m_texCubeRed;
}
