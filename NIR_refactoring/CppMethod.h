// ���� CppMethod.h - ���������� ������ CppMethod

#pragma once

#include <cstring>
#include <vector>

#include "Structs2.h"

/// <summary>����� ������</summary>
class CppMethod {
private:
    std::vector<std::string> tokens;    // ������ ������

public:
    std::vector<Parameter> parameters,       // ��������� ������
                      oldParameters;    // ������ ��������� ������

    std::vector<std::string> body;      // ������ ���� ������

    std::string type,                   // ��� ������������� ��������
                name;                   // �������� ������

    /// <summary>����������� ������� ������ CppMethod</summary>
    CppMethod();

    /// <summary>����������� ������� ������ CppMethod</summary>
    /// <param name="lexemes">- ������ �������</param>
    CppMethod(std::vector<std::string> tokens);

    /// <summary>��������� ������ ���������� ������</summary>
    /// <returns>������ ���������� ������</returns>
    std::vector<Parameter> GetMethodParameters();

    /// <summary>��������� ������ ���� ������</summary>
    /// <returns>������ ������ ���� ������</returns>
    std::vector<std::string> GetMethodBody();

    /// <summary>��������� ���� ������������� �������� ������</summary>
    /// <returns>��� ������������� ��������</returns>
    std::string GetMethodType();

    /// <summary>��������� �������� ������</summary>
    /// <returns>�������� ������</returns>
    std::string GetMethodName();

    std::vector<std::string> GetTokens();

    /// <summary>��������� ������� ������</summary>
    /// <returns>������ ������</returns>
    std::vector<std::string> ToTokens();
};
