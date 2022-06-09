// файл CppCode.h - объявление класса CppCode

#pragma once
#include <string>
#include <vector>
#include <set>
#include <clocale>
#include <sstream>
#include <regex>

#include "CppMethod.h"
#include "CppClass.h"
#include "CppClassMethod.h"
#include "Structs.h"

/// <summary>Класс кода</summary>
class CppCode {
private:
    std::string cppCode;                        // строка, содержащая код

public:
    std::vector<std::string> tokens,            // список токенов кода
                             types,             // список типов переменных в коде
                             keyWordsSet,       // список ключевых слов
                             specSymbolsSet;    // список спецсимволов

    std::vector<CppMethod> methods;             // список методов кода

    std::vector<CppClass> classes;              // список классов в коде

    /// <summary>Конструктор по умолчанию</summary>
    CppCode();

    /// <summary>Конструктор класса</summary>
    CppCode(std::string cppCode);

    /// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
    std::string Modify();

    std::string ModifyV2();

    /// <summary>Токенизация кода</summary>
    std::vector<std::string> GetTokens();

    /// <summary>Токенизация кода</summary>
    std::vector<std::string> GetTokensV2();

    /// <summary>Получение типов данных, описанных в коде</summary>
    /// <param name="type">- способ объявления пользовательского типа</param>
    /// <returns>вектор пользовательских типов данных</returns>
    std::vector<std::string> GetTypes(std::string type);

    /// <summary>Получение типов данных, описанных в коде</summary>
    /// <returns>вектор пользовательских типов данных</returns>
    std::vector<std::string> GetStdTypes();

    /// <summary>Получение вектора методов</summary>
    /// <returns>вектор методов в коде</returns>
    std::vector<CppMethod> GetMethods();

    /// <summary>Полечение списка вызовов метода</summary>
    /// <param name="cppMethod">- метод, вызовы которого требуется получить</param>
    /// <returns>список вызовов методов</returns>
    std::vector<MethodCall> GetMethodCalls(CppMethod cppMethod);

    /// <summary>Получение списка классов в коде</summary>
    /// <returns>список классов в коде</returns>
    std::vector<CppClass> GetClasses();

    /// <summary>Обновление частей списка токенов, содержащих вызовы методов</summary>
    /// <param name="cppMethod">- метод, вызовы которого нужно обновить</param>
    void UpdateMethodCalls(CppMethod cppMethod);

    /// <summary>Обновление частей списка токенов, содержащих вызовы методов</summary>
    /// <param name="cppMethod">- метод, вызовы которого нужно обновить</param>
    /// <param name="methodCall">- вызов метода, который нужно обновить</param>
    /// <param name="newMethodCall">- новый вызов метода</param>
    void UpdateMethodCalls(CppMethod cppMethod, MethodCall methodCall, MethodCall newMethodCall);

    /// <summary>Обновление частей списка токенов, содержащих вызовы методов</summary>
    /// <param name="cppMethod">- метод, вызовы которого нужно обновить</param>
    /// <param name="methodCall">- вызов метода, который нужно обновить</param>
    /// <param name="newMethodCall">- новый вызов метода</param>
    void UpdateMethodCallsV2(CppMethod cppMethod, MethodCall methodCall, MethodCall newMethodCall);

    /// <summary>Обновление методов</summary>
    /// <param name="methods">- список методов</param>
    void UpdateMethods(std::vector<CppMethod> methods);

    /// <summary>Обновление списка токенов</summary>
    void UpdateTokens();

    /// <summary>Обновление списка токенов</summary>
    void UpdateTokensV2();

    void UpdateTokensV3();

    /// <summary>Обновление строки, содержащей код</summary>
    /// <param name="code">- строка, содержащая код</param>
    void UpdateCode(std::string code);

    /// <summary>Получение строки, содержащей код</summary>
    /// <returns>Строка, содержащая код</returns>
    std::string ToString();

    /// <summary>Получение строки, содержащей код</summary>
    /// <returns>Строка, содержащая код</returns>
    std::string GetCode();

    /// <summary>Получение разделителя в коде</summary>
    /// <param name="curLexeme">- текущая лексема кода</param>
    /// <param name="nxtLexeme">- следующая лексема кода</param>
    /// <returns>Разделитель в коде</returns>
    std::string GetSep(std::string curLexeme, std::string nxtLexeme);
};