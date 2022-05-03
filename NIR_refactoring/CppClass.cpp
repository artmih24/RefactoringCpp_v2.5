// файл CppClass.cpp - релизация методов класса CppClass

#include "CppClass.h"

CppClass::CppClass() {
	classCode = {};
	possibleTypes = {};
	types = {};
	fields = {};
	methods = {};
	constructors = {};
	destructor = {};
	name = "";
	classGraph = {};
}

CppClass::CppClass(vector<string> lexemes, vector<string> possibleTypes) {
	classCode = lexemes;
	name = lexemes[0];
	this->possibleTypes = possibleTypes;
	types = {};
	GetFields();
	GetMethods();
	GetConstructors();
	GetDestructor();
	classGraph = GetClassGraph();
}

CppClassGraph CppClass::GetClassGraph() {
	vector<vector<bool>> classGraphMatrix = {};
	vector<bool> classGraphMatrixLine = {};
	int fieldsSize = fields.size(),
		methodsSize = methods.size(),
		constructorsSize = constructors.size(),
		destructorsSize = (destructor.destructorCode.size() == 0) ? 0 : 1,
		fieldsAndMethodsSize = fieldsSize + methodsSize + constructorsSize + destructorsSize;
	for (int i = 0; i < fieldsAndMethodsSize; i++)
		classGraphMatrixLine.push_back(false);
	for (int i = 0; i < fieldsAndMethodsSize; i++)
		classGraphMatrix.push_back(classGraphMatrixLine);
	int i = 0,
		j = 0;
	for (i = 0; i < fieldsSize; i++) {
		for (j = 0; j < fieldsSize; j++) {
			if (fields[i].value == fields[j].name) {
				classGraphMatrix[i][j] = true;
				classGraphMatrix[j][i] = true;
			}
			for (int k = 0; k < fields[i].value.size(); k++)
				if (fields[i].value.substr(k, k + fields[j].name.size()) == fields[j].name &&
					k < fields[i].value.size() - fields[j].name.size()) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
				}
		}
		for (j; j < fieldsSize + methodsSize; j++) {
			int ind = j - fieldsSize;
			if (fields[i].value.substr(0, methods[ind].name.size()) == fields[ind].name) {
				classGraphMatrix[i][j] = true;
				classGraphMatrix[j][i] = true;
			}
		}
		for (j; j < fieldsSize + methodsSize + constructorsSize; j++)
			if (fields[i].value.substr(0, name.size()) == name) {
				int parametersSize = 0,
					ind = j - (fieldsSize + methodsSize);
				if (fields[i].value.substr(0, name.size() + 2) != name + "()") {
					parametersSize++;
					for (int k = 0; k < fields[i].value.size(); k++) 
						if (fields[i].value.substr(k, k + 1) == "," &&
							k < fields[i].value.size() - 1)
							parametersSize++;
				}
				if (constructors[ind].parameters.size() == parametersSize) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
				}
			}
	}
	for (i; i < fieldsSize + methodsSize; i++) {
		int ind = i - fieldsSize;
		for (j = 0; j < fieldsSize; j++)
			for (int k = 0; k < methods[ind].usingFields.size(); k++)
				if (fields[j].name == methods[ind].usingFields[k]) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
				}
		for (j; j < fieldsSize + methodsSize; j++) {
			int indj = j - fieldsSize;
			for (int k = 0; k < methods[ind].usingMethods.size(); k++)
				if (methods[indj].name == methods[ind].usingMethods[k].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
				}
		}
		for (j; j < fieldsSize + methodsSize + constructorsSize; j++) {
			int indj = j - fieldsSize - methodsSize;
			for (int k = 0; k < methods[ind].usingConstructors.size(); k++)
				if (constructors[indj].parameters.size() == methods[ind].usingConstructors[k].parameters.size() &&
						(constructors[indj].parameters[0].name == methods[ind].usingConstructors[k].parameters[0].name &&
						constructors[indj].parameters[0].type == methods[ind].usingConstructors[k].parameters[0].type &&
						constructors[indj].parameters.size() > 1)) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
				}
		}
	}
	for (i; i < fieldsSize + methodsSize + constructorsSize; i++) {
		int ind = i - (fieldsSize + methodsSize);
		for (j = 0; j < fieldsSize; j++)
			for (string lexeme : constructors[ind].constructorCode)
				if (lexeme == fields[j].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
					break;
				}
		for (j; j < fieldsSize + methodsSize; j++) {
			int indj = j - fieldsSize;
			for (string lexeme : constructors[ind].constructorCode)
				if (lexeme == methods[indj].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
					break;
				}
		}
		for (j; j < fieldsSize + methodsSize + constructorsSize; j++) {
			int indj = j - (fieldsSize + methodsSize);
			for (int k = 0; k < constructors[indj].constructorCode.size(); k++)
				if (constructors[indj].constructorCode[k] == name &&
					constructors[indj].constructorCode[k + 1] == "(" &&
					k < constructors[indj].constructorCode.size() - 1) {
					int parametersSize = 0;
					if (constructors[indj].constructorCode[k + 2] != ")") {
						parametersSize++;
						int depth = 0;
						for (int l = k + 2; l < constructors[indj].constructorCode.size(); l++) {
							if (constructors[indj].constructorCode[l] == "," && depth == 0)
								parametersSize++;
							if (constructors[indj].constructorCode[l] == "(")
								depth++;
							if (constructors[indj].constructorCode[l] == ")") {
								if (depth > 1)
									depth--;
								else
									break;
							}
						}
						if (constructors[ind].parameters.size() == parametersSize) {
							classGraphMatrix[i][j] = true;
							classGraphMatrix[j][i] = true;
						}
					}
				}
		}
	}
	if (destructorsSize != 0) {
		int ind = i - (fieldsSize + methodsSize + constructorsSize);
		for (j = 0; j < fieldsSize; j++)
			for (string lexeme : destructor.destructorCode)
				if (lexeme == fields[j].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
				}
		for (j; j < fieldsSize + methodsSize; j++) {
			int indj = j - fieldsSize;
			for (string lexeme : destructor.destructorCode)
				if (lexeme == methods[indj].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
					break;
				}
		}
		for (j; j < fieldsSize + methodsSize + constructorsSize; j++) {
			int indj = j - (fieldsSize + methodsSize);
			for (int k = 0; k < destructor.destructorCode.size(); k++)
				if (destructor.destructorCode[k] == name &&
					destructor.destructorCode[k + 1] == "(" &&
					k < destructor.destructorCode.size() - 1) {
					int parametersSize = 0;
					if (destructor.destructorCode[k + 2] != ")") {
						parametersSize++;
						int depth = 0;
						for (int l = k + 2; l < destructor.destructorCode.size(); l++) {
							if (destructor.destructorCode[l] == "," && depth == 0)
								parametersSize++;
							if (destructor.destructorCode[l] == "(")
								depth++;
							if (destructor.destructorCode[l] == ")") {
								if (depth > 1)
									depth--;
								else
									break;
							}
						}
						if (constructors[indj].parameters.size() == parametersSize) {
							classGraphMatrix[i][j] = true;
							classGraphMatrix[j][i] = true;
						}
					}
				}
		}
	}
	CppClassGraph classGraph = {};
	classGraph.cppClassGraph = classGraphMatrix;
	classGraph.totalSize = fieldsAndMethodsSize;
	classGraph.fieldsSize = fieldsSize;
	classGraph.methodsSize = methodsSize;
	classGraph.constructorsSize = constructorsSize;
	classGraph.destructorsSize = destructorsSize;
	return classGraph;
}

