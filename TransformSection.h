#ifndef TRANSFORMSECTION_H
#define TRANSFORMSECTION_H

#include "RectangleSection.h"

class TransformSection : public RectangleSection
{
public:
    TransformSection(const std::string &type="normal");

    virtual void prepare() override;
    virtual void render() override;
};

#endif // TRANSFORMSECTION_H
