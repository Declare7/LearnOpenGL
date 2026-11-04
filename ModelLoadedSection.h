#ifndef MODELLOADEDSECTION_H
#define MODELLOADEDSECTION_H

#include "SectionBase.h"
#include "Model.h"

namespace lngl {
class Camera;
}

class ModelLoadedSection : public SectionBase
{
public:
    explicit ModelLoadedSection(const std::string &type="normal");

    void prepare() override;
    void render() override;

private:
    std::unique_ptr<lngl::Model> m_model;
};

#endif // MODELLOADEDSECTION_H
