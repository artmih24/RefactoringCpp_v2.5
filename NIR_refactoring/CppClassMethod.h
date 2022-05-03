#pragma once

#include <cstring>
#include <vector>

#include "Structs.h"

class CppClassMethod {
private:
    std::vector<std::string> lexemes;         // лексемы метода

    //CppClass cppClass;

public:
    std::vector<Parameter> parameters,   // параметры метода
        oldParameters;              // старые параметры метода

    std::vector<std::string> body,            // лексемы тела метода
        usingFields;

    std::vector<CppClassMethod> usingMethods;
    std::vector<CppClassConstructor> usingConstructors;
    CppClassDestructor usingDestructor;

    std::string type,                    // тип возвращаемого значения
        name,                       // название метода
        className;

    AccessMode accessMode;

    /// <summary>Конструктор объекта класса CppMethod</summary>
    CppClassMethod();

    /// <summary>Конструктор объекта класса CppMethod</summary>
    /// <param name="lexemes">- список лексем</param>
    CppClassMethod(vector<string> lexemes, string className, vector<string> classCode, vector<CppClassField> fields, vector<CppClassMethod> methods, vector<CppClassConstructor> constructors);
    //CppClassMethod(vector<string> lexemes, string className);

    /// <summary>Получение списка параметров метода</summary>
    /// <returns>Список параметров метода</returns>
    std::vector<Parameter> GetMethodParameters();

    /// <summary>Получение лексем тела метода</summary>
    /// <returns>Список лексем тела метода</returns>
    std::vector<std::string> GetMethodBody();

    std::vector<std::string> GetUsingFields(std::vector<CppClassField> fields);

    std::vector<CppClassMethod> GetUsingMethods(std::vector<CppClassMethod> methods);

    std::vector<CppClassConstructor> GetUsingConstructors(std::vector<CppClassConstructor> constructors);

    CppClassDestructor GetUsingDestructor();

    AccessMode GetAccessMode(std::vector<std::string> classCode);

    std::string GetClassName();

    /// <summary>Получения типа возвращаемого значения метода</summary>
    /// <returns>Тип возвращаемого значения</returns>
    std::string GetMethodType();

    /// <summary>Получение названия метода</summary>
    /// <returns>Название метода</returns>
    std::string GetMethodName();
    std::string GetMethodNameV2();

    /// <summary>Получение списока лексем</summary>
    /// <returns>Список лексем</returns>
    std::vector<std::string> ToLexemes();
};

