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
	vector<string> possibleTypes;	// возможные типы данных

	set<string> types;				// типы данных

public:
	string name;								// название класса
	vector<string> tokens;						// токены кода класса
	vector<CppClassField> fields;				// поля класса
	vector<CppClassMethod> methods;				// методы класса
	vector<CppClassConstructor> constructors;	// конструкторы класса
	CppClassDestructor destructor;				// деструкторы класса
	CppClassGraph classGraph;					// граф связности полей, методов, конструкторов и деструкторов класса

	CppClass();
	CppClass(vector<string> tokens, vector<string> possibleTypes);

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

	vector<string> GetTokens(AccessMode accessMode);

	vector<string> ToTokens();
};

