// ���� RefactoringManager.cpp - ���������� ������� ������ RefactoringManager

#include "RefactoringManager.h"

/// <summary>����������� ������� ������ RefactoringManager</summary>
/// <param name="filePath">- ���� � ����� � �������� �����</param>
RefactoringManager::RefactoringManager(std::string filePath) {
    this->filePath = filePath;
    this->outFilePath = this->GetOutputFileName(this->filePath);
    this->fileStream = std::ifstream(this->filePath);
    std::cout << this->outFilePath << std::endl;
}

/// <summary>���������� ������������</summary>
void RefactoringManager::Refactoring() {
    this->GetFileContent(this->filePath);
    this->code = CppCode(this->fileContent);
    this->RemoveAssignmentsToParametersProc(this->code);
    this->RemoveParametersProc(this->code);
    this->SendFullObjectProc(this->code);
    this->ExtractClassProc(this->code);
    this->fileContent = this->code.ToString();
    WriteFile(this->outFilePath);
}

/// <summary>���������� ������������ ������� �������� ������������ ����������</summary>
/// <param name="code">- �������� ��� ���������</param>
void RefactoringManager::RemoveAssignmentsToParametersProc(CppCode code) {
    std::vector<CppMethod> methods = code.methods;   // ������ ������� ����
    for (int i = 0; i < methods.size(); i++)
        methods[i] = this->RemoveAssignmentsToParameters(methods[i]);
    code.UpdateMethods(methods);
    code.UpdateTokens();
    this->code = code;
}

