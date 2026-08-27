#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <string>
#include "ShaderProgram.h"

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
    ShaderProgram * loadShader(const std::string &type="normal");

private:
    std::string getClassName();

protected:
    std::string m_type{""};
    unsigned int m_VAO{0};
    ShaderProgram *m_program{nullptr};
};

#endif // SECTIONBASE_H
