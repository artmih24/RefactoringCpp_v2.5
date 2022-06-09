// файл CppMethod.cpp - реализация методов класса CppMethod

#include "CppMethod.h"

CppMethod::CppMethod() {
    tokens = {};
    parameters = {};
    oldParameters = {};
    body = {};
    type = "";
    name = "";
}

CppMethod::CppMethod(std::vector<std::string> tokens_) {
    tokens = tokens_;
    parameters = this->GetMethodParameters();
    oldParameters = this->GetMethodParameters();
    body = this->GetMethodBody();
    type = this->GetMethodType();
    name = this->GetMethodName();
}

std::vector<Parameter> CppMethod::GetMethodParameters() {
    std::vector<std::string> tokens = this->tokens;
    std::vector<Parameter> parameters = {};
    bool params = false;
    int tokensSize = tokens.size(),
        start = -1,
        j = 0;
    for (int i = 0; i < tokensSize; i++) {
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

std::vector<std::string> CppMethod::GetMethodBody() {
    std::vector<std::string> tokens = this->tokens,
        bodyTokens = {};
    int tokensSize = tokens.size(),
        depth = 0, 
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
                if (depth > 1)
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

std::string CppMethod::GetMethodType() {
    return this->tokens[0];
}

std::string CppMethod::GetMethodName() {
    return this->tokens[1];
}

std::vector<std::string> CppMethod::GetTokens() {
    return this->tokens;
}

std::vector<std::string> CppMethod::ToTokens() {
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