#pragma once
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "astyle.h"
#define STDCALL __stdcall

class Formatter
{
public:
	char* GetText(const string& filePath);
	void SetText(const char* textOut, const string& filePathStr);
	void  STDCALL ASErrorHandler(int errorNumber, const char* errorMessage);
	char* STDCALL ASMemoryAlloc(unsigned long memoryNeeded);

	char* FormatText(char* textIn);
};

