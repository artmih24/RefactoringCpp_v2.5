// файл CppCode.cpp - реализация методов класса CppCode

#include "CppCode.h"

CppCode::CppCode() {
    this->cppCode = "";
    this->tokens = {};
    this->methods = {};
    this->types = {};
    this->keyWordsSet = {};
    this->specSymbolsSet = {};
    this->classes = {};
}

/// <summary>Конструктор класса</summary>
CppCode::CppCode(std::string cppCode) {
    this->cppCode = cppCode;
    this->tokens = this->GetTokens();
    this->methods = this->GetMethods();
    this->types = this->GetStdTypes();
    this->classes = this->GetClasses();
    this->keyWordsSet = {
        "#include", "include", "export", "module",
        "#define", "#undef",
        "#ifdef", "ifndef", "#if", "#else", "#endif", "#elifdef", "#elifndef", "#elif",
        "#pragma",
        "using",
        "namespace",
        "struct", "union", "enum", "class", "typedef",
        "template", "typename",
        "static", "volatile", "const", "extern", 
        "virtual", "friend",
        "private", "public", "protected",
        "if", "else",
        "while", "do",
        "for", 
        "switch", "case", "default",
        "try", "catch", "throw",
        "break", "continue", "return",
        "this", "new", "delete",
        "asm", "_asm", "__asm"
    };
    this->specSymbolsSet = {
        "<", ">", "\"", "\'", ".", " ", "\n", "\t", ";", ":", ",", 
        "(", ")", "[", "]", "&", "|", "!", "?", "/", "\\", 
        "-", "+", "*", "&", "%", "=", "~"
    };
}

std::vector<std::string> CppCode::GetStdTypes() {
    return {
        "bool",
        "char", "__int8", "int8_t",
        "signed char",
        "unsigned char", "unsigned __int8", "uint8_t",
        "wchar_t", "__wchar_t",
        "char16_t",
        "char32_t",
        "short", "short int", "signed short int", "signed short", "__int16", "int16_t",
        "unsigned short", "unsigned short int", "unsigned __int16", "uint16_t",
        "int", "signed int", "signed", "__int32", "int32_t",
        "unsigned int", "unsigned", "unsigned __int32", "uint32_t",
        "long", "long int", "signed long int", "signed long",
        "unsigned long", "unsigned long int",
        "long long", "long long int", "signed long long int", "signed long long", "__int64", "int64_t",
        "unsigned long long", "unsigned long long int", "unsigned __int64", "uint64_t",
        "float",
        "double",
        "long double",
        "size_t", "uintptr_t",
        "ptrdiff_t", "intptr_t",
        "byte",
        "string",
        "void",
        "auto"
    };
}

/// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
std::string CppCode::Modify() {
    std::string modCode = this->cppCode;
    for (int i = 0; i < modCode.length(); i++) {
        if (((modCode[i + 1] == '(') || (modCode[i + 1] == ')') ||
            (modCode[i + 1] == '{') || (modCode[i + 1] == '}') ||
            (modCode[i + 1] == ';') ||
            (modCode[i + 1] == ',') ||
            (modCode[i + 1] == '=') || (modCode[i + 1] == '+') || (modCode[i + 1] == '-') ||
            (modCode[i + 1] == '*') || (modCode[i + 1] == '/') || (modCode[i + 1] == '%') ||
            (modCode[i + 1] == '&') || (modCode[i + 1] == '|') || (modCode[i + 1] == '^')) &&
            (i < (modCode.length() - 1)))
            modCode.insert(++i, " ");
        if (((modCode[i] == '(') || (modCode[i] == ')') ||
            (modCode[i] == '{') || (modCode[i] == '}') ||
            (modCode[i] == ';') || (modCode[i] == ':') ||
            (modCode[i] == ',') ||
            (modCode[i] == '=') || (modCode[i] == '+') || (modCode[i] == '-') ||
            (modCode[i] == '*') || (modCode[i] == '/') || (modCode[i] == '%') ||
            (modCode[i] == '&') || (modCode[i] == '|') || (modCode[i] == '^')) ||
            (((modCode[i] == '<') || (modCode[i] == '>')) &&
                (modCode[i - 1] == modCode[i]) && (i > 0)))
            modCode.insert((i + 1), " ");
        if (((modCode[i + 1] == '<') || (modCode[i + 1] == '>')) &&
            (modCode[i + 2] == modCode[i + 1]) && (i < (modCode.length() - 2)))
            modCode.insert((i += 2), " ");
    }
    return modCode;
}

/// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
std::string CppCode::ModifyV2() {
    std::string modCode = this->cppCode;
    for (int i = 0; i < modCode.length(); i++) {
        std::set<char> chars = {'(', ')', '{', '}', ';', ',', '=', '+', '-', '*', '/', '%', '&', '|'};
        if ((chars.find(modCode[i + 1]) != chars.end()) && (i < (modCode.length() - 1)))
            modCode.insert(++i, " ");
        chars = {'(', ')', '{', '}', ';', ':', ',', '=', '+', '-', '*', '/', '%', '&', '|'};
        if ((chars.find(modCode[i]) != chars.end()) ||
            (((modCode[i] == '<') || (modCode[i] == '>')) &&
                (modCode[i - 1] == modCode[i]) && (i > 0)))
            modCode.insert((i + 1), " ");
        if (((modCode[i + 1] == '<') || (modCode[i + 1] == '>')) &&
            (modCode[i + 2] == modCode[i + 1]) && (i < (modCode.length() - 2)))
            modCode.insert((i += 2), " ");
    }
    return modCode;
}


/// <summary>Разбиение кода на токены</summary>
std::vector<std::string> CppCode::GetTokens() {
    setlocale(LC_ALL, "Russian");
    std::vector<std::string> tokens = {};
    const char* separators = " \n\t";
    std::string modCppCode = this->ModifyV2(),
        lineCppCode,
        lineCppCodeToTab;
    std::istringstream ssCppCode(modCppCode),
        ssLineCppCode,
        ssLineCppCodeToTab;
    while (getline(ssCppCode, lineCppCode, '\n')) {
        ssLineCppCode = std::istringstream(lineCppCode);
        while (getline(ssLineCppCode, lineCppCodeToTab, '\t')) {
            ssLineCppCodeToTab = std::istringstream(lineCppCodeToTab);
            while (1) {
                std::string token = "";
                ssLineCppCodeToTab >> token;
                if (token == "")
                    break;
                tokens.push_back(token);
            }
            tokens.push_back("\t");
        }
        tokens.push_back("\n");
    }
    return tokens;
}

/// <summary>Разбиение кода на токены</summary>
std::vector<std::string> CppCode::GetTokensV2() {
    std::vector<std::string> tokens = {};
    std::string cppCode = this->cppCode;
    int lenOfCppCode = cppCode.length();
    for (int i = 0; i < lenOfCppCode; i++)
        for (int j = i + 1; j < lenOfCppCode - i - 1; j++) {
            std::string codeSubStr = cppCode.substr(i, j);
            for (std::string str : this->keyWordsSet)
                if (codeSubStr == str) {
                    tokens.push_back(codeSubStr);
                    i = j;
                    break;
                }
            for (std::string str : this->types)
                if (codeSubStr == str) {
                    tokens.push_back(codeSubStr);
                    i = j;
                    break;
                }
            for (std::string str : this->specSymbolsSet)
                if (codeSubStr == str) {
                    tokens.push_back(codeSubStr);
                    i = j;
                    break;
                }
            if (regex_match(codeSubStr, std::regex("[a-zA-Z0-9_]+"))) {
                tokens.push_back(codeSubStr);
                i = j;
                break;
            }
        }
    return {};
}

/// <summary>Получение типов данных, описанных в коде</summary>
/// <param name="type">- способ объявления пользовательского типа</param>
/// <returns>вектор пользовательских типов данных</returns>
std::vector<std::string> CppCode::GetTypes(std::string type) {
    std::vector<std::string> types = {},
        tokens = this->tokens;
    int tokensSize = tokens.size();
    for (int i = 0; i < tokensSize; i++)
        if (tokens[i] == type) {
            bool mode2 = (type == "typedef" || (type == "enum" && tokens[i + 1] == "class"));
            types.push_back(tokens[i + (mode2 ? 2 : 1)]);
        }
    return types;
}

