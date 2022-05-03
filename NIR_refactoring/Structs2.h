#pragma once
#include <string>

/// <summary>ѕараметр метода</summary>
struct Parameter {
    std::string type,               // тип значени€
        name,                       // название переменной
        defaultValue;
    bool pointer,
        reference;
};

enum class AccessMode {
    Public,
    Private,
    Protected,
    Null
};

struct CppClassConstructor {
    std::vector<Parameter> parameters;
    std::vector<std::string> constructorCode;
    AccessMode accessMode;
};

struct CppClassDestructor {
    std::vector<std::string> destructorCode;
    AccessMode accessMode;
};

/// <summary>¬ызов метода</summary>
struct MethodCall {
    std::string methodName;                     // название метода
    std::vector<std::string> parameterValues;   // список значений параметров
    std::vector<bool> isReference;
};