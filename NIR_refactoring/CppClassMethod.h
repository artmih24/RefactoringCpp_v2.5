#pragma once

#include <cstring>
#include <vector>

#include "Structs.h"

class CppClassMethod {
private:
    std::vector<std::string> tokens;        // ������ ������

public:
    std::vector<Parameter> parameters,      // ��������� ������
                           oldParameters;   // ������ ��������� ������

    std::vector<std::string> body,          // ������� ���� ������
                             usingFields;

    std::vector<CppClassMethod> usingMethods;
    std::vector<CppClassConstructor> usingConstructors;
    CppClassDestructor usingDestructor;

    std::string type,                       // ��� ������������� ��������
                name,                       // �������� ������
                className;

    AccessMode accessMode;

    /// <summary>����������� ������� ������ CppMethod</summary>
    CppClassMethod();

    /// <summary>����������� ������� ������ CppMethod</summary>
    /// <param name="lexemes">- ������ ������</param>
    CppClassMethod(std::vector<std::string> methodTokens, 
                   std::string className, 
                   std::vector<std::string> tokens, 
                   std::vector<CppClassField> fields, 
                   std::vector<CppClassMethod> methods, 
                   std::vector<CppClassConstructor> constructors);
    //CppClassMethod(vector<string> lexemes, string className);

    /// <summary>��������� ������ ���������� ������</summary>
    /// <returns>������ ���������� ������</returns>
    std::vector<Parameter> GetMethodParameters();

    /// <summary>��������� ������ ���� ������</summary>
    /// <returns>������ ������ ���� ������</returns>
    std::vector<std::string> GetMethodBody();

    std::vector<std::string> GetUsingFields(std::vector<CppClassField> fields);

    std::vector<CppClassMethod> GetUsingMethods(std::vector<CppClassMethod> methods);

    std::vector<CppClassConstructor> GetUsingConstructors(std::vector<CppClassConstructor> constructors);

    CppClassDestructor GetUsingDestructor();

    AccessMode GetAccessMode(std::vector<std::string> tokens);

    std::string GetClassName();

    /// <summary>��������� ���� ������������� �������� ������</summary>
    /// <returns>��� ������������� ��������</returns>
    std::string GetMethodType();

    /// <summary>��������� �������� ������</summary>
    /// <returns>�������� ������</returns>
    std::string GetMethodName();
    std::string GetMethodNameV2();

    /// <summary>��������� ������� ������</summary>
    /// <returns>������ ������</returns>
    std::vector<std::string> ToTokens();
};

