#ifndef TRIANGLESECTION_H
#define TRIANGLESECTION_H

#include "SectionBase.h"

class TriangleSection : public SectionBase
{
public:
    TriangleSection(const std::string &type="normal");

    virtual void prepare() override;
    virtual void render() override;

private:
    void renderUniform();
    void prepareColors();
};

#endif // TRIANGLESECTION_H