/// <summary>Получение вектора методов</summary>
/// <returns>- вектор методов в коде</returns>
std::vector<CppMethod> CppCode::GetMethods() {
    std::vector<CppMethod> methods = {};
    std::vector<std::string> types = this->GetStdTypes(),
        tokens = this->tokens,
        typedefs = this->GetTypes("typedef"),
        structs = this->GetTypes("struct"),
        unions = this->GetTypes("union"),
        enums = this->GetTypes("enum"),
        usings = this->GetTypes("using"),
        classes = this->GetTypes("class");
    for (int i = 0; i < typedefs.size(); i++)
        types.push_back(typedefs[i]);
    for (int i = 0; i < structs.size(); i++)
        types.push_back(structs[i]);
    for (int i = 0; i < unions.size(); i++)
        types.push_back(unions[i]);
    for (int i = 0; i < enums.size(); i++)
        types.push_back(enums[i]);
    for (int i = 0; i < usings.size(); i++)
        types.push_back(usings[i]);
    for (int i = 0; i < classes.size(); i++)
        types.push_back(classes[i]);
    int typesSize = types.size(),
        tokensSize = tokens.size();
    for (int i = 0; i < tokensSize; i++) {
        std::vector<std::string> curMethod = {};
        bool isType = false;
        for (int j = 0; j < typesSize && !isType; j++)
            isType = ((tokens[i] == types[j]) &&
                (i < tokensSize - 2) &&
                (tokens[i + 2] == "("));
        if (isType) {
            int depth = 0,
                k = 0;
            for (k = i; k < tokensSize; k++) {
                if (tokens[k] == "{")
                    depth++;
                if (tokens[k] == "}") {
                    if (depth > 1)
                        depth--;
                    else
                        break;
                }
            }
            for (int l = i; l < k; l++)
                curMethod.push_back(tokens[l]);
            curMethod.push_back("}");
            i = k;
            methods.push_back(CppMethod(curMethod));
        }
    }
    return methods;
}

std::vector<CppClass> CppCode::GetClasses() {
    std::vector<CppClass> classes = {};
    std::vector<std::string> tokens = this->tokens,
        newClassTokens = {};
    int tokensSize = tokens.size(),
        j = 0,
        depth = 0;
    for (int i = 0; i < tokensSize; i++)
        if (tokens[i] == "class" && (i > 0 && tokens[i - 1] != "enum")) {
            for (j = i; j < tokensSize; j++) {
                if (tokens[j] == "{")
                    depth++;
                if (tokens[j] == "}") {
                    if (depth > 1)
                        depth--;
                    else
                        break;
                }
            }
            for (int k = i; k < j; k++)
                newClassTokens.push_back(tokens[k]);
            newClassTokens.push_back("}");
            i = j;
            classes.push_back(CppClass(newClassTokens, this->types));
        }
    return classes;
}

std::vector<MethodCall> CppCode::GetMethodCalls(CppMethod cppMethod) {
    std::vector<std::string> tokens = this->tokens;
    int tokensSize = tokens.size(),
        parametersSize = cppMethod.parameters.size(),
        j = 0,
        k = 0,
        depth = 0;
    std::string cppMethodName = cppMethod.name,
        parameterValue = "";
    std::vector<MethodCall> methodCalls = {};
    for (int i = 0; i < tokens.size(); i++)
        if (tokens[i] == cppMethodName) {
            MethodCall methodCall = {};
            methodCall.methodName = cppMethodName;
            methodCall.parameterValues = {};
            depth = 0;
            tokensSize = tokens.size();
            for (j = i + 1; j < tokensSize; j++) {
                if (tokens[j] == "(")
                    depth++;
                if (tokens[j] == ")") {
                    if (depth > 1)
                        depth--;
                    else
                        break;
                }
            }
            if (j < tokensSize - 1 && tokens[j + 1] != "{") {
                parameterValue = "";
                for (k = i + 2; k < j + 1; k++) {
                    if (tokens[k] != "," && k != j)
                        parameterValue += (tokens[k] + (tokens[k + 1] == ")" || tokens[k + 1] == "," ? "" : " "));
                    else {
                        methodCall.parameterValues.push_back(parameterValue);
                        parameterValue = "";
                    }
                }
                i = j;
            }
            methodCalls.push_back(methodCall);
        }
    return methodCalls;
}

