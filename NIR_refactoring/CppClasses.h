#pragma once
#include "CppClass.h"

/// <summary>Классы</summary>
struct CppClasses {
    CppClass oldClassWOExtractedMethods;    // новый класс вместо исходного
    CppClass newClassWExtractedMethods;     // новый класс
    std::vector<std::string> newTokens;     // новые токены кода

    CppClasses(CppClass oldClass, CppClass newClass, std::vector<std::string> tokens) {
        this->oldClassWOExtractedMethods = oldClass;
        this->newClassWExtractedMethods = newClass;
        this->newTokens = tokens;
    }
};