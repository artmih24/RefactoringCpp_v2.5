// файл CppClass.h - объявление класса CppClass

#pragma once
#include <cstring>
#include <vector>
#include <set>

#include "Structs.h"
#include "CppClassMethod.h"


/// <summary>Класс класса C++</summary>
class CppClass
{
private:
	std::vector<std::string> possibleTypes;		// возможные типы данных

	std::set<std::string> types;				// типы данных

public:
	std::string name;								// название класса
	std::vector<std::string> tokens;						// токены кода класса
	std::vector<CppClassField> fields;				// поля класса
	std::vector<CppClassMethod> methods;				// методы класса
	std::vector<CppClassConstructor> constructors;	// конструкторы класса
	CppClassDestructor destructor;				// деструкторы класса
	CppClassGraph classGraph;					// граф связности полей, методов, конструкторов и деструкторов класса

	CppClass();
	CppClass(std::vector<std::string> tokens, std::vector<std::string> possibleTypes);

	CppClassGraph GetClassGraph();

	void GetFields();

	void GetMethods();

	void GetConstructors();

	void GetDestructor();

	void CreateConstructor();

	void UpdateClass();

	void UpdateFields();

	void UpdateMethods();

	bool GetFlag(AccessMode accessMode);

	std::vector<std::string> GetTokens(AccessMode accessMode);

	std::vector<std::string> ToTokens();
};

