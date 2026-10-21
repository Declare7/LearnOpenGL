#ifndef SECTIONBASE_H
#define SECTIONBASE_H

#include <string>
#include "ShaderProgram.h"

class GLFWwindow;

class SectionBase
{
public:
    SectionBase(const std::string &type, bool supportInput=false);
    virtual ~SectionBase();

    bool supportInput();

public:
    virtual void prepare()=0;
    virtual void render()=0;
    virtual void processInput(GLFWwindow *window){};
    virtual void processMouse(double msX, double msY){};
    virtual void processWheel(double xOffset, double yOffset){};

protected:
    std::unique_ptr<lngl::ShaderProgram> loadShader(const std::string &type="normal");

private:
    std::string getClassName();

protected:
    std::string m_type{""};
    unsigned int m_VAO{0};
    std::unique_ptr<lngl::ShaderProgram> m_shader;
    bool m_supportInput{false};
};

#endif // SECTIONBASE_H
