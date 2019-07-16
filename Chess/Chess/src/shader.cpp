#include "shader.h"

Shader::Shader(const std::string & name, GLuint type) 
{
    m_id = glCreateShader (type);
    m_name = name;
    m_type = type;
    m_filename = "";
    m_source = "";
}

Shader::~Shader()
{
}

void Shader::loadFromFile (const std::string & filename) 
{
    m_filename = filename;
    std::ifstream in (filename.c_str());

    if (!in)
        std::cout << "Error loading shader source file" << std::endl;

    std::string source;
    char c[2];
    c[1] = '\0';
    while (in.get(c[0])) 
	{
        source.append (c);
    }

    in.close ();
    setSource (source);
}

void Shader::compile () 
{
    const GLchar* tmp = m_source.c_str();
    glShaderSource (m_id, 1, &tmp, NULL);
    glCompileShader (m_id);

    GLint shaderCompiled;
    glGetShaderiv (m_id, GL_COMPILE_STATUS, &shaderCompiled);
    if (!shaderCompiled) 
	{
        GLint maxLength = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(m_id, maxLength, &maxLength, &errorLog[0]);

        for (auto c : errorLog)
            std::cout << c;
        std::cout << std::endl;

        std::cout << "Error: shader not compiled." << m_name << std::endl;
    }
}

void Shader::reload () 
{
    if (m_filename != "") 
	{
        loadFromFile (std::string (m_filename));
        compile ();
    }
}
