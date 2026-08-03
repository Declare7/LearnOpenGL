#ifndef RECTANGLESECTION_H
#define RECTANGLESECTION_H

#include "SectionBase.h"

class RectangleSection : public SectionBase
{
public:
    RectangleSection(const std::string &type="normal");

    void prepare() override;
    void render() override;

private:
    void prepareEBO();
};

#endif // RECTANGLESECTION_H
