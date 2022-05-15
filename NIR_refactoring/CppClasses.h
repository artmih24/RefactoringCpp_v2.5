#pragma once
#include "CppClass.h"

/// <summary>������</summary>
struct CppClasses {
    CppClass oldClassWOExtractedMethods;    // ����� ����� ������ ���������
    CppClass newClassWExtractedMethods;     // ����� �����
    std::vector<std::string> newTokens;     // ����� ������ ����

    CppClasses(CppClass oldClass, CppClass newClass, std::vector<std::string> tokens) {
        this->oldClassWOExtractedMethods = oldClass;
        this->newClassWExtractedMethods = newClass;
        this->newTokens = tokens;
    }
};