void CppCode::UpdateMethodCalls(CppMethod cppMethod) {
    std::vector<std::string> tokens = this->tokens,
        newTokens = {};
    int tokensSize = tokens.size(),
        parametersSize = cppMethod.parameters.size(),
        oldParametersSize = cppMethod.oldParameters.size(),
        j = 0,
        k = 0,
        depth = 0;
    std::string cppMethodName = cppMethod.name,
        parameterValue = "";
    for (int i = 0; i < tokens.size(); i++)
        if (tokens[i] != cppMethodName) {
            if (i < tokens.size() - 1 && tokens[i + 1] == cppMethodName)
                newTokens.push_back(tokens[i] + " " + tokens[i + 1]);
            else
                newTokens.push_back(tokens[i]);
        }
        else {
            MethodCall methodCall = {};
            methodCall.methodName = cppMethodName;
            methodCall.parameterValues = {};
            depth = 0;
            tokensSize = tokens.size();
            for (j = i + 1; j < tokensSize; j++) {
                if (tokens[j] == "(")
                    depth++;
                if (tokens[j] == ")") {
                    if (depth > 1)
                        depth--;
                    else
                        break;
                }
            }
            if (tokens[j + 1] != "{") {
                parameterValue = "";
                for (k = i + 2; k < j + 1; k++)
                    if (tokens[k] != "," && k != j)
                        parameterValue += (tokens[k] + (tokens[k + 1] == ")" || tokens[k + 1] == "," ? "" : " "));
                    else {
                        methodCall.parameterValues.push_back(parameterValue);
                        parameterValue = "";
                    }
                newTokens.push_back("(");
                if (parametersSize == methodCall.parameterValues.size())
                    for (int l = 0; l < parametersSize; l++) {
                        newTokens.push_back(methodCall.parameterValues[l]);
                        if (l < parametersSize - 1)
                            newTokens.push_back(",");
                    }
                else {
                    int newParametersSize = methodCall.parameterValues.size(),
                        m = 0;
                    for (int l = 0; l < parametersSize; l++)
                        if (cppMethod.parameters[l].name == cppMethod.oldParameters[m].name && m < oldParametersSize && l < parametersSize) {
                            newTokens.push_back(methodCall.parameterValues[l]);
                            if (l < parametersSize - 1)
                                newTokens.push_back(",");
                        }
                        else {
                            l--;
                            m++;
                        }
                }
                newTokens.push_back(")");
                i = j;
            }
        }
    this->tokens = newTokens;
}

void CppCode::UpdateMethodCalls(CppMethod cppMethod, MethodCall methodCall, MethodCall newMethodCall) {
    if (methodCall.methodName == cppMethod.name) {
        std::vector<std::string> tokens = this->tokens,
            newTokens = {};
        int tokensSize = tokens.size();
        for (int i = 0; i < tokensSize; i++)
            if ((i < tokensSize - 2) &&
                tokens[i] != methodCall.methodName &&
                tokens[i + 1] != "(" &&
                tokens[i + 2] != methodCall.parameterValues[0])
                newTokens.push_back(tokens[i]);
            else {
                if (i < tokensSize + 2 && 
                    tokens[i] == methodCall.methodName &&
                    tokens[i + 2] == methodCall.parameterValues[0]) {
                    newTokens.push_back(newMethodCall.methodName);
                    newTokens.push_back("(");
                    int parametersSize = methodCall.parameterValues.size(),
                        newParametersSize = newMethodCall.parameterValues.size();
                    for (int j = 0; j < newParametersSize; j++) {
                        newTokens.push_back(newMethodCall.parameterValues[j]);
                        if (j < newParametersSize - 1)
                            newTokens.push_back(",");
                    }
                    newTokens.push_back(")");
                    i += 3;
                    for (int j = 0; j < parametersSize; j++)
                        i += (j < parametersSize - 1) ? 2 : 1;
                }
                else
                    newTokens.push_back(tokens[i]);
            }
        this->tokens = newTokens;
    }
}