void CppClass::GetFields() {
	vector<CppClassField> classFields = {};
	vector<string> lexemes = this->classCode;
	int lexemesSize = lexemes.size(),
		start = -1,
		j = 0;
	AccessMode fieldAccessMode = AccessMode::Null;
	CppClassField classField = {};
	for (int i = 0; i < lexemesSize; i++) {
		string fieldType = "",
			fieldName = "";
		fieldAccessMode = AccessMode::Null;
		classField = {};
		int depth = 1;
		for (string type : possibleTypes)
			if ((lexemes[i] == type) || (lexemes[i] == ",")) {
				if (lexemes[i] == type) {
					fieldType = lexemes[i];
					types.insert(fieldType);
					break;
				}
				else if (lexemes[i] == ",") {
					for (j = 0; j < i; j++) {
						if (lexemes[j] == "{")
							depth++;
						if (lexemes[j] == "}")
							depth--;
					}
					if (depth == 1) {
						while (lexemes[j] != type)
							j--;
						fieldType = lexemes[j];
					}
					else {
						for (j = i; j < lexemesSize; j++) {
							if (lexemes[j] == "{")
								depth++;
							if (lexemes[j] == "}") {
								if (depth > 2)
									depth--;
								else
									break;
							}
						}
						i = j;
						break;
					}
				}
			}
		if (fieldType != "") {
			fieldName = lexemes[i + 1];
			j = i;
			while (!((lexemes[j] == "private") ||
				(lexemes[j] == "public") ||
				(lexemes[j] == "protected")))
				j--;
			if (lexemes[j] == "private")
				fieldAccessMode = AccessMode::Private;
			else if (lexemes[j] == "public")
				fieldAccessMode = AccessMode::Public;
			else if (lexemes[j] == "protected")
				fieldAccessMode = AccessMode::Protected;
			classField.type = fieldType;
			classField.name = fieldName;
			classField.accessMode = fieldAccessMode;
			classFields.push_back(classField);
		}
	}
	this->fields = classFields;
}

