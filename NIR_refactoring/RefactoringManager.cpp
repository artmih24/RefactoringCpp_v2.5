// ���� RefactoringManager.cpp - ���������� ������� ������ RefactoringManager

#include "RefactoringManager.h"

/// <summary>����������� ������� ������ RefactoringManager</summary>
/// <param name="filePath">- ���� � ����� � �������� �����</param>
RefactoringManager::RefactoringManager(string filePath) {
    this->filePath = filePath;
    this->outFilePath = this->GetOutputFileName(this->filePath);
    this->fileStream = ifstream(this->filePath);
    cout << this->outFilePath << endl;
}

/// <summary>���������� ������������</summary>
void RefactoringManager::Refactoring() {
    this->GetFileContent(this->filePath);
    this->code = CppCode(this->fileContent);
    this->RemoveAssignmentsToParametersProc(this->code);
    this->RemoveParametersProc(this->code);
    this->SendFullObjectProc(this->code);
    this->fileContent = this->code.ToString();
    WriteFile(this->outFilePath);
}

/// <summary>���������� ������������ ������� �������� ������������ ����������</summary>
/// <param name="code">- �������� ��� ���������</param>
void RefactoringManager::RemoveAssignmentsToParametersProc(CppCode code) {
    vector<CppMethod> methods = code.methods;   // ������ ������� ����
    for (int i = 0; i < methods.size(); i++)
        methods[i] = this->RemoveAssignmentsToParameters(methods[i]);
    code.UpdateMethods(methods);
    code.UpdateLexemes();
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
    vector<CppMethod> methods = code.GetMethods();  // ������ ������� ����
    for (int j = 0; j < methods.size(); j++) {
        // ����������� ���������� ������� - �������� ������ ����������
        for (int i = 0; i < methods.size(); i++)
            methods[i] = this->RemoveParameters(methods[i]);
        code.UpdateMethods(methods);
        code.UpdateLexemes();
        this->code = code;
        // ����������� �� ���� ����, ������� � ��������������� 
        // ������ - �������� ��������, �������������� � �������������� ���������
        for (int i = 0; i < methods.size(); i++)
            code.UpdateMethodCalls(methods[i]);
        code.UpdateMethods(methods);
        code.UpdateLexemes();
        this->code = code;
        string codeString = code.ToString();
        code.UpdateCode(codeString);
        code.lexemes = code.GetLexemes();
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
    vector<Parameter> parameters = curCppMethod.parameters; // ������ ���������� ������
    vector<string> body = curCppMethod.body;    // ������� ���� ������
    int parametersSize = parameters.size(), // ���-�� ����������
        bodySize = body.size(), // ���-�� ������ ���� ������
        depth = 0,  // ������� ����������� ������
        k = 0,
        l = 0;
    string parameterValue;
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
    vector<CppMethod> methods = code.methods;   // ������ ������� ����
    int j = 0;
    for (int i = 0; i < methods.size(); i++) {
        vector<MethodCall> methodCalls = code.GetMethodCalls(methods[i]);
        for (MethodCall methodCall : methodCalls) {
            vector<string> values = methodCall.parameterValues;
            vector<Object> objects = {},
                objectsFin = {};
            for (int k = 0; k < values.size(); k++) {
                for (j = 0; j < values[k].size(); j++)
                    if (values[k].find('.') && j < values[k].size())
                        break;
                if (j < values[k].size()) {
                    Object object = {};
                    object.name = values[k].substr(0, values[k].find('.'));
                    vector<string> lexemes = code.lexemes,
                        types = code.GetTypes("class");
                    int lexemesSize = lexemes.size();
                    for (int l = 0; l < lexemesSize; l++) {
                        for (string type : types)
                            if (lexemes[l] == type && lexemes[l + 1] == object.name) {
                                object.className = lexemes[l];
                                break;
                            }
                        if (object.className != "")
                            break;
                    }
                    //object.className = methods[i].parameters[k].type;
                    objects.push_back(object);
                }
            }
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
            if (objectsFin.size() == 1) {
                if (objects[0].className == "")
                    continue;
                MethodSendFullObject result = this->SendFullObject(methods[i], objects[0], methodCall);
                if (result.cppMethod.name == "")
                    continue;
                MethodCall newMethodCall = result.methodCall;
                code.UpdateMethodCallsV2(methods[i], methodCall, newMethodCall);
                methods = code.methods;
                methods.push_back(result.cppMethod);
                //methods = code.GetMethods();
                code.UpdateMethods(methods);
                code.UpdateLexemes();
            }
        }
    }
    //code.UpdateMethods(methods);
    code.UpdateLexemesV2();
    this->code = code;
}

MethodSendFullObject RefactoringManager::SendFullObject(CppMethod method, Object object, MethodCall methodCall) {
    CppMethod newCppMethod,     // ����� �����
        curCppMethod = method;  // ������� �����
    MethodSendFullObject newCppMethodStruct;
    MethodCall newMethodCall = {};
    vector<string> values = methodCall.parameterValues;
    int cnt = 0;
    for (string value : values)
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
        Parameter newParameter = {};
        newParameter.name = object.name;
        newParameter.type = object.className;
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
        for (string lexeme : method.body) {
            if (isNotFirst)
                newCppMethod.body.push_back(lexeme);
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
    vector<CppClass> classes = code.classes;
    vector<CppClassMethod> curClassMethods = {};
    int methodsSize = 0,
        numWidth = 0,
        k = 0,
        num = 0, 
        prevNum = 0;
    string selectedMethodsNumsStr = "";
    set<int> selectedMethodsNums = {};
    CppClasses resultClasses = {};
    for (int i = 0; i < classes.size(); i++) {
        selectedMethodsNums = {};
        curClassMethods = classes[i].methods;
        methodsSize = curClassMethods.size();
        numWidth = to_string(methodsSize).length();
        cout << "Select methods to move in new class" << endl;
        for (int j = 0; j < methodsSize; j++)
            cout << setw(numWidth) << (j + 1) << curClassMethods[j].name << endl;
        cin >> selectedMethodsNumsStr;
        /*string::iterator iteratorRemoveSpaces = remove_if(selectedMethodsNumsStr.begin(), selectedMethodsNumsStr.end(), ' ');
        selectedMethodsNumsStr.erase(iteratorRemoveSpaces, selectedMethodsNumsStr.end());*/
        regex extraSpaces("[ ]+"),
            extraCommas("[,]+"), 
            extraDashes("[-]+"),
            extraIntervals("[-]{1}[0-9]+[-]{1}"),
            letters("[A-Za-z�-ߨ�-��]+"),
            specSymbols("[._\\/\?+=<>\"\'`~#@$%^&*()]");
        regex_replace(selectedMethodsNumsStr, extraSpaces, " ");
        regex_replace(selectedMethodsNumsStr, extraCommas, ",");
        regex_replace(selectedMethodsNumsStr, extraDashes, "-");
        regex_replace(selectedMethodsNumsStr, extraIntervals, "-");
        regex_replace(selectedMethodsNumsStr, letters, "");
        regex_replace(selectedMethodsNumsStr, specSymbols, "");
        for (int j = 0; j < selectedMethodsNumsStr.length(); j++) {
            for (k = j; k < selectedMethodsNumsStr.length(); k++)
                if (selectedMethodsNumsStr.substr(k, k + 1) == "," &&
                    selectedMethodsNumsStr.substr(k, k + 1) == "-")
                    break;
            num = stoi(selectedMethodsNumsStr.substr(j, k - 1));
            if (selectedMethodsNumsStr.substr(j - 1, j) == "-")
                for (int l = prevNum; l < num; l++)
                    selectedMethodsNums.insert(l - 1);
            else
                if (selectedMethodsNumsStr.substr(k, k + 1) == "-")
                    prevNum = num;
            if (num <= methodsSize)
                selectedMethodsNums.insert(num - 1);
            else {
                cout << "Parsed number " << num;
                while (num > methodsSize)
                    num /= 10;
                cout << " was divided by 10 while it was greater than " << methodsSize << " and written like " << num << endl;
                selectedMethodsNums.insert(num - 1);
            }
            if (selectedMethodsNumsStr.substr(k, k + 1) == "-")
                prevNum = num;
            j = k + 1;
        }
        vector<int> vecSelectedMethodsNums(selectedMethodsNums.begin(), selectedMethodsNums.end());
        resultClasses = ExtractClass(classes[i], vecSelectedMethodsNums);
        classes[i] = resultClasses.oldClassWOExtractedMethods;
        classes.push_back(resultClasses.newClassWExtractedMethods);
    }
    code.classes = classes;
}

CppClasses RefactoringManager::ExtractClass(CppClass cppClass, vector<int> methodsNums) {
    vector<CppClassMethod> methods = cppClass.methods,
        oldClassMethodsFin = {},
        newClassMethodsFin = {},
        oldClassMethods = {},
        newClassMethods = {};
    vector<CppClassField> fields = cppClass.fields,
        oldClassFieldsFin = {},
        newClassFieldsFin = {},
        oldClassFields = {},
        newClassFields = {};
    vector<CppClassConstructor> constructors = cppClass.constructors,
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
    CppClasses result = {};
    int ind = 0;
    for (int i = 0; i < methods.size(); i++)
        for (int methodNum : methodsNums)
            if (i == methodNum) {
                newClassMethods.push_back(methods[i]);
                for (int j = 0; j < classGraph.totalSize; j++)
                    if (classGraph.cppClassGraph[i - classGraph.fieldsSize][j]) {
                        if (j >= 0 && j < classGraph.fieldsSize)
                            newClassFields.push_back(fields[j]);
                        else {
                            ind = j - classGraph.fieldsSize;
                            if (ind >= 0 && ind < classGraph.methodsSize)
                                newClassMethods.push_back(methods[j]);
                            else {
                                ind -= classGraph.methodsSize;
                                if (ind >= 0 && ind < classGraph.constructorsSize)
                                    newClassConstructors.push_back(constructors[j]);
                                else if (ind >= classGraph.constructorsSize)
                                    newClassDestructor = destructor;
                            }
                        }
                    }
            }
            else {
                oldClassMethods.push_back(methods[i]);
                for (int j = 0; j < classGraph.totalSize; j++)
                    if (classGraph.cppClassGraph[i - classGraph.fieldsSize][j]) {
                        if (j >= 0 && j < classGraph.fieldsSize)
                            oldClassFields.push_back(fields[j]);
                        else {
                            ind = j - classGraph.fieldsSize;
                            if (ind >= 0 && ind < classGraph.methodsSize)
                                oldClassMethods.push_back(methods[j]);
                            else {
                                ind -= classGraph.methodsSize;
                                if (ind >= 0 && ind < classGraph.constructorsSize)
                                    oldClassConstructors.push_back(constructors[j]);
                                else if (ind >= classGraph.constructorsSize)
                                    oldClassDestructor = destructor;
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
    oldClass.destructor = oldClassDestructor;
    oldClass.classCode = oldClass.ToLexemes();
    newClassFieldsFin.assign(newClassFields.begin(), newClassFields.end());
    newClass.fields = newClassFieldsFin;
    newClassMethodsFin.assign(newClassMethods.begin(), newClassMethods.end());
    newClass.methods = newClassMethodsFin;
    newClass.name = "New_" + cppClass.name;
    newClassConstructorsFin.assign(newClassConstructors.begin(), newClassConstructors.end());
    newClass.constructors = newClassConstructorsFin;
    newClass.destructor = newClassDestructor;
    newClass.classCode = newClass.ToLexemes();
    result.oldClassWOExtractedMethods = oldClass;
    result.newClassWExtractedMethods = newClass;
    return result;
}

/// <summary>����������� �������� �����, � ������� ����� ����������� ���������� ���</summary>
/// <param name="filePath">- ������ �������� ��������� �����</param>
/// <returns>�������� �����, � ������� ����� ����������� ���������� ���</returns>
string RefactoringManager::GetOutputFileName(string filePath) {
    string newFilePath;
    if (filePath.substr(filePath.length() - 4, filePath.length()) == ".cpp")
        newFilePath = filePath.substr(0, filePath.length() - 4) + "_refactored.cpp";
    else if (filePath.substr(filePath.length() - 2, filePath.length()) == ".c")
        newFilePath = filePath.substr(0, filePath.length() - 2) + "_refactored.c";
    return newFilePath;
}

/// <summary>������ �����</summary>
/// <param name="filePath">- ������ �������� �����</param>
void RefactoringManager::GetFileContent(string filePath) {
    string fileString = ""; // ����������� ������ �� �����
    char buff[2000];    // ����� ��� ������ ������
    ifstream fileStream(filePath);  // ����� ������ �� �����
    if (fileStream.is_open())
        while (getline(fileStream, fileString))
            this->stringStream << fileString << endl;
    else {
        strerror_s(buff, errno);
        cerr << "Error: " << buff << endl;
    }
    this->fileContent = this->stringStream.str();
    fileStream.close();
}

/// <summary>������ � ����</summary>
/// <param name="filePath">- ������ �������� �����</param>
void RefactoringManager::WriteFile(string filePath) {
    string outString = "";  // ������, ������������ � ����
    this->outFileStream.open(filePath, ios::out);
    this->outStringStream << this->fileContent;
    while (!outStringStream.eof()) {
        getline(this->outStringStream, outString);
        this->outFileStream << outString << " " << endl;
    }
    this->outFileStream.close();
}

//void RefactoringManager::FormatFile(string filePath) {
//    Formatter formatter;
//    char* textIn = formatter.GetText(filePath);
//}