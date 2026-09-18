#ifndef TRIANGLESECTION_H
#define TRIANGLESECTION_H

#include "SectionBase.h"

class TriangleSection : public SectionBase
{
public:
    TriangleSection(const std::string &type="normal");
    virtual ~TriangleSection();

    virtual void prepare() override;
    virtual void render() override;

private:
    void renderUniform();
    void prepareColors();
    void prepareDoubleProgram();
    void prepareUniformCustom();

private:
    unsigned int m_VAO2{0};
    std::shared_ptr<ShaderProgram> m_shader2;
};

#endif // TRIANGLESECTION_H
