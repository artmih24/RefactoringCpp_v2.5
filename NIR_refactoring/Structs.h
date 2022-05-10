// файл Structs.h - объявление структур

#pragma once
#include <string>
#include <map>

#include "CppMethod.h"
#include "Structs2.h"

struct CppClassField {
    std::string type,                    // тип значения
        name,                       // название переменной
        value;
    AccessMode accessMode;
};

struct CppClassGraph {
    std::vector<std::vector<bool>> cppClassGraph;
    int totalSize,
        fieldsSize,
        methodsSize,
        constructorsSize,
        destructorsSize;
};

struct ObjectFieldParameter {
    std::string objectName,
        className;
    CppClassField classField;
};

struct ClassMethodCallParameter {
    std::string objectName,
        className;
    MethodCall methodCall;
};

struct Object {
    std::string name,
        className;
    std::vector<ObjectFieldParameter> objectFields;
};

struct MethodSendFullObject {
    CppMethod cppMethod;
    MethodCall methodCall;
};