// файл CppClass.h - объ€вление класса CppClass

#pragma once
#include <cstring>
#include <vector>
#include <set>

#include "Structs.h"
#include "CppClassMethod.h"


class CppClass
{
private:
	vector<string> possibleTypes;

	set<string> types;

public:
	string name;
	vector<string> classCode;
	vector<CppClassField> fields;
	vector<CppClassMethod> methods;
	vector<CppClassConstructor> constructors;
	CppClassDestructor destructor;
	CppClassGraph classGraph;

	CppClass();
	CppClass(vector<string> lexemes, vector<string> possibleTypes);

	CppClassGraph GetClassGraph();

	void GetFields();

	void GetMethods();

	void GetConstructors();

	void GetDestructor();

	void UpdateClass();

	void UpdateFields();

	void UpdateMethods();

	bool GetFlag(AccessMode accessMode);

	vector<string> GetLexemes(AccessMode accessMode);

	vector<string> ToLexemes();
};

