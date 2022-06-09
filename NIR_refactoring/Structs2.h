#pragma once
#include <string>

/// <summary>Параметр метода</summary>
struct Parameter {
    std::string type,           // тип значения
                name,           // название переменной
                defaultValue;   // значение по умолчанию
    bool pointer,               // передается ли указатель
         reference;             // передается ли переменая по ссылке

    Parameter(std::string type, std::string name) {
        this->type = type;
        this->name = name;
    }
};

/// <summary>Модификатор доступа</summary>
enum class AccessMode {
    Public,
    Private,
    Protected
};

/// <summary>Конструктор объекта класса</summary>
struct CppClassConstructor {
    std::vector<Parameter> parameters;  // параметры конструктора
    std::vector<std::string> tokens;    // токены кода конструктора
    AccessMode accessMode;              // модификатор доступа
};

/// <summary>Деструктор объекта класса</summary>
struct CppClassDestructor {
    std::vector<std::string> tokens;    // токены кода деструктора
    AccessMode accessMode;              // модификатор доступа
};

/// <summary>Вызов метода</summary>
struct MethodCall {
    std::string methodName;                     // название метода
    std::vector<std::string> parameterValues;   // список значений параметров
    std::vector<bool> isReference;              // передается ли переменная по ссылке
};