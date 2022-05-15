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
	std::vector<std::string> possibleTypes;		// ��������� ���� ������

	std::set<std::string> types;				// ���� ������

public:
	std::string name;								// �������� ������
	std::vector<std::string> tokens;						// ������ ���� ������
	std::vector<CppClassField> fields;				// ���� ������
	std::vector<CppClassMethod> methods;				// ������ ������
	std::vector<CppClassConstructor> constructors;	// ������������ ������
	CppClassDestructor destructor;				// ����������� ������
	CppClassGraph classGraph;					// ���� ��������� �����, �������, ������������� � ������������ ������

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