void CppCode::UpdateMethodCallsV2(CppMethod cppMethod, MethodCall methodCall, MethodCall newMethodCall) {
    if (methodCall.methodName == cppMethod.name) {
        std::vector<CppMethod> methods = this->methods;
        int methodsSize = methods.size();
        for (int i = 0; i < methodsSize; i++) {
            std::vector<std::string> tokens = methods[i].GetTokens(),
                newLexemes = {};
            if (tokens.size() == 0) {
                tokens = methods[i].ToTokens();
            }
            int lexemesSize = tokens.size();
            for (int i = 0; i < lexemesSize; i++)
                if ((i < lexemesSize - 2) &&
                    tokens[i] != methodCall.methodName &&
                    tokens[i + 1] != "(" &&
                    tokens[i + 2] != methodCall.parameterValues[0])
                    newLexemes.push_back(tokens[i]);
                else {
                    if (i < lexemesSize + 2 &&
                        tokens[i] == methodCall.methodName &&
                        tokens[i + 2] == methodCall.parameterValues[0]) {
                        newLexemes.push_back(newMethodCall.methodName);
                        newLexemes.push_back("(");
                        int parametersSize = methodCall.parameterValues.size(),
                            newParametersSize = newMethodCall.parameterValues.size();
                        for (int j = 0; j < newParametersSize; j++) {
                            newLexemes.push_back(newMethodCall.parameterValues[j]);
                            if (j < newParametersSize - 1)
                                newLexemes.push_back(",");
                        }
                        newLexemes.push_back(")");
                        int k = 0,
                            depth = 0;
                        for (k = i; k < lexemesSize; k++) {
                            if (tokens[k] == "(" || tokens[k] == "{")
                                depth++;
                            if (tokens[k] == ")" || tokens[k] == "}") {
                                if (depth > 1)
                                    depth--;
                                else
                                    break;
                            }
                        }
                        newLexemes.push_back(tokens[k + 1]);
                        i += 3;
                        for (int j = 0; j < parametersSize; j++)
                            i += (j < parametersSize - 1) ? 2 : 1;
                    }
                    else
                        newLexemes.push_back(tokens[i]);
                }
            methods[i] = CppMethod(newLexemes);
        }
        this->methods = methods;
    }
}

void CppCode::UpdateMethods(std::vector<CppMethod> methods) {
    int methodsSize = methods.size();
    for (int i = 0; i < methodsSize; i++) {
        if (methods[i].name == "main") {
            std::vector<CppMethod> buff = {};
            while (methods[methods.size() - 1].name != "main") {
                buff.push_back(methods[methods.size() - 1]);
                methods.pop_back();
            }
            CppMethod main = methods[methods.size() - 1];
            methods.pop_back();
            while (buff.size() > 0) {
                methods.push_back(buff[buff.size() - 1]);
                buff.pop_back();
            }
            methods.push_back(main);
        }
    }
    this->methods = methods;
}

void CppCode::UpdateTokens() {
    std::vector<std::string> newTokens = {},
        curTokens = this->tokens;
    std::vector<CppMethod> methods = this->methods;
    int methodsSize = methods.size(),
        curTokensSize = curTokens.size();
    for (int i = 0; i < methodsSize; i++) {
        for (int j = 0; j < curTokens.size(); j++) {
            if ((curTokens[j] == methods[i].type) && 
                (curTokens[j + 1] == methods[i].name)) {
                int k = 0,
                    depth = 0;
                for (k = j + 1; k < curTokens.size(); k++) {
                    if (curTokens[k] == "{")
                        depth++;
                    if ((curTokens[k] == "}") && (depth > 0)) {
                        if (depth > 1)
                            depth--;
                        else
                            break;
                    }
                }
                j = k;
                std::vector<std::string> newMethodTokens = methods[i].ToTokens();
                int newMethodTokensSize = newMethodTokens.size();
                for (int l = 0; l < newMethodTokensSize; l++)
                    newTokens.push_back(newMethodTokens[l]);
            }
            else
                newTokens.push_back(curTokens[j]);
        }
        curTokens = newTokens;
        newTokens = {};
    }
    this->tokens = curTokens;
}

