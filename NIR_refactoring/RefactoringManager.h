// ���� RefactoringManager.h - ���������� ������ RefactoringManager

#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <regex>
using namespace std;

#include "CppCode.h"
#include "CppClasses.h"

/// <summary>�����, ��������������� ��� ���������� ������������</summary>
class RefactoringManager {
private:
    string filePath,            // ������ ���� � ��������� �����
        outFilePath,            // ������ ���� � ������ �����
        fileContent;            // ������, ���������� ���

    ifstream fileStream;        // ����� ������ �� �����

    ofstream outFileStream;     // ����� ������ � ����

    stringstream stringStream,  // ����� ��� ����������� ����������
        outStringStream;        // ����� ��� ���������� ������

    CppCode code;               // ��� �� �/�++, ����������� �������� ��������� ��������

public:
    /// <summary>����������� ������� ������ RefactoringManager</summary>
    /// <param name="filePath">- ���� � ����� � �������� �����</param>
    RefactoringManager(string filePath);

    /// <summary>���������� ������������</summary>
    void Refactoring();

    /// <summary>���������� ������������ ������� �������� ����������� ����������</summary>
    /// <param name="code">- �������� ��� ���������</param>
    void RemoveAssignmentsToParametersProc(CppCode code);

    /// <summary>�������� ������������ ���������� ������</summary>
    /// <param name="method">- ����������� �����</param>
    /// <returns>����� �����</returns>
    CppMethod RemoveAssignmentsToParameters(CppMethod method);

    /// <summary>���������� ������������ ������� �������� ����������</summary>
    /// <param name="code">- �������� ��� ���������</param>
    void RemoveParametersProc(CppCode code);

    /// <summary>�������� ���������� ������</summary>
    /// <param name="method">- ����������� �����</param>
    /// <returns>����� �����</returns>
    CppMethod RemoveParameters(CppMethod method);

    void SendFullObjectProc(CppCode code);

    MethodSendFullObject SendFullObject(CppMethod method, Object object, MethodCall methodCall);

    void ExtractClassProc(CppCode code);

    CppClasses ExtractClass(CppClass cppClass, vector<int> methodsNums);

    /// <summary>����������� �������� �����, � ������� ����� ����������� ���������� ���</summary>
    /// <param name="filePath">- ������ �������� ��������� �����</param>
    /// <returns>�������� �����, � ������� ����� ����������� ���������� ���</returns>
    string GetOutputFileName(string filePath);

    /// <summary>������ �����</summary>
    /// <param name="filePath">- ������ �������� �����</param>
    void GetFileContent(string filePath);

    /// <summary>������ � ����</summary>
    /// <param name="filePath">- ������ �������� �����</param>
    void WriteFile(string filePath);

    void FormatFile(string filePath);
};