#include "program.h"

using namespace std;

void Program::init()
{
    const string & name = "Program";
    const string & vertexShaderFilename = "shaders/shader.vert";
    const string & fragmentShaderFilename = "shaders/shader.frag";

    m_id = glCreateProgram();
    m_name = name;
    Shader * vs = new Shader(name + " Vertex Shader", GL_VERTEX_SHADER);
    Shader * fs = new Shader(name + " Fragment Shader",GL_FRAGMENT_SHADER);
    vs->loadFromFile(vertexShaderFilename);
    vs->compile();
    attach(vs);
    fs->loadFromFile(fragmentShaderFilename);
    fs->compile();
    attach(fs);

    glBindAttribLocation(m_id, 0, "vertex");
    glBindAttribLocation(m_id, 1, "normal");
    glBindAttribLocation(m_id, 2, "texture");

    link();
}

void Program::initForShadowMap()
{
    
    const string & name = "ProgramShadowMap";
    const string & vertexShaderFilename = "shaders/shader_SM.vert";
    const string & fragmentShaderFilename = "shaders/shader_SM.frag";
    
    m_id = glCreateProgram();
    m_name = name;
    Shader * vs = new Shader(name + " Vertex Shader", GL_VERTEX_SHADER);
    Shader * fs = new Shader(name + " Fragment Shader",GL_FRAGMENT_SHADER);
    vs->loadFromFile(vertexShaderFilename);
    vs->compile();
    attach(vs);
    fs->loadFromFile(fragmentShaderFilename);
    fs->compile();
    attach(fs);
    
    glBindAttribLocation(m_id, 0, "vertex");

    link();
}

void Program::initForSelection()
{
    const string & name = "ProgramSelection";
    const string & vertexShaderFilename = "shaders/selection_shader.vert";
    const string & fragmentShaderFilename = "shaders/selection_shader.frag";

    m_id = glCreateProgram();
    m_name = name;
    Shader * vs = new Shader(name + " Vertex Shader", GL_VERTEX_SHADER);
    Shader * fs = new Shader(name + " Fragment Shader",GL_FRAGMENT_SHADER);
    vs->loadFromFile(vertexShaderFilename);
    vs->compile();
    attach(vs);
    fs->loadFromFile(fragmentShaderFilename);
    fs->compile();
    attach(fs);

    glBindAttribLocation(m_id, 0, "vertex");
    glBindAttribLocation(m_id, 2, "texture");

    link();
}

void Program::attach(Shader * shader) 
{
    glAttachShader (m_id, shader->getId());
    m_shaders.push_back (shader);
}

void Program::detach(Shader * shader) 
{
    for (unsigned int i = 0; i < m_shaders.size (); i++)
        if (m_shaders[i]->getId() == shader->getId())
            glDetachShader (m_id, shader->getId());
}

void Program::link() {
    glLinkProgram (m_id);
    GLint linked;
    glGetProgramiv (m_id, GL_LINK_STATUS, &linked);
    if (!linked) {
        cout << "Shaders not linked" << endl;

        GLint maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(m_id, maxLength, &maxLength, &errorLog[0]);

        for (auto c : errorLog)
            cout << c;
        cout << endl;
    }
}

void Program::use() {
    glUseProgram(m_id);
}

void Program::stop() {
    glUseProgram (0);
}

void Program::reload() {
    for (unsigned int i = 0; i < m_shaders.size (); i++) 
	{
        m_shaders[i]->reload ();
        attach (m_shaders[i]);
    }

    link();
}


