#include "CppClassMethod.h"

CppClassMethod::CppClassMethod() {
    accessMode = AccessMode::Null;
    this->lexemes = {};
    parameters = {};
    oldParameters = {};
    body = {};
    name = "";
    type = "";
    usingFields = {};
    usingMethods = {};
    usingConstructors = {};
    usingDestructor = {};
    className = "";
    //cppClass = {};
}

//CppClassMethod::CppClassMethod(vector<string> lexemes, string className) {
//    accessMode = GetAccessMode();
//    this->lexemes = lexemes;
//    parameters = GetMethodParameters();
//    oldParameters = GetMethodParameters();
//    body = GetMethodBody();
//    name = GetMethodNameV2();
//    type = GetMethodType();
//    usingFields = GetUsingFields();
//    usingMethods = GetUsingMethods();
//    usingConstructors = GetUsingConstructors();
//    usingDestructor = GetUsingDestructor();
//    this->className = className;
//}

CppClassMethod::CppClassMethod(vector<string> lexemes, 
                               string className, 
                               vector<string> classCode, 
                               vector<CppClassField> fields, 
                               vector<CppClassMethod> methods, 
                               vector<CppClassConstructor> constructors) {
    accessMode = GetAccessMode(classCode);
    this->lexemes = lexemes;
    parameters = GetMethodParameters();
    oldParameters = GetMethodParameters();
    body = GetMethodBody();
    name = GetMethodName();
    type = GetMethodType();
    usingFields = GetUsingFields(fields);
    usingMethods = GetUsingMethods(methods);
    usingConstructors = GetUsingConstructors(constructors);
    usingDestructor = GetUsingDestructor();
    this->className = className;
}

//CppClassMethod::CppClassMethod(vector<string> lexemes, string className) {
//    accessMode = GetAccessMode();
//    this->lexemes = lexemes;
//    parameters = GetMethodParameters();
//    oldParameters = GetMethodParameters();
//    body = GetMethodBody();
//    name = GetMethodName();
//    type = GetMethodType();
//    usingFields = GetUsingFields();
//    usingMethods = GetUsingMethods();
//    usingConstructors = GetUsingConstructors();
//    usingDestructor = GetUsingDestructor();
//    this->className = className;
//}

vector<Parameter> CppClassMethod::GetMethodParameters() {
    vector<string> lexemes = this->lexemes;
    vector<Parameter> parameters = {};
    bool params = false;
    int lexemesSize = lexemes.size(),
        start = -1,
        j = 0;
    for (int i = 0; i < lexemesSize; i++) {
        if (lexemes[i] == "(" && !params) {
            start = i;
            params = true;
        }
        if (lexemes[i] == ")") {
            j = i;
            break;
        }
    }
    for (int i = start + 1; i < j - 1; i += 3)
        if (lexemes[i] != ")") {
            Parameter parameter = {};
            parameter.type = lexemes[i];
            parameter.name = lexemes[i + 1];
            parameters.push_back(parameter);
        }
    return parameters;
}

vector<string> CppClassMethod::GetMethodBody() {
    vector<string> lexemes = this->lexemes,
        bodyLexemes = {};
    int lexemesSize = lexemes.size(),
        depth = 1,
        i = 0,
        start = -1;
    if (lexemesSize != 0) {
        for (i = 0; i < lexemesSize; i++) {
            if (lexemes[i] == "{") {
                depth++;
                if (start == -1)
                    start = i;
            }
            if (lexemes[i] == "}") {
                if (depth > 2)
                    depth--;
                else
                    break;
            }
        }
        if (start >= 0) {
            for (int j = start; j < i; j++)
                bodyLexemes.push_back(lexemes[j]);
            bodyLexemes.push_back("}");
            return bodyLexemes;
        }
    }
    return {};
}

vector<string> CppClassMethod::GetUsingFields(vector<CppClassField> fields) {
    vector<string> usingFields = {},
        lexemes = this->lexemes;
    vector<CppClassField> classFields = fields;
    for (string lexeme : lexemes)
        for (CppClassField classField : classFields)
            if (lexeme == classField.name) {
                usingFields.push_back(classField.name);
                break;
            }
    return usingFields;
}

vector<CppClassMethod> CppClassMethod::GetUsingMethods(vector<CppClassMethod> methods) {
    vector<string> lexemes = this->lexemes;
    vector<CppClassMethod> usingMethods = {},
        classMethods = methods;
    for (string lexeme : lexemes)
        for (CppClassMethod classMethod : classMethods)
            if (lexeme == classMethod.name) {
                usingMethods.push_back(classMethod);
                break;
            }
    return usingMethods;
}

vector<CppClassConstructor> CppClassMethod::GetUsingConstructors(vector<CppClassConstructor> constructors) {
    vector<string> lexemes = this->lexemes;
    vector<CppClassConstructor> classConstructors = constructors,
        usingConstructors = {};
    int valuesSize = 0;
    for (int i = 0; i < lexemes.size(); i++)
        for (CppClassConstructor constructor : classConstructors)
            if (lexemes[i] == name &&
                lexemes[i + 1] == "(" &&
                i < lexemes.size() - 1) {
                if (lexemes[i + 2] == ")" &&
                    constructor.parameters.size() == 0)
                    usingConstructors.push_back(constructor);
                else {
                    valuesSize = 1;
                    for (int j = i + 2; j < lexemes.size(); j++)
                        if (lexemes[j] == ",")
                            valuesSize++;
                    if (constructor.parameters.size() == valuesSize)
                        usingConstructors.push_back(constructor);
                }
            }
    return usingConstructors;
}

CppClassDestructor CppClassMethod::GetUsingDestructor() {
    return this->usingDestructor;
}

AccessMode CppClassMethod::GetAccessMode(vector<string> classCode) {
    vector<string> lexemes = classCode;
    AccessMode accessMode = AccessMode::Private;
    for (int i = 0; i < lexemes.size(); i++)
        if (lexemes[i] == this->type &&
            lexemes[i + 1] == this->name &&
            lexemes[i + 2] == "(" &&
            i < lexemes.size() - 2) {
            for (int j = i; j >= 0; j--) {
                if (lexemes[j] == "public") {
                    accessMode = AccessMode::Public;
                    break;
                }
                else if (lexemes[j] == "private") {
                    accessMode = AccessMode::Private;
                    break;
                }
                else if (lexemes[j] == "protected") {
                    accessMode = AccessMode::Protected;
                    break;
                }
            }
        }
    return accessMode;
}

string CppClassMethod::GetClassName() {
    return this->lexemes[1];
}

string CppClassMethod::GetMethodType() {
    return this->lexemes[0];
}

string CppClassMethod::GetMethodName() {
    return this->lexemes[1];
}

string CppClassMethod::GetMethodNameV2() {
    return this->lexemes[4];
}

vector<string> CppClassMethod::ToLexemes() {
    vector<string> lexemes = {},
        body = this->body;
    vector<Parameter> parameters = this->parameters;
    int parametersSize = parameters.size(),
        bodySize = body.size();
    lexemes.push_back(this->type);
    lexemes.push_back(this->name);
    lexemes.push_back("(");
    for (int i = 0; i < parametersSize; i++) {
        lexemes.push_back(parameters[i].type);
        lexemes.push_back(parameters[i].name);
        if (i < parametersSize - 1)
            lexemes.push_back(",");
    }
    lexemes.push_back(")");
    for (int i = 0; i < bodySize; i++)
        lexemes.push_back(body[i]);
    return lexemes;
}