void CppCode::UpdateTokensV2() {
    std::vector<std::string> newTokens = {},
        curTokens = this->tokens;
    std::vector<CppMethod> methods = this->methods;
    int methodsSize = methods.size(),
        curTokensSize = curTokens.size();
    for (int i = 0; i < methodsSize; i++) {
        bool isNotNewMethod = false,
            isMethod = false;
        for (int j = 0; j < curTokens.size(); j++)
            if ((curTokens[j] == methods[i].type) &&
                (curTokens[j + 1] == methods[i].name)) {
                isNotNewMethod = true;
                break;
            }
        if (isNotNewMethod)
            for (int j = 0; j < curTokens.size(); j++) {
                if ((curTokens[j] == methods[i].type) &&
                    (curTokens[j + 1] == methods[i].name)) {
                    int k = 0,
                        depth = 0;
                    for (k = j + 1; k < curTokens.size(); k++) {
                        if (curTokens[k] == "{")
                            depth++;
                        if ((curTokens[k] == "}") && (depth > 0)) {
                            if (depth > 1)
                                depth--;
                            else
                                break;
                        }
                    }
                    j = k;
                    std::vector<std::string> newMethodTokens = methods[i].ToTokens();
                    int newMethodTokensSize = newMethodTokens.size();
                    for (int l = 0; l < newMethodTokensSize; l++)
                        newTokens.push_back(newMethodTokens[l]);
                }
                else
                    newTokens.push_back(curTokens[j]);
            }
        else {
            bool nextMethodIsNew = false;
            for (int j = 0; j < curTokens.size(); j++) {
                if (i > 0 && (curTokens[j] == methods[i - 1].type) &&
                    (curTokens[j + 1] == methods[i - 1].name)) {
                    nextMethodIsNew = true;
                }
                if (nextMethodIsNew && 
                    (curTokens[j] == methods[i + 1].type) &&
                    (curTokens[j + 1] == methods[i + 1].name)) {
                    nextMethodIsNew = true;
                    std::vector<std::string> newMethodTokens = methods[i].ToTokens();
                    int newMethodTokensSize = newMethodTokens.size();
                    for (int l = 0; l < newMethodTokensSize; l++)
                        newTokens.push_back(newMethodTokens[l]);
                }
                newTokens.push_back(curTokens[j]); 
            }
        }
        curTokens = newTokens;
        newTokens = {};
    }
    this->tokens = curTokens;
}