void CppClass::GetMethods() {
	vector<CppClassMethod> methods = {};
	vector<string> lexemes = this->classCode,
		types = possibleTypes;
	int lexemesSize = lexemes.size(),
		typesSize = types.size();
	CppClassMethod newMethod = {};
	for (int i = 0; i < lexemesSize; i++) {
		vector<string> curMethodLexemes = {};
		bool isType = false;
		for (int j = 0; j < typesSize && !isType; j++) {
			isType = ((lexemes[i] == types[j]) &&
				(i < lexemesSize - 2) &&
				(lexemes[i + 2] == "("));
		}
		if (isType) {
			int depth = 1,
				k = 0;
			for (k = i; k < lexemesSize; k++) {
				if (lexemes[k] == "{")
					depth++;
				if (lexemes[k] == "}") {
					if (depth > 2)
						depth--;
					else
						break;
				}
			}
			for (int l = i; l < k; l++) {
				curMethodLexemes.push_back(lexemes[l]);
			}
			curMethodLexemes.push_back("}");
			newMethod = CppClassMethod(curMethodLexemes, this->name, this->classCode, this->fields, this->methods, this->constructors);
			for (int j = i; j >= 0; j--) {
				if (lexemes[j] == "private") {
					newMethod.accessMode = AccessMode::Private;
					break;
				}
				else if (lexemes[j] == "public") {
					newMethod.accessMode = AccessMode::Public;
					break;
				}
				else if (lexemes[j] == "protected") {
					newMethod.accessMode = AccessMode::Protected;
					break;
				}
			}
			i = k;
			methods.push_back(newMethod);
		}
	}
	this->methods = methods;
}

void CppClass::GetConstructors() {
	vector<CppClassConstructor> constructors = {};
	vector<string> lexemes = this->classCode,
		types = possibleTypes;
	int lexemesSize = lexemes.size(),
		typesSize = types.size();
	CppClassConstructor newConstructor = {};
	for (int i = 0; i < lexemesSize; i++) {
		vector<string> curMethodLexemes = {};
		if (lexemes[i] == name) {
			int depth = 1,
				k = 0;
			for (k = i; k < lexemesSize; k++) {
				if (lexemes[k] == "{")
					depth++;
				if (lexemes[k] == "}") {
					if (depth > 2)
						depth--;
					else
						break;
				}
			}
			for (int l = i; l < k; l++)
				curMethodLexemes.push_back(lexemes[l]);
			curMethodLexemes.push_back("}");
			for (int j = i; j >= 0; j--) {
				if (lexemes[j] == "private") {
					newConstructor.accessMode = AccessMode::Private;
					break;
				}
				else if (lexemes[j] == "public") {
					newConstructor.accessMode = AccessMode::Public;
					break;
				}
				else if (lexemes[j] == "protected") {
					newConstructor.accessMode = AccessMode::Protected;
					break;
				}
			}
			i = k;
			newConstructor.constructorCode = curMethodLexemes;
			constructors.push_back(newConstructor);
		}
	}
	this->constructors = constructors;
}

void CppClass::GetDestructor() {
	vector<string> destructorCode = {};
	vector<string> lexemes = this->classCode,
		types = possibleTypes;
	int lexemesSize = lexemes.size(),
		typesSize = types.size();
	for (int i = 0; i < lexemesSize; i++) {
		vector<string> curMethodLexemes = {};
		if (lexemes[i] == name && 
			(lexemes[i - 1] == "~" && i > 0)) {
			int depth = 1,
				k = 0;
			for (k = i; k < lexemesSize; k++) {
				if (lexemes[k] == "{")
					depth++;
				if (lexemes[k] == "}") {
					if (depth > 2)
						depth--;
					else
						break;
				}
			}
			for (int l = i; l < k; l++)
				destructorCode.push_back(lexemes[l]);
			destructorCode.push_back("}");
			CppClassDestructor destructor = {};
			destructor.destructorCode = destructorCode;
			for (int j = i; j >= 0; j--) {
				if (lexemes[j] == "private") {
					destructor.accessMode = AccessMode::Private;
					break;
				}
				else if (lexemes[j] == "public") {
					destructor.accessMode = AccessMode::Public;
					break;
				}
				else if (lexemes[j] == "protected") {
					destructor.accessMode = AccessMode::Protected;
					break;
				}
			}
			this->destructor = destructor;
			break;
		}
	}
}

