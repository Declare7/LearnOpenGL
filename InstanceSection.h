#ifndef INSTANCESECTION_H
#define INSTANCESECTION_H

#include "SectionBase.h"

class InstanceSection : public SectionBase
{
public:
    explicit InstanceSection(const std::string &type="normal");

    virtual void prepare();
    virtual void render();

};

#endif // INSTANCESECTION_H