void CppCode::UpdateTokensV3() {
    std::vector<std::string> newTokens = {},
        curTokens = this->tokens;
    std::vector<CppMethod> methods = this->methods;
    std::vector<CppClass> classes = this->classes;
    int methodsSize = methods.size(),
        classesSize = classes.size(),
        curTokensSize = curTokens.size();
    for (int i = 0; i < classesSize; i++) {
        bool isNotNewClass = false;
        for (int j = 0; j < curTokensSize; j++) {
            if (j < curTokens.size() - 1 &&
                curTokens[j] == "class" &&
                curTokens[j + 1] == classes[i].name) {
                isNotNewClass = true;
                break;
            }
        }
        if (isNotNewClass) {
            for (int j = 0; j < curTokens.size(); j++) {
                if (curTokens[j] == "class" &&
                    curTokens[j + 1] == classes[i].name) {
                    int k = 0,
                        depth = 0;
                    for (k = j + 1; k < curTokens.size(); k++) {
                        if (curTokens[k] == "{")
                            depth++;
                        if ((curTokens[k] == "}") && (depth > 0)) {
                            if (depth > 1)
                                depth--;
                            else
                                break;
                        }
                    }
                    j = k;
                    std::vector<std::string> newClassTokens = classes[i].ToTokens();
                    int newClassTokensSize = newClassTokens.size() - 1;
                    for (int l = 0; l < newClassTokensSize; l++)
                        newTokens.push_back(newClassTokens[l]);
                }
                else
                    newTokens.push_back(curTokens[j]);
            }
        }
        else {
            bool nextClassIsNew = false;
            for (int j = 0; j < curTokens.size(); j++) {
                if (i > 0 && (curTokens[j] == "class") &&
                    (curTokens[j + 1] == classes[i - 1].name)) {
                    nextClassIsNew = true;
                }
                if (j > 0 &&
                    i > 0 &&
                    (nextClassIsNew &&
                    (curTokens[j - 1] == "}") &&
                    (curTokens[j] == ";"))) {
                    nextClassIsNew = true;
                    std::vector<std::string> newClassTokens = classes[i].ToTokens();
                    int newClassTokensSize = newClassTokens.size() - 1;
                    newTokens.push_back(";");
                    newTokens.push_back("\n");
                    newTokens.push_back("\n");
                    for (int l = 0; l < newClassTokensSize; l++)
                        newTokens.push_back(newClassTokens[l]);
                    /*if (nextClassIsNew) {
                        newTokens.push_back(";");
                        newTokens.push_back("\n");
                        newTokens.push_back("\n");
                        newTokens.push_back("class");
                    }*/
                }
                newTokens.push_back(curTokens[j]);
            }
        }
        curTokens = newTokens;
        newTokens = {};
    }
    for (int i = 0; i < methodsSize; i++) {
        bool isNotNewMethod = false,
            isMethod = false;
        for (int j = 0; j < curTokens.size(); j++)
            if ((curTokens[j] == methods[i].type) &&
                (curTokens[j + 1] == methods[i].name)) {
                isNotNewMethod = true;
                break;
            }
        if (isNotNewMethod)
            for (int j = 0; j < curTokens.size(); j++) {
                if ((curTokens[j] == methods[i].type) &&
                    (curTokens[j + 1] == methods[i].name)) {
                    int k = 0,
                        depth = 0;
                    for (k = j + 1; k < curTokens.size(); k++) {
                        if (curTokens[k] == "{")
                            depth++;
                        if ((curTokens[k] == "}") && (depth > 0)) {
                            if (depth > 1)
                                depth--;
                            else
                                break;
                        }
                    }
                    j = k;
                    std::vector<std::string> newMethodTokens = methods[i].ToTokens();
                    int newMethodTokensSize = newMethodTokens.size();
                    for (int l = 0; l < newMethodTokensSize; l++)
                        newTokens.push_back(newMethodTokens[l]);
                }
                else
                    newTokens.push_back(curTokens[j]);
            }
        else {
            bool nextMethodIsNew = false;
            for (int j = 0; j < curTokens.size(); j++) {
                if (i > 0 && (curTokens[j] == methods[i - 1].type) &&
                    (curTokens[j + 1] == methods[i - 1].name)) {
                    nextMethodIsNew = true;
                }
                if (nextMethodIsNew &&
                    i < methods.size() - 1 &&
                    (curTokens[j] == methods[i + 1].type) &&
                    (curTokens[j + 1] == methods[i + 1].name)) {
                    nextMethodIsNew = true;
                    std::vector<std::string> newMethodTokens = methods[i].ToTokens();
                    int newMethodTokensSize = newMethodTokens.size();
                    for (int l = 0; l < newMethodTokensSize; l++)
                        newTokens.push_back(newMethodTokens[l]);
                }
                newTokens.push_back(curTokens[j]);
            }
        }
        curTokens = newTokens;
        newTokens = {};
    }
    this->tokens = curTokens;
}

std::string CppCode::ToString() {
    std::stringstream cppCodeStream;
    std::vector<std::string> tokens = this->tokens;
    int tokensSize = tokens.size();
    for (int i = 0; i < tokensSize; i++) {
        std::string sep = (i < tokensSize - 1 ? this->GetSep(tokens[i], tokens[i + 1]) : " ");
        cppCodeStream << tokens[i] << sep;
    }
    return cppCodeStream.str();
}

std::string CppCode::GetCode() {
    return this->cppCode;
}

std::string CppCode::GetSep(std::string curLexeme, std::string nxtLexeme) {
    if ((curLexeme == "/" || curLexeme == "<" || curLexeme == ">" || curLexeme == "=") && nxtLexeme == curLexeme)
        return "";
    else if (curLexeme == "if" && nxtLexeme == "(")
        return " ";
    else if (curLexeme == "\n" || curLexeme == "\t" ||
        curLexeme == "(" || nxtLexeme == "(" || nxtLexeme == ")" ||
        nxtLexeme == "," || nxtLexeme == ";")
        return "";
    else if ((curLexeme == "-" && nxtLexeme.substr(0, 1) == ">") ||
        (curLexeme == "." || nxtLexeme == "."))
        return "";
    else if (curLexeme == "this" && nxtLexeme == "-")
        return "";
    else if (curLexeme == "this->")
        return "";
    else if (curLexeme == "std:" || curLexeme == ":")
        return "";
    else if ((curLexeme == "+" || curLexeme == "-") && nxtLexeme == "=")
        return "";
    else if (curLexeme != "\n" && nxtLexeme == "}")
        return "\n";
    else
        return " ";
}

void CppCode::UpdateCode(std::string code) {
    this->cppCode = code;
}