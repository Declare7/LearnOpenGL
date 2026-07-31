#ifndef TRIANGLESECTION_H
#define TRIANGLESECTION_H

#include "SectionBase.h"

class TriangleSection : public SectionBase
{
public:
    TriangleSection();

    void prepare(const std::string &type="normal") override;
    void render() override;
};

#endif // TRIANGLESECTION_H
