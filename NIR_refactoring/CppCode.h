// файл CppCode.h - объявление класса CppCode

#pragma once
#include <string>
#include <vector>
#include <clocale>
#include <sstream>
#include <regex>
using namespace std;

#include "CppMethod.h"
#include "CppClass.h"
#include "CppClassMethod.h"
#include "Structs.h"

/// <summary>Класс кода</summary>
class CppCode {
private:
    string cppCode;                 // строка, содержащая код

public:
    vector<string> lexemes;         // список лексем кода

    vector<CppMethod> methods;      // список методов кода

    vector<CppClass> classes;

    vector<string> types;           // список типов переменных в коде

    vector<string> keyWordsSet;     // список ключевых слов

    vector<string> specSymbolsSet;  // список спецсимволов

    /// <summary>Конструктор по умолчанию</summary>
    CppCode();

    /// <summary>Конструктор класса</summary>
    CppCode(string cppCode);

    /// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
    string Modify();

    /// <summary>Разбиение кода на лексемы</summary>
    vector<string> GetLexemes();
    /// <summary>Разбиение кода на лексемы</summary>
    vector<string> GetLexemesV2();

    /// <summary>Получение типов данных, описанных в коде</summary>
    /// <param name="type">- способ объявления пользовательского типа</param>
    /// <returns>вектор пользовательских типов данных</returns>
    vector<string> GetTypes(string type);

    /// <summary>Получение типов данных, описанных в коде</summary>
    /// <returns>вектор пользовательских типов данных</returns>
    vector<string> GetStdTypes();

    /// <summary>Получение вектора методов</summary>
    /// <returns>вектор методов в коде</returns>
    vector<CppMethod> GetMethods();

    vector<MethodCall> GetMethodCalls(CppMethod cppMethod);

    vector<CppClass> GetClasses();

    /// <summary>Обновление частей списка лексем, содержащих вызовы методов</summary>
    /// <param name="cppMethod">- метод, вызовы которого нужно обновить</param>
    void UpdateMethodCalls(CppMethod cppMethod);

    void UpdateMethodCalls(CppMethod cppMethod, MethodCall methodCall, MethodCall newMethodCall);
    void UpdateMethodCallsV2(CppMethod cppMethod, MethodCall methodCall, MethodCall newMethodCall);

    /// <summary>Обновление методов</summary>
    /// <param name="methods">- список методов</param>
    void UpdateMethods(vector<CppMethod> methods);

    /// <summary>Обновление списка лексем</summary>
    void UpdateLexemes();
    void UpdateLexemesV2();

    /// <summary>Обновление строки, содержащей код</summary>
    /// <param name="code">- строка, содержащая код</param>
    void UpdateCode(string code);

    /// <summary>Получение строки, содержащей код</summary>
    /// <returns>Строка, содержащая код</returns>
    string ToString();

    string GetCode();

    /// <summary>Получение разделителя в коде</summary>
    /// <param name="curLexeme">- текущая лексема кода</param>
    /// <param name="nxtLexeme">- следующая лексема кода</param>
    /// <returns>Разделитель в коде</returns>
    string GetSep(string curLexeme, string nxtLexeme);
};