#include "CppClassMethod.h"

CppClassMethod::CppClassMethod() {
    accessMode = AccessMode::Private;
    this->tokens = {};
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

CppClassMethod::CppClassMethod(std::vector<std::string> methodTokens,
                               std::string className,
                               std::vector<std::string> tokens,
                               std::vector<CppClassField> fields,
                               std::vector<CppClassMethod> methods,
                               std::vector<CppClassConstructor> constructors) {
    accessMode = GetAccessMode(tokens);
    this->tokens = methodTokens;
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

std::vector<Parameter> CppClassMethod::GetMethodParameters() {
    std::vector<std::string> tokens = this->tokens;
    std::vector<Parameter> parameters = {};
    bool params = false;
    int lexemesSize = tokens.size(),
        start = -1,
        j = 0;
    for (int i = 0; i < lexemesSize; i++) {
        if (tokens[i] == "(" && !params) {
            start = i;
            params = true;
        }
        if (tokens[i] == ")") {
            j = i;
            break;
        }
    }
    for (int i = start + 1; i < j - 1; i += 3)
        if (tokens[i] != ")") {
            Parameter parameter = Parameter(tokens[i], tokens[i + 1]);
            parameters.push_back(parameter);
        }
    return parameters;
}

std::vector<std::string> CppClassMethod::GetMethodBody() {
    std::vector<std::string> tokens = this->tokens,
                             bodyTokens = {};
    int tokensSize = tokens.size(),
        depth = 1,
        i = 0,
        start = -1;
    if (tokensSize != 0) {
        for (i = 0; i < tokensSize; i++) {
            if (tokens[i] == "{") {
                depth++;
                if (start == -1)
                    start = i;
            }
            if (tokens[i] == "}") {
                if (depth > 2)
                    depth--;
                else
                    break;
            }
        }
        if (start >= 0) {
            for (int j = start; j < i; j++)
                bodyTokens.push_back(tokens[j]);
            bodyTokens.push_back("}");
            return bodyTokens;
        }
    }
    return {};
}

std::vector<std::string> CppClassMethod::GetUsingFields(std::vector<CppClassField> fields) {
    std::vector<std::string> usingFields = {},
                             result = {},
                             tokens = this->tokens;
    std::vector<CppClassField> classFields = fields;
    for (std::string token : tokens)
        for (CppClassField classField : classFields) {
            if (token == classField.name) {
                usingFields.push_back(classField.name);
                break;
            }
            else if (token[0] == '>' && 
                     token.substr(1, token.size() - 1) == classField.name) {
                usingFields.push_back(classField.name);
                break;
            }
        }
    for (std::string field : usingFields) {
        bool exists = false;
        for (std::string res : result)
            if (res == field) {
                exists = true;
                break;
            }
        if (!exists)
            result.push_back(field);
    }
    return result;
}

std::vector<CppClassMethod> CppClassMethod::GetUsingMethods(std::vector<CppClassMethod> methods) {
    std::vector<std::string> tokens = this->tokens;
    std::vector<CppClassMethod> usingMethods = {},
                                result = {},
                                classMethods = methods;
    for (std::string token : tokens)
        for (CppClassMethod classMethod : classMethods)
            if (token == classMethod.name) {
                usingMethods.push_back(classMethod);
                break;
            }
    for (CppClassMethod method : usingMethods) {
        bool exists = false;
        for (CppClassMethod res : result)
            if (res.name == method.name) {
                exists = true;
                break;
            }
        if (!exists)
            result.push_back(method);
    }
    return result;
}

std::vector<CppClassConstructor> CppClassMethod::GetUsingConstructors(std::vector<CppClassConstructor> constructors) {
    std::vector<std::string> tokens = this->tokens;
    std::vector<CppClassConstructor> classConstructors = constructors,
                                     result = {},
                                     usingConstructors = {};
    int valuesSize = 0;
    for (int i = 0; i < tokens.size(); i++)
        for (CppClassConstructor constructor : classConstructors)
            if (tokens[i] == name &&
                tokens[i + 1] == "(" &&
                i < tokens.size() - 1) {
                if (tokens[i + 2] == ")" &&
                    constructor.parameters.size() == 0)
                    usingConstructors.push_back(constructor);
                else {
                    valuesSize = 1;
                    for (int j = i + 2; j < tokens.size(); j++)
                        if (tokens[j] == ",")
                            valuesSize++;
                    if (constructor.parameters.size() == valuesSize)
                        usingConstructors.push_back(constructor);
                }
            }
    for (CppClassConstructor constructor : usingConstructors) {
        bool exists = false;
        if (!exists)
            result.push_back(constructor);
    }
    return result;
}

CppClassDestructor CppClassMethod::GetUsingDestructor() {
    return this->usingDestructor;
}

AccessMode CppClassMethod::GetAccessMode(std::vector<std::string> tokens) {
    std::vector<std::string> tokens_ = tokens;
    AccessMode accessMode = AccessMode::Private;
    for (int i = 0; i < tokens.size(); i++)
        if (tokens_[i] == this->type &&
            tokens_[i + 1] == this->name &&
            tokens_[i + 2] == "(" &&
            i < tokens_.size() - 2) {
            for (int j = i; j >= 0; j--) {
                if (tokens_[j] == "public") {
                    accessMode = AccessMode::Public;
                    break;
                }
                else if (tokens_[j] == "private") {
                    accessMode = AccessMode::Private;
                    break;
                }
                else if (tokens_[j] == "protected") {
                    accessMode = AccessMode::Protected;
                    break;
                }
            }
        }
    return accessMode;
}

std::string CppClassMethod::GetClassName() {
    return this->tokens[1];
}

std::string CppClassMethod::GetMethodType() {
    return this->tokens[0];
}

std::string CppClassMethod::GetMethodName() {
    return this->tokens[1];
}

std::string CppClassMethod::GetMethodNameV2() {
    return this->tokens[4];
}

std::vector<std::string> CppClassMethod::ToTokens() {
    std::vector<std::string> tokens = {},
                             body = this->body;
    std::vector<Parameter> parameters = this->parameters;
    int parametersSize = parameters.size(),
        bodySize = body.size();
    tokens.push_back(this->type);
    tokens.push_back(this->name);
    tokens.push_back("(");
    for (int i = 0; i < parametersSize; i++) {
        tokens.push_back(parameters[i].type);
        tokens.push_back(parameters[i].name);
        if (i < parametersSize - 1)
            tokens.push_back(",");
    }
    tokens.push_back(")");
    for (int i = 0; i < bodySize; i++)
        tokens.push_back(body[i]);
    return tokens;
}
