#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <string>

class GLFWwindow;

class SectionBase
{
public:
    SectionBase(const std::string &type);
    virtual ~SectionBase();

public:
    virtual void prepare()=0;
    virtual void render()=0;
    virtual void processInput(GLFWwindow *window){};
    virtual void processMouse(double msX, double msY){};
    virtual void processWheel(double xOffset, double yOffset){};

protected:
    void loadShader(const std::string &type="normal");

private:
    std::string readVertexShader(const std::string &name);
    std::string readFragmentShader(const std::string &name);
    std::string readShader(const std::string &name, const std::string &suffix);
    std::string getClasName();

protected:
    std::string m_type{""};
    unsigned int m_shaderProgram{0};
    unsigned int m_VAO{0};
};

#endif // SECTIONBASE_H
