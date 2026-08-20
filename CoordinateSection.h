#ifndef COORDINATESECTION_H
#define COORDINATESECTION_H

#include "RectangleSection.h"

class CoordinateSection : public RectangleSection
{
public:
    CoordinateSection(const std::string &type="normal");

    void prepare() override;
    void render() override;

protected:
    void prepare3D();
    void render3D();
};

#endif // COORDINATESECTION_H
