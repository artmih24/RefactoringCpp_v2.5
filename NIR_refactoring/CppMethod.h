// файл CppMethod.h - объявление класса CppMethod

#pragma once

#include <cstring>
#include <vector>

#include "Structs2.h"

/// <summary>Класс метода</summary>
class CppMethod {
private:
    std::vector<std::string> tokens;    // токены метода

public:
    std::vector<Parameter> parameters,       // параметры метода
                      oldParameters;    // старые параметры метода

    std::vector<std::string> body;      // токены тела метода

    std::string type,                   // тип возвращаемого значения
                name;                   // название метода

    /// <summary>Конструктор объекта класса CppMethod</summary>
    CppMethod();

    /// <summary>Конструктор объекта класса CppMethod</summary>
    /// <param name="lexemes">- список токенов</param>
    CppMethod(std::vector<std::string> tokens);

    /// <summary>Получение списка параметров метода</summary>
    /// <returns>Список параметров метода</returns>
    std::vector<Parameter> GetMethodParameters();

    /// <summary>Получение лексем тела метода</summary>
    /// <returns>Список лексем тела метода</returns>
    std::vector<std::string> GetMethodBody();

    /// <summary>Получения типа возвращаемого значения метода</summary>
    /// <returns>Тип возвращаемого значения</returns>
    std::string GetMethodType();

    /// <summary>Получение названия метода</summary>
    /// <returns>Название метода</returns>
    std::string GetMethodName();

    std::vector<std::string> GetTokens();

    /// <summary>Получение списока лексем</summary>
    /// <returns>Список лексем</returns>
    std::vector<std::string> ToTokens();
};
