#pragma once
#include <string>

/// <summary>�������� ������</summary>
struct Parameter {
    std::string type,               // ��� ��������
        name,                       // �������� ����������
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

/// <summary>����� ������</summary>
struct MethodCall {
    std::string methodName;                     // �������� ������
    std::vector<std::string> parameterValues;   // ������ �������� ����������
    std::vector<bool> isReference;
};