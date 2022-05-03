// файл CppMethod.h - объявление класса CppMethod

#pragma once

#include <cstring>
#include <vector>
using namespace std;

#include "Structs2.h"

/// <summary>Класс метода</summary>
class CppMethod {
private:
    vector<string> lexemes;         // лексемы метода

public:
    vector<Parameter> parameters,   // параметры метода
        oldParameters;              // старые параметры метода

    vector<string> body;            // лексемы тела метода

    string type,                    // тип возвращаемого значения
        name;                       // название метода

    /// <summary>Конструктор объекта класса CppMethod</summary>
    CppMethod();

    /// <summary>Конструктор объекта класса CppMethod</summary>
    /// <param name="lexemes">- список лексем</param>
    CppMethod(vector<string> lexemes);

    /// <summary>Получение списка параметров метода</summary>
    /// <returns>Список параметров метода</returns>
    vector<Parameter> GetMethodParameters();

    /// <summary>Получение лексем тела метода</summary>
    /// <returns>Список лексем тела метода</returns>
    vector<string> GetMethodBody();

    /// <summary>Получения типа возвращаемого значения метода</summary>
    /// <returns>Тип возвращаемого значения</returns>
    string GetMethodType();

    /// <summary>Получение названия метода</summary>
    /// <returns>Название метода</returns>
    string GetMethodName();

    vector<string> GetLexemes();

    /// <summary>Получение списока лексем</summary>
    /// <returns>Список лексем</returns>
    vector<string> ToLexemes();
};