/// <summary>�������� ������������ ���������� ������</summary>
/// <param name="method">- ����������� �����</param>
/// <returns>����� �����</returns>
CppMethod RefactoringManager::RemoveAssignmentsToParameters(CppMethod method) {
    CppMethod newCppMethod,     // ����� �����
        curCppMethod = method;  // ������� �����
    // ���� �� ���� ����������
    for (int i = 0; i < curCppMethod.parameters.size(); i++) {
        newCppMethod.body = {};
        newCppMethod.parameters = curCppMethod.parameters;
        newCppMethod.type = curCppMethod.type;
        newCppMethod.name = curCppMethod.name;
        bool assignment = false;    // ���� ������������ ����������
        // �������� ���� ������ �� ������� ������������ ��������� � ���
        for (int j = 0; j < curCppMethod.body.size(); j++)
            // ���� ���� ������������
            if ((curCppMethod.body[j] == curCppMethod.parameters[i].name) &&
                ((((curCppMethod.body[j + 1] == "=") && (curCppMethod.body[j + 2] != "=")) && (j < curCppMethod.body.size() - 1)) ||
                    ((((curCppMethod.body[j + 1] == "+") || (curCppMethod.body[j + 1] == "-") ||
                        (curCppMethod.body[j + 1] == "*") || (curCppMethod.body[j + 1] == "/") ||
                        (curCppMethod.body[j + 1] == "%") || (curCppMethod.body[j + 1] == "&") ||
                        (curCppMethod.body[j + 1] == "|") || (curCppMethod.body[j + 1] == "^")) &&
                        (curCppMethod.body[j + 2] == "=")) && (j < curCppMethod.body.size() - 2)) ||
                    (((curCppMethod.body[j + 1] == "<") || (curCppMethod.body[j + 1] == ">")) &&
                        (curCppMethod.body[j + 2] == curCppMethod.body[j + 1]) &&
                        (curCppMethod.body[j + 3] == "=") && (j < curCppMethod.body.size() - 3)))) {
                assignment = true;
                break;
            }
        // ���� ���� ������������
        if (assignment) {
            int start = -1;
            for (int j = 0; j < curCppMethod.body.size(); j++) {
                // �������� ��� �� ����� ����������, ��������� ��������� ���� ���������� ��� ����
                if (curCppMethod.body[j] == curCppMethod.parameters[i].name)
                    newCppMethod.body.push_back("new_var_" + curCppMethod.body[j]);
                else
                    newCppMethod.body.push_back(curCppMethod.body[j]);
                // � ������ ���� ������ ��������� ����� ����������, ������� ����� �������� �������� ���������
                if ((curCppMethod.body[j] == "{") && (start == -1)) {
                    newCppMethod.body.push_back("\n\t" + curCppMethod.parameters[i].type);
                    newCppMethod.body.push_back("new_var_" + curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back("=");
                    newCppMethod.body.push_back(curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back(";");
                    start = j;
                }
            }
            // �������� ����� ����� ��������� ���������
            curCppMethod = newCppMethod;
        }
    }
    return curCppMethod;
}

/// <summary>���������� ������������ ������� �������� ����������</summary>
/// <param name="code">- �������� ��� ���������</param>
void RefactoringManager::RemoveParametersProc(CppCode code) {
    std::vector<CppMethod> methods = code.GetMethods();  // ������ ������� ����
    for (int j = 0; j < methods.size(); j++) {
        // ����������� ���������� ������� - �������� ������ ����������
        for (int i = 0; i < methods.size(); i++)
            methods[i] = this->RemoveParameters(methods[i]);
        code.UpdateMethods(methods);
        code.UpdateTokens();
        this->code = code;
        // ����������� �� ���� ����, ������� � ��������������� 
        // ������ - �������� ��������, �������������� � �������������� ���������
        for (int i = 0; i < methods.size(); i++)
            code.UpdateMethodCalls(methods[i]);
        code.UpdateMethods(methods);
        code.UpdateTokens();
        this->code = code;
        std::string codeString = code.ToString();
        code.UpdateCode(codeString);
        code.tokens = code.GetTokens();
        methods = code.GetMethods();
    }
    this->code = code;
}

/// <summary>�������� ���������� ������</summary>
/// <param name="method">- ����������� �����</param>
/// <returns>����� �����</returns>
CppMethod RefactoringManager::RemoveParameters(CppMethod method) {
    CppMethod newCppMethod,     // ����� �����
        curCppMethod = method;  // ������� �����
    std::vector<Parameter> parameters = curCppMethod.parameters; // ������ ���������� ������
    std::vector<std::string> body = curCppMethod.body;    // ������ ���� ������
    int parametersSize = parameters.size(), // ���-�� ����������
        bodySize = body.size(), // ���-�� ������� ���� ������
        depth = 0,  // ������� ����������� ������
        k = 0,
        l = 0;
    std::string parameterValue;
    newCppMethod.parameters = {};
    newCppMethod.oldParameters = parameters;
    newCppMethod.body = body;
    newCppMethod.type = curCppMethod.type;
    newCppMethod.name = curCppMethod.name;
    // ���� �� ���� ����������
    for (int i = 0; i < parameters.size(); i++) {
        bool usingParameter = false;
        // ���������, ������������ �� �������� � ���� ������
        for (int j = 0; j < body.size(); j++)
            // ���� ����� ������������ - � ��� ���� ���������� ����� ���� ��
            if (body[j] == curCppMethod.name) {
                MethodCall methodCall = {};
                methodCall.methodName = curCppMethod.name;
                methodCall.parameterValues = {};
                depth = 0;
                for (k = j + 1; k < body.size(); k++) {
                    if (body[k] == "(")
                        depth++;
                    if (body[k] == ")")
                        if (depth > 1)
                            depth--;
                        else
                            break;
                }
                parameterValue = "";
                // ���� �������� � ������������ ����������
                for (l = j + 2; l < k + 1; l++)
                    if (body[l] == parameters[i].name) {
                        j = k + 1;
                        break;
                    }
            }
            // ���� �������� ������������
            else if (body[j] == parameters[i].name) {
                usingParameter = true;
                break;
            }
        // ���� ������������, �� ��������� ��� � ������ ���������� ������ ������
        if (usingParameter)
            newCppMethod.parameters.push_back(parameters[i]);
        // �������� ����� ����� ��������� ���������
        curCppMethod = newCppMethod;
    }
    return curCppMethod;
}

void RefactoringManager::SendFullObjectProc(CppCode code) {
    std::vector<CppMethod> methods = code.methods;   // ������ ������� ����
    int j = 0;
    // ���� �� ���� �������
    for (int i = 0; i < methods.size(); i++) {
        std::vector<MethodCall> methodCalls = code.GetMethodCalls(methods[i]);
        // ���� �� ���� ������� ������
        for (MethodCall methodCall : methodCalls) {
            std::vector<std::string> values = methodCall.parameterValues;
            std::vector<Object> objects = {},
                objectsFin = {};
            // ���� �� ���� ��������� ����������
            for (int k = 0; k < values.size(); k++) {
                // ����� ����� �������� � ����������
                for (j = 0; j < values[k].size(); j++)
                    if (values[k].find('.') && j < values[k].size())
                        break;
                // ���� � ��������� ���������� ���� �������
                if (j < values[k].size()) {
                    // ������� ������
                    Object object = {};
                    object.name = values[k].substr(0, values[k].find('.'));
                    // �������, � ������ ���� ������ ���������
                    std::vector<std::string> tokens = code.tokens,
                        types = code.GetTypes("class"),
                        types2 = code.GetTypes("struct"),
                        types3 = code.GetTypes("union");
                    types.insert(types.end(), types2.begin(), types2.end());
                    types.insert(types.end(), types3.begin(), types3.end());
                    int lexemesSize = tokens.size();
                    for (int l = 0; l < lexemesSize; l++) {
                        for (std::string type : types)
                            if (tokens[l] == type && tokens[l + 1] == object.name) {
                                // ���� ������ ����� ���
                                object.className = tokens[l];
                                break;
                            }
                        if (object.className != "")
                            break;
                    }
                    //object.className = methods[i].parameters[k].type;
                    objects.push_back(object);
                }
            }
            // ������� ������ �������� ��� ������������� ���������
            for (Object object : objects) {
                bool exists = false;
                for (Object objectFin : objectsFin) {
                    if (objectFin.name == object.name) {
                        exists = true;
                        break;
                    }
                }
                if (!exists)
                    objectsFin.push_back(object);
            }
            // ���� � ���������� ���� ���� ������ � ���� �� �������
            if (objectsFin.size() == 1) {
                // ���������� ���� �� ������ ���������� ��� �������
                if (objects[0].className == "")
                    continue;
                // �������� ����� � �������� ��� �����
                MethodSendFullObject result = this->SendFullObject(methods[i], objects[0], methodCall);
                // ���������� ���� �� ������ ����� �������� �������
                if (result.cppMethod.name == "")
                    continue;
                MethodCall newMethodCall = result.methodCall;
                // ��������� ����� ������
                code.UpdateMethodCallsV2(methods[i], methodCall, newMethodCall);
                // ��������� ������ ���� ���������
                methods = code.methods;
                methods.push_back(result.cppMethod);
                //methods = code.GetMethods();
                code.UpdateMethods(methods);
                code.UpdateTokensV2();
            }
        }
    }
    //code.UpdateMethods(methods);
    code.UpdateTokensV2();
    this->code = code;
}

MethodSendFullObject RefactoringManager::SendFullObject(CppMethod method, Object object, MethodCall methodCall) {
    CppMethod newCppMethod,     // ����� �����
        curCppMethod = method;  // ������� �����
    MethodSendFullObject newCppMethodStruct;
    MethodCall newMethodCall = {};
    std::vector<std::string> values = methodCall.parameterValues;
    int cnt = 0;
    for (std::string value : values)
        if (value.substr(0, object.name.length()) == object.name && 
            value.length() > object.name.length())
            cnt++;
    if (cnt > 1) {
        newCppMethod.type = curCppMethod.type;
        newCppMethod.name = curCppMethod.name + "_Obj_" + object.name;
        int valuesSize = values.size();
        for (int i = 0; i < valuesSize; i++)
            if (values[i].substr(0, object.name.length()) == object.name)
                newCppMethod.name += "_" + values[i].substr(object.name.length() + 1, values[i].length());
        newCppMethod.parameters = {};
        Parameter newParameter = Parameter(object.className, object.name);
        newCppMethod.parameters.push_back(newParameter);
        newMethodCall.methodName = newCppMethod.name;
        newMethodCall.parameterValues.push_back(object.name);
        for (int i = 0; i < valuesSize; i++)
            if (values[i].substr(0, object.name.length()) != object.name) {
                newMethodCall.parameterValues.push_back(values[i]);
                newParameter = curCppMethod.parameters[i];
                newCppMethod.parameters.push_back(newParameter);
            }
        newCppMethod.body = {};
        newCppMethod.body.push_back("{");
        newCppMethod.body.push_back("\n");
        for (int i = 0; i < curCppMethod.parameters.size(); i++) {
            int ucnt = 0;
            for (Parameter parameter : newCppMethod.parameters)
                if (parameter.name == curCppMethod.parameters[i].name &&
                    parameter.type == curCppMethod.parameters[i].type)
                    ucnt++;
            if (ucnt == 0) {
                newCppMethod.body.push_back(curCppMethod.parameters[i].type);
                newCppMethod.body.push_back(curCppMethod.parameters[i].name);
                newCppMethod.body.push_back("=");
                newCppMethod.body.push_back(object.name);
                newCppMethod.body.push_back(".");
                newCppMethod.body.push_back(values[i].substr(object.name.length() + 1, values[i].length()));
                newCppMethod.body.push_back(";");
                newCppMethod.body.push_back("\n");
            }
        }
        newCppMethod.body.pop_back();
        bool isNotFirst = false;
        for (std::string token : method.body) {
            if (isNotFirst)
                newCppMethod.body.push_back(token);
            else {
                isNotFirst = true;
                continue;
            }
        }
        newCppMethod.body.push_back("\n");
        newCppMethod.body.push_back("\n");
    }
    newCppMethodStruct.cppMethod = newCppMethod;
    newCppMethodStruct.methodCall = newMethodCall;
    return newCppMethodStruct;
}

void RefactoringManager::ExtractClassProc(CppCode code) {
    std::vector<CppClass> classes = code.classes;
    std::vector<CppClassMethod> curClassMethods = {};
    int methodsSize = 0,
        classesSize = classes.size(),
        numWidth = 0,
        k = 0,
        num = 0, 
        prevNum = 0;
    std::string selectedMethodsNumsStr = "";
    std::set<int> selectedMethodsNums = {};
    std::vector<std::string> newTokens = {};
    for (int i = 0; i < classesSize; i++) {
        if (classes[i].methods.size() > 1) {
            selectedMethodsNums = {};
            curClassMethods = classes[i].methods;
            methodsSize = curClassMethods.size();
            numWidth = std::to_string(methodsSize).size();
            std::cout << "�������� ������ ��� ����������� � ����� �����," << std::endl;
            std::cout << "��� ������� '0', ����� ���������� ����������� ������ " << classes[i].name << std::endl;
            for (int j = 0; j < methodsSize; j++)
                std::cout << std::setw(numWidth) << (j + 1) << " " << curClassMethods[j].name << std::endl;
            std::cin >> selectedMethodsNumsStr;
            /*string::iterator iteratorRemoveSpaces = remove_if(selectedMethodsNumsStr.begin(), selectedMethodsNumsStr.end(), ' ');
            selectedMethodsNumsStr.erase(iteratorRemoveSpaces, selectedMethodsNumsStr.end());*/
            std::regex extraSpaces("[ ]+"),
                extraCommas("[,]+"),
                extraDashes("[-]+"),
                extraIntervals("[-]{1}[0-9]+[-]{1}"),
                letters("[A-Za-z�-ߨ�-��]+"),
                specSymbols("[._\\/\?+=<>\"\'`~#@$%^&*()]");
            std::regex_replace(selectedMethodsNumsStr, extraSpaces, " ");
            std::regex_replace(selectedMethodsNumsStr, extraCommas, ",");
            std::regex_replace(selectedMethodsNumsStr, extraDashes, "-");
            std::regex_replace(selectedMethodsNumsStr, extraIntervals, "-");
            std::regex_replace(selectedMethodsNumsStr, letters, "");
            std::regex_replace(selectedMethodsNumsStr, specSymbols, "");
            selectedMethodsNums = GetMethodNumbers(selectedMethodsNumsStr);
            if (*selectedMethodsNums.begin() == 0 && selectedMethodsNums.size() == 1)
                continue;
            std::vector<int> vecSelectedMethodsNums(selectedMethodsNums.begin(), selectedMethodsNums.end());
            CppClasses result = ExtractClass(classes[i], vecSelectedMethodsNums);
            //classes[i] = resultClasses.newClassWExtractedMethods;
            classes[i] = result.oldClassWOExtractedMethods;
            //classes[i].UpdateClass();
            //classes.push_back(resultClasses.oldClassWOExtractedMethods);
            classes.push_back(result.newClassWExtractedMethods);
            newTokens = result.newTokens;
        }
    }
    if (newTokens.size() != 0)
        code.tokens = newTokens;
    code = CppCode(code.ToString());
    code.classes = classes;
    code.UpdateTokensV3();
    code.UpdateCode(code.ToString());
    this->code = code;
}

CppClasses RefactoringManager::ExtractClass(CppClass cppClass, std::vector<int> methodsNums) {
    std::vector<CppClassMethod> methods = cppClass.methods,
        oldClassMethodsFin = {},
        newClassMethodsFin = {},
        oldClassMethods = {},
        newClassMethods = {};
    std::vector<CppClassField> fields = cppClass.fields,
        oldClassFieldsFin = {},
        newClassFieldsFin = {},
        oldClassFields = {},
        newClassFields = {};
    std::vector<CppClassConstructor> constructors = cppClass.constructors,
        oldClassConstructorsFin = {},
        newClassConstructorsFin = {},
        oldClassConstructors = {},
        newClassConstructors = {};
    CppClassDestructor destructor = cppClass.destructor,
        oldClassDestructor = {},
        newClassDestructor = {};
    CppClassGraph classGraph = cppClass.classGraph;
    CppClass oldClass = {},
        newClass = {};
    int ind = 0;
    for (int i = fields.size(); i < methods.size() + fields.size(); i++) {
        int ii = i - fields.size();
        for (int methodNum : methodsNums)
            if (i != methodNum) {
                newClassMethods.push_back(methods[ii]);
                for (int j = 0; j < classGraph.totalSize; j++)
                    if (classGraph.cppClassGraph[i][j]) {
                        if (j >= 0 && j < classGraph.fieldsSize)
                            newClassFields.push_back(fields[j]);
                        else {
                            ind = j - classGraph.fieldsSize;
                            if (ind >= 0 && ind < classGraph.methodsSize)
                                newClassMethods.push_back(methods[ind]);
                            else {
                                ind -= classGraph.methodsSize;
                                if (ind >= 0 && ind < classGraph.constructorsSize)
                                    newClassConstructors.push_back(constructors[ind]);
                                else if (ind >= classGraph.constructorsSize)
                                    newClassDestructor = destructor;
                            }
                        }
                    }
            }
            else {
                oldClassMethods.push_back(methods[ii]);
                for (int j = 0; j < classGraph.totalSize; j++)
                    if (classGraph.cppClassGraph[i][j]) {
                        if (j >= 0 && j < classGraph.fieldsSize)
                            oldClassFields.push_back(fields[j]);
                        else {
                            ind = j - classGraph.fieldsSize;
                            if (ind >= 0 && ind < classGraph.methodsSize)
                                oldClassMethods.push_back(methods[ind]);
                            else {
                                ind -= classGraph.methodsSize;
                                if (ind >= 0 && ind < classGraph.constructorsSize)
                                    oldClassConstructors.push_back(constructors[ind]);
                                else if (ind >= classGraph.constructorsSize)
                                    oldClassDestructor = destructor;
                            }
                        }
                    }
            }
    }
    oldClassFieldsFin.assign(oldClassFields.begin(), oldClassFields.end());
    oldClass.fields = oldClassFieldsFin;
    oldClassMethodsFin.assign(oldClassMethods.begin(), oldClassMethods.end());
    oldClass.methods = oldClassMethodsFin;
    oldClass.name = cppClass.name;
    oldClassConstructorsFin.assign(oldClassConstructors.begin(), oldClassConstructors.end());
    oldClass.constructors = oldClassConstructorsFin;
    if (oldClass.constructors.size() == 0)
        oldClass.CreateConstructor();
    oldClass.destructor = oldClassDestructor;
    oldClass.tokens = oldClass.ToTokens();
    newClassFieldsFin.assign(newClassFields.begin(), newClassFields.end());
    newClass.fields = newClassFieldsFin;
    newClassMethodsFin.assign(newClassMethods.begin(), newClassMethods.end());
    newClass.methods = newClassMethodsFin;
    newClass.name = "New_" + cppClass.name;
    newClassConstructorsFin.assign(newClassConstructors.begin(), newClassConstructors.end());
    newClass.constructors = newClassConstructorsFin;
    if (newClass.constructors.size() == 0)
        newClass.CreateConstructor();
    newClass.destructor = newClassDestructor;
    newClass.tokens = newClass.ToTokens();
    std::vector<std::string> tokens = code.tokens,
        newTokens = {};
    for (int i = 0; i < tokens.size(); i++) {
        //newTokens = {};
        if (i < tokens.size() - 4 &&
            tokens[i] == cppClass.name &&
            tokens[i + 2] == "=" &&
            tokens[i + 3] == cppClass.name &&
            tokens[i + 4] == "(") {
            int depth = 0,
                j = 0;
            std::vector<int> commasInds = {};
            for (j = i; j < tokens.size(); j++) {
                if (tokens[j] == "," && depth == 1)
                    commasInds.push_back(j);
                if (tokens[j] == "(")
                    depth++;
                if (tokens[j] == ")") {
                    if (depth > 1)
                        depth--;
                    else
                        break;
                }
            }
            std::vector<std::string> values = {};
            std::string curValue = "";
            for (int k = i + 5; k < j + 1; k++) {
                if (tokens[k] == "," || k == j) {
                    values.push_back(curValue);
                    curValue = "";
                }
                else
                    curValue += tokens[k];
            }
            newTokens.push_back(tokens[i]);
            newTokens.push_back(tokens[i + 1]);
            newTokens.push_back(tokens[i + 2]);
            newTokens.push_back(tokens[i + 3]);
            newTokens.push_back(tokens[i + 4]);
            //newTokens.push_back(tokens[i + 5]);
            int indOld = 0,
                indNew = 0;
            std::vector<std::string> newObjDecl = {};
            newObjDecl.push_back(newClass.name);
            newObjDecl.push_back("New_" + tokens[i + 1]);
            newObjDecl.push_back("=");
            newObjDecl.push_back(newClass.name);
            newObjDecl.push_back("(");
            for (int k = i + 5; k < j; k++) {
                int ind = indOld + indNew;
                if (indOld < oldClass.fields.size() && 
                    ind < cppClass.fields.size() &&
                    cppClass.fields[ind].name == oldClass.fields[indOld].name) {
                    newTokens.push_back(values[ind]);
                    newTokens.push_back(",");
                }
                if (indNew < newClass.fields.size() &&
                    ind < cppClass.fields.size() &&
                    cppClass.fields[ind].name == newClass.fields[indNew].name) {
                    newObjDecl.push_back(values[ind]);
                    newObjDecl.push_back(",");
                }
                if (indOld < oldClass.fields.size() &&
                    ind < cppClass.fields.size() &&
                    cppClass.fields[ind].name == oldClass.fields[indOld].name) {
                    indOld++;
                }
                if (indNew < newClass.fields.size() &&
                    ind < cppClass.fields.size() &&
                    cppClass.fields[ind].name == newClass.fields[indNew].name) {
                    indNew++;
                }
            }
            newObjDecl.pop_back();
            newObjDecl.push_back(")");
            newTokens.pop_back();
            newTokens.push_back(")");
            newTokens.push_back(";");
            newTokens.push_back("\n");
            for (std::string token : newObjDecl)
                newTokens.push_back(token);
            //newTokens.push_back("\n");
            i = j;
        }
        else
            newTokens.push_back(tokens[i]);
    }
    tokens = newTokens;
    newTokens = {};
    std::vector<std::string> objects = {},
        classes = {};
    std::vector<CppClass> cppClasses = {};
    cppClasses.push_back(oldClass);
    cppClasses.push_back(newClass);
    for (int i = 0; i < tokens.size(); i++) {
        for (CppClass cppClass : cppClasses) {
            if (tokens[i] == cppClass.name && 
                tokens[i + 1] != "{" && 
                tokens[i + 1] != "(") {
                objects.push_back(tokens[i + 1]);
                classes.push_back(tokens[i]);
                break;
            }
        }
    }
    for (int i = 0; i < tokens.size(); i++) {
        std::string className = "",
            fieldOrMethod = "";
        bool flag = false,
            flag2 = false;
        for (int l = 0; l < objects.size(); l++) {
            if (tokens[i].substr(0, objects[l].size() + 1) == objects[l] + ".") {
                className = classes[l];
                fieldOrMethod = tokens[i].substr(objects[l].size() + 1, tokens[i].size() - 1);
                CppClass thisClass = {};
                for (CppClass cppClass : cppClasses) {
                    if (cppClass.name == className) {
                        thisClass = cppClass;
                        break;
                    }
                }
                for (CppClassField field : thisClass.fields) {
                    if (field.name == fieldOrMethod) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    for (CppClassMethod method : thisClass.methods) {
                        if (method.name == fieldOrMethod) {
                            flag2 = true;
                            break;
                        }
                    }
                }
            }
        }
        if (!flag && !flag2 && className != "") {
            std::string otherClass = "",
                otherObject = "";
            for (int m = 0; m < classes.size(); m++) {
                if (classes[m] != className) {
                    otherClass = classes[m];
                    otherObject = objects[m];
                    break;
                }
            }
            newTokens.push_back(otherObject + "." + fieldOrMethod);
        }
        else
            newTokens.push_back(tokens[i]);
    }
    CppClasses result = CppClasses(oldClass, newClass, newTokens);
    return result;
}

std::set<int> RefactoringManager::GetMethodNumbers(std::string input) {
    std::set<int> result = {};
    std::stringstream str_strm;
    std::string temp_str;
    int temp_int;
    regex_replace(input, std::regex("[,]{1}"), " , ");
    regex_replace(input, std::regex("[-]{1}"), " - ");
    str_strm << input; //convert the string s into stringstream
    while (!str_strm.eof()) {
        str_strm >> temp_str; //take words into temp_str one by one
        if (std::stringstream(temp_str) >> temp_int) { //try to convert string to int
            result.insert(temp_int);
            str_strm >> temp_str;
            if (temp_str == ",")
                continue;
            else if (temp_str == "-") {
                int start = temp_int;
                str_strm >> temp_str;
                if (std::stringstream(temp_str) >> temp_int) {
                    for (int num = start; num <= temp_int; num++)
                        result.insert(num);
                }
            }
        }
        temp_str = ""; //clear temp string
    }
    return result;
}

/// <summary>����������� �������� �����, � ������� ����� ����������� ���������� ���</summary>
/// <param name="filePath">- ������ �������� ��������� �����</param>
/// <returns>�������� �����, � ������� ����� ����������� ���������� ���</returns>
std::string RefactoringManager::GetOutputFileName(std::string filePath) {
    std::string newFilePath;
    if (filePath.substr(filePath.length() - 4, filePath.length()) == ".cpp")
        newFilePath = filePath.substr(0, filePath.length() - 4) + "_refactored.cpp";
    else if (filePath.substr(filePath.length() - 2, filePath.length()) == ".c")
        newFilePath = filePath.substr(0, filePath.length() - 2) + "_refactored.c";
    return newFilePath;
}

/// <summary>������ �����</summary>
/// <param name="filePath">- ������ �������� �����</param>
void RefactoringManager::GetFileContent(std::string filePath) {
    std::string fileString = ""; // ����������� ������ �� �����
    char buff[2000];    // ����� ��� ������ ������
    std::ifstream fileStream(filePath);  // ����� ������ �� �����
    if (fileStream.is_open())
        while (std::getline(fileStream, fileString))
            this->stringStream << fileString << std::endl;
    else {
        strerror_s(buff, errno);
        std::cerr << "Error: " << buff << std::endl;
    }
    this->fileContent = this->stringStream.str();
    fileStream.close();
}

/// <summary>������ � ����</summary>
/// <param name="filePath">- ������ �������� �����</param>
void RefactoringManager::WriteFile(std::string filePath) {
    std::string outString = "";  // ������, ������������ � ����
    this->outFileStream.open(filePath, std::ios::out);
    this->outStringStream << this->fileContent;
    while (!outStringStream.eof()) {
        std::getline(this->outStringStream, outString);
        this->outFileStream << outString << " " << std::endl;
    }
    this->outFileStream.close();
}

//void RefactoringManager::FormatFile(string filePath) {
//    Formatter formatter;
//    char* textIn = formatter.GetText(filePath);
//}