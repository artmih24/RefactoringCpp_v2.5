// ���� CppClass.h - ���������� ������ CppClass

#pragma once
#include <cstring>
#include <vector>
#include <set>

#include "Structs.h"
#include "CppClassMethod.h"


/// <summary>����� ������ C++</summary>
class CppClass
{
private:
	vector<string> possibleTypes;	// ��������� ���� ������

	set<string> types;				// ���� ������

public:
	string name;								// �������� ������
	vector<string> tokens;						// ������ ���� ������
	vector<CppClassField> fields;				// ���� ������
	vector<CppClassMethod> methods;				// ������ ������
	vector<CppClassConstructor> constructors;	// ������������ ������
	CppClassDestructor destructor;				// ����������� ������
	CppClassGraph classGraph;					// ���� ��������� �����, �������, ������������� � ������������ ������

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