void CppClass::UpdateClass() {
	classCode = ToLexemes();
	GetFields();
	GetMethods();
	GetConstructors();
	GetDestructor();
	classGraph = GetClassGraph();
}

void CppClass::UpdateFields() {
	classCode = ToLexemes();
	GetFields();
}

void CppClass::UpdateMethods() {
	classCode = ToLexemes();
	GetMethods();
}

bool CppClass::GetFlag(AccessMode accessMode) {
	for (CppClassField field : fields)
		if (field.accessMode == accessMode)
			return true;
	for (CppClassMethod method : methods)
		if (method.accessMode == accessMode)
			return true;
	for (CppClassConstructor constructor : constructors)
		if (constructor.accessMode == accessMode)
			return true;
	if (destructor.accessMode == accessMode)
		return true;
	return false;
}

vector<string> CppClass::GetLexemes(AccessMode accessMode) {
	vector<string> lexemes = {};
	for (CppClassField field : fields)
		if (field.accessMode == accessMode) {
			lexemes.push_back(field.type);
			lexemes.push_back(field.name);
			if (field.value != "") {
				lexemes.push_back("=");
				lexemes.push_back(field.value);
			}
		}
	for (CppClassConstructor constructor : constructors)
		if (constructor.accessMode == accessMode) {
			lexemes.push_back(name);
			lexemes.push_back("(");
			for (Parameter parameter : constructor.parameters) {
				lexemes.push_back(parameter.type);
				if (parameter.pointer)
					lexemes.push_back("*");
				lexemes.push_back(parameter.name);
				if (parameter.defaultValue != "") {
					lexemes.push_back("=");
					lexemes.push_back(parameter.defaultValue);
				}
				if (parameter.name != constructor.parameters[constructor.parameters.size() - 1].name)
					lexemes.push_back(",");
			}
			lexemes.push_back(")");
			for (string lexeme : constructor.constructorCode)
				lexemes.push_back(lexeme);
		}
	for (CppClassMethod method : methods)
		if (method.accessMode == accessMode) {
			lexemes.push_back(method.type);
			lexemes.push_back(method.name);
			lexemes.push_back("(");
			for (Parameter parameter : method.parameters) {
				lexemes.push_back(parameter.type);
				if (parameter.pointer)
					lexemes.push_back("*");
				lexemes.push_back(parameter.name);
				if (parameter.defaultValue != "") {
					lexemes.push_back("=");
					lexemes.push_back(parameter.defaultValue);
				}
				if (parameter.name != method.parameters[method.parameters.size() - 1].name)
					lexemes.push_back(",");
			}
			lexemes.push_back(")");
			for (string lexeme : method.body)
				lexemes.push_back(lexeme);
		}
	if (destructor.accessMode == accessMode) {
		lexemes.push_back("~");
		lexemes.push_back(name);
		lexemes.push_back("(");
		lexemes.push_back(")");
		for (string lexeme : destructor.destructorCode)
			lexemes.push_back(lexeme);
	}
	return lexemes;
}

vector<string> CppClass::ToLexemes() {
	vector<string> lexemes = {},
		newLexemes = {};
	lexemes.push_back("class");
	lexemes.push_back(name);
	lexemes.push_back("{");
	bool publicFlag = GetFlag(AccessMode::Public),
		privateFlag = GetFlag(AccessMode::Private),
		protectedFlag = GetFlag(AccessMode::Protected);
	if (publicFlag) {
		lexemes.push_back("public:");
		newLexemes = GetLexemes(AccessMode::Public);
		for (string lexeme : newLexemes)
			lexemes.push_back(lexeme);
	}
	if (privateFlag) {
		lexemes.push_back("private:");
		newLexemes = GetLexemes(AccessMode::Private);
		for (string lexeme : newLexemes)
			lexemes.push_back(lexeme);
	}
	if (protectedFlag) {
		lexemes.push_back("protected:");
		newLexemes = GetLexemes(AccessMode::Protected);
		for (string lexeme : newLexemes)
			lexemes.push_back(lexeme);
	}
	lexemes.push_back("}");
	return lexemes;
}
