// файл RefactoringManager.cpp - реализация методов класса RefactoringManager

#include "RefactoringManager.h"

/// <summary>Конструктор объекта класса RefactoringManager</summary>
/// <param name="filePath">- путь к файлу с исходным кодом</param>
RefactoringManager::RefactoringManager(std::string filePath) {
    this->filePath = filePath;
    this->outFilePath = this->GetOutputFileName(this->filePath);
    this->fileStream = std::ifstream(this->filePath);
    std::cout << this->outFilePath << std::endl;
}

/// <summary>Проведение рефакторинга</summary>
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

/// <summary>Проведение рефакторинга методом удаления присваиваний параметрам</summary>
/// <param name="code">- исходный код программы</param>
void RefactoringManager::RemoveAssignmentsToParametersProc(CppCode code) {
    std::vector<CppMethod> methods = code.methods;   // список методов кода
    for (int i = 0; i < methods.size(); i++)
        methods[i] = this->RemoveAssignmentsToParameters(methods[i]);
    code.UpdateMethods(methods);
    code.UpdateTokens();
    this->code = code;
}

/// <summary>Удаление присваиваний параметрам метода</summary>
/// <param name="method">- изначальный метод</param>
/// <returns>новый метод</returns>
CppMethod RefactoringManager::RemoveAssignmentsToParameters(CppMethod method) {
    CppMethod newCppMethod,     // новый метод
        curCppMethod = method;  // текущий метод
    // цикл по всем параметрам
    for (int i = 0; i < curCppMethod.parameters.size(); i++) {
        newCppMethod.body = {};
        newCppMethod.parameters = curCppMethod.parameters;
        newCppMethod.type = curCppMethod.type;
        newCppMethod.name = curCppMethod.name;
        bool assignment = false;    // флаг присваивания переменной
        // проверка тела метода на наличие присваиваний параметру в нем
        for (int j = 0; j < curCppMethod.body.size(); j++)
            // если есть присваивание
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
        // если есть присваивание
        if (assignment) {
            int start = -1;
            for (int j = 0; j < curCppMethod.body.size(); j++) {
                // заменяем его на новую переменную, остальные фрагменты кода записываем как есть
                if (curCppMethod.body[j] == curCppMethod.parameters[i].name)
                    newCppMethod.body.push_back("new_var_" + curCppMethod.body[j]);
                else
                    newCppMethod.body.push_back(curCppMethod.body[j]);
                // в начало тела метода добавляем новую переменную, которой затем присвоим значение параметра
                if ((curCppMethod.body[j] == "{") && (start == -1)) {
                    newCppMethod.body.push_back("\n\t" + curCppMethod.parameters[i].type);
                    newCppMethod.body.push_back("new_var_" + curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back("=");
                    newCppMethod.body.push_back(curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back(";");
                    start = j;
                }
            }
            // запомним метод перед следующей итерацией
            curCppMethod = newCppMethod;
        }
    }
    return curCppMethod;
}

/// <summary>Проведение рефакторинга методом удаления параметров</summary>
/// <param name="code">- исходный код программы</param>
void RefactoringManager::RemoveParametersProc(CppCode code) {
    std::vector<CppMethod> methods = code.GetMethods();  // список методов кода
    for (int j = 0; j < methods.size(); j++) {
        // рефакторинг реализаций методов - удаление лишних параметров
        for (int i = 0; i < methods.size(); i++)
            methods[i] = this->RemoveParameters(methods[i]);
        code.UpdateMethods(methods);
        code.UpdateTokens();
        this->code = code;
        // рефакторинг во всем коде, включая и преобразованные 
        // методы - удаление значений, передававшихся в неиспользуемые параметры
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

/// <summary>Удаление параметров метода</summary>
/// <param name="method">- изначальный метод</param>
/// <returns>новый метод</returns>
CppMethod RefactoringManager::RemoveParameters(CppMethod method) {
    CppMethod newCppMethod,     // новый метод
        curCppMethod = method;  // текущий метод
    std::vector<Parameter> parameters = curCppMethod.parameters; // список параметров метода
    std::vector<std::string> body = curCppMethod.body;    // токены тела метода
    int parametersSize = parameters.size(), // кол-во параметров
        bodySize = body.size(), // кол-во токенов тела метода
        depth = 0,  // уровень вложенности скобок
        k = 0,
        l = 0;
    std::string parameterValue;
    newCppMethod.parameters = {};
    newCppMethod.oldParameters = parameters;
    newCppMethod.body = body;
    newCppMethod.type = curCppMethod.type;
    newCppMethod.name = curCppMethod.name;
    // цикл по всем параметрам
    for (int i = 0; i < parameters.size(); i++) {
        bool usingParameter = false;
        // проверяем, используется ли параметр в теле метода
        for (int j = 0; j < body.size(); j++)
            // если метод реккурсивный - в его теле происходит вызов него же
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
                // ищем параметр в передаваемых параметрах
                for (l = j + 2; l < k + 1; l++)
                    if (body[l] == parameters[i].name) {
                        j = k + 1;
                        break;
                    }
            }
            // если параметр используется
            else if (body[j] == parameters[i].name) {
                usingParameter = true;
                break;
            }
        // если используется, то добавляем его в список параметров нового метода
        if (usingParameter)
            newCppMethod.parameters.push_back(parameters[i]);
        // запомним метод перед следующей итерацией
        curCppMethod = newCppMethod;
    }
    return curCppMethod;
}

void RefactoringManager::SendFullObjectProc(CppCode code) {
    std::vector<CppMethod> methods = code.methods;   // список методов кода
    int j = 0;
    // цикл по всем методам
    for (int i = 0; i < methods.size(); i++) {
        std::vector<MethodCall> methodCalls = code.GetMethodCalls(methods[i]);
        // цикл по всем вызовам метода
        for (MethodCall methodCall : methodCalls) {
            std::vector<std::string> values = methodCall.parameterValues;
            std::vector<Object> objects = {},
                objectsFin = {};
            // цикл по всем значениям параметров
            for (int k = 0; k < values.size(); k++) {
                // поиск полей объектов в параметрах
                for (j = 0; j < values[k].size(); j++)
                    if (values[k].find('.') && j < values[k].size())
                        break;
                // если в параметры передаются поля объекта
                if (j < values[k].size()) {
                    // находим объект
                    Object object = {};
                    object.name = values[k].substr(0, values[k].find('.'));
                    // находим, к какому типу объект относится
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
                                // если смогли найти тип
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
            // создаем список объектов без повторяющихся элементов
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
            // если в параметрах есть поля одного и того же объекта
            if (objectsFin.size() == 1) {
                // пропускаем если не смогли определить тип объекта
                if (objects[0].className == "")
                    continue;
                // копируем метод и получаем его вызов
                MethodSendFullObject result = this->SendFullObject(methods[i], objects[0], methodCall);
                // пропускаем если не смогли найти название объекта
                if (result.cppMethod.name == "")
                    continue;
                MethodCall newMethodCall = result.methodCall;
                // обновляем вызов метода
                code.UpdateMethodCallsV2(methods[i], methodCall, newMethodCall);
                // обновляем данные кода программы
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
    CppMethod newCppMethod,     // новый метод
        curCppMethod = method;  // текущий метод
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
            std::cout << "Выберите методы для перемещения в новый класс," << std::endl;
            std::cout << "или введите '0', чтобы пропустить рефакторинг класса " << classes[i].name << std::endl;
            for (int j = 0; j < methodsSize; j++)
                std::cout << std::setw(numWidth) << (j + 1) << " " << curClassMethods[j].name << std::endl;
            std::cin >> selectedMethodsNumsStr;
            /*string::iterator iteratorRemoveSpaces = remove_if(selectedMethodsNumsStr.begin(), selectedMethodsNumsStr.end(), ' ');
            selectedMethodsNumsStr.erase(iteratorRemoveSpaces, selectedMethodsNumsStr.end());*/
            std::regex extraSpaces("[ ]+"),
                extraCommas("[,]+"),
                extraDashes("[-]+"),
                extraIntervals("[-]{1}[0-9]+[-]{1}"),
                letters("[A-Za-zА-ЯЁа-яё]+"),
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

/// <summary>Составление названия файла, в котором будет содержаться измененный код</summary>
/// <param name="filePath">- полное название исходного файла</param>
/// <returns>название файла, в котором будет содержаться измененный код</returns>
std::string RefactoringManager::GetOutputFileName(std::string filePath) {
    std::string newFilePath;
    if (filePath.substr(filePath.length() - 4, filePath.length()) == ".cpp")
        newFilePath = filePath.substr(0, filePath.length() - 4) + "_refactored.cpp";
    else if (filePath.substr(filePath.length() - 2, filePath.length()) == ".c")
        newFilePath = filePath.substr(0, filePath.length() - 2) + "_refactored.c";
    return newFilePath;
}

/// <summary>Чтение файла</summary>
/// <param name="filePath">- полное название файла</param>
void RefactoringManager::GetFileContent(std::string filePath) {
    std::string fileString = ""; // считываемая строка из файла
    char buff[2000];    // буфер для вывода ошибки
    std::ifstream fileStream(filePath);  // поток чтения из файла
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

/// <summary>Запись в файл</summary>
/// <param name="filePath">- полное название файла</param>
void RefactoringManager::WriteFile(std::string filePath) {
    std::string outString = "";  // строка, записываемая в файл
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