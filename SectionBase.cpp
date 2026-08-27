#include "SectionBase.h"
#include <typeinfo>

#ifdef __GNUG__
#include <cxxabi.h>
#include <memory>
#endif

#include "glad/glad.h"

SectionBase::SectionBase(const std::string &type)
    : m_type(type)
{

}

SectionBase::~SectionBase()
{
    delete m_program;
}

void SectionBase::loadShader(const std::string &type)
{
    std::string path = "shader";
    path = SOURCE_PATH;
    path = path + "/shader/" + getClassName() + "/";

    m_program = new ShaderProgram(path, type);
}

std::string SectionBase::getClassName()
{
#ifdef __GNUG__
    int status;
    std::unique_ptr<char, void(*)(void*)> demangled(
        abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, &status),
        std::free
        );
    return (status == 0) ? demangled.get() : typeid(*this).name();
#elif defined(_MSC_VER)
    std::string name = typeid(*this).name();
    // 移除 "class " 前缀
    if (name.find("class ") == 0) name = name.substr(6);
    return name;
#else
    return typeid(*this).name();
#endif
}

