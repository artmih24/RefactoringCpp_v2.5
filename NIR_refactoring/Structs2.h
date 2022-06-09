#pragma once
#include <string>

/// <summary>�������� ������</summary>
struct Parameter {
    std::string type,           // ��� ��������
                name,           // �������� ����������
                defaultValue;   // �������� �� ���������
    bool pointer,               // ���������� �� ���������
         reference;             // ���������� �� ��������� �� ������

    Parameter(std::string type, std::string name) {
        this->type = type;
        this->name = name;
    }
};

/// <summary>����������� �������</summary>
enum class AccessMode {
    Public,
    Private,
    Protected
};

/// <summary>����������� ������� ������</summary>
struct CppClassConstructor {
    std::vector<Parameter> parameters;  // ��������� ������������
    std::vector<std::string> tokens;    // ������ ���� ������������
    AccessMode accessMode;              // ����������� �������
};

/// <summary>���������� ������� ������</summary>
struct CppClassDestructor {
    std::vector<std::string> tokens;    // ������ ���� �����������
    AccessMode accessMode;              // ����������� �������
};

/// <summary>����� ������</summary>
struct MethodCall {
    std::string methodName;                     // �������� ������
    std::vector<std::string> parameterValues;   // ������ �������� ����������
    std::vector<bool> isReference;              // ���������� �� ���������� �� ������
};