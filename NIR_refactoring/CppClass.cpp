// файл CppClass.cpp - релизация методов класса CppClass

#include "CppClass.h"

CppClass::CppClass() {
	tokens = {};
	possibleTypes = {};
	types = {};
	fields = {};
	methods = {};
	constructors = {};
	destructor = {};
	name = "";
	classGraph = {};
}

CppClass::CppClass(std::vector<std::string> tokens, std::vector<std::string> possibleTypes) {
	this->tokens = tokens;
	name = tokens[1];
	this->possibleTypes = possibleTypes;
	types = {};
	GetFields();
	GetMethods();
	GetConstructors();
	GetDestructor();
	classGraph = GetClassGraph();
}

CppClassGraph CppClass::GetClassGraph() {
	std::vector<std::vector<bool>> classGraphMatrix = {};
	std::vector<bool> classGraphMatrixLine = {};
	int fieldsSize = fields.size(),
		methodsSize = methods.size(),
		constructorsSize = constructors.size(),
		destructorsSize = (destructor.tokens.size() == 0) ? 0 : 1,
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
			if (methods[ind].name.size() < fields[i].value.size() && 
				fields[i].value.substr(0, methods[ind].name.size()) == fields[ind].name) {
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
			for (std::string token : constructors[ind].tokens)
				if (token == fields[j].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
					break;
				}
		for (j; j < fieldsSize + methodsSize; j++) {
			int indj = j - fieldsSize;
			for (std::string token : constructors[ind].tokens)
				if (token == methods[indj].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
					break;
				}
		}
		for (j; j < fieldsSize + methodsSize + constructorsSize; j++) {
			int indj = j - (fieldsSize + methodsSize);
			for (int k = 0; k < constructors[indj].tokens.size(); k++)
				if (constructors[indj].tokens[k] == name &&
					constructors[indj].tokens[k + 1] == "(" &&
					k < constructors[indj].tokens.size() - 1) {
					int parametersSize = 0;
					if (constructors[indj].tokens[k + 2] != ")") {
						parametersSize++;
						int depth = 0;
						for (int l = k + 2; l < constructors[indj].tokens.size(); l++) {
							if (constructors[indj].tokens[l] == "," && 
								depth == 0)
								parametersSize++;
							if (constructors[indj].tokens[l] == "(")
								depth++;
							if (constructors[indj].tokens[l] == ")") {
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
			for (std::string token : destructor.tokens)
				if (token == fields[j].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
				}
		for (j; j < fieldsSize + methodsSize; j++) {
			int indj = j - fieldsSize;
			for (std::string token : destructor.tokens)
				if (token == methods[indj].name) {
					classGraphMatrix[i][j] = true;
					classGraphMatrix[j][i] = true;
					break;
				}
		}
		for (j; j < fieldsSize + methodsSize + constructorsSize; j++) {
			int indj = j - (fieldsSize + methodsSize);
			for (int k = 0; k < destructor.tokens.size(); k++)
				if (destructor.tokens[k] == name &&
					destructor.tokens[k + 1] == "(" &&
					k < destructor.tokens.size() - 1) {
					int parametersSize = 0;
					if (destructor.tokens[k + 2] != ")") {
						parametersSize++;
						int depth = 0;
						for (int l = k + 2; l < destructor.tokens.size(); l++) {
							if (destructor.tokens[l] == "," && depth == 0)
								parametersSize++;
							if (destructor.tokens[l] == "(")
								depth++;
							if (destructor.tokens[l] == ")") {
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
	std::vector<CppClassField> classFields = {},
							   classFieldsFin = {};
	std::vector<std::string> tokens = this->tokens;
	int tokensSize = tokens.size(),
		start = -1,
		j = 0;
	AccessMode fieldAccessMode = AccessMode::Private;
	CppClassField classField = {};
	std::string fieldType = "";
	for (int i = 0; i < tokensSize; i++) {
		std::string fieldName = "";
		fieldAccessMode = AccessMode::Private;
		classField = {};
		int depth = 1;
		for (std::string type : possibleTypes)
			if ((tokens[i] == type) || (tokens[i] == ",")) {
				if (tokens[i] == type) {
					fieldType = tokens[i];
					types.insert(fieldType);
					break;
				}
				else if (tokens[i] == ",") {
					for (j = 0; j < i; j++) {
						if (tokens[j] == "{")
							depth++;
						if (tokens[j] == "}")
							depth--;
					}
					if (depth == 1) {
						while (tokens[j] != fieldType)
							j--;
						fieldType = tokens[j];
					}
					else {
						for (j = i; j < tokensSize; j++) {
							if (tokens[j] == "{")
								depth++;
							if (tokens[j] == "}") {
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
		if (i < (tokensSize - 1) && fieldType != "") {
			for (int k = i; k < tokensSize - 1; k += 3) {
				fieldName = tokens[k + 1];
				if (k < tokensSize - 2 && fieldName == ",")
					fieldName = tokens[k + 2];
				if (k < tokensSize - 3 && fieldName == "\n")
					fieldName = tokens[k + 3];
				if (fieldName == "\n" || 
					fieldName == "," || 
					fieldName == ";" || 
					fieldName == "(" ||
					fieldName == "class" ||
					fieldName == name)
					break;
				j = k;
				fieldAccessMode = AccessMode::Private;
				while (j > 0 && 
					!((tokens[j] == "private:") ||
					(tokens[j] == "public:") ||
					(tokens[j] == "protected:")))
					j--;
				if (tokens[j] == "private:")
					fieldAccessMode = AccessMode::Private;
				else if (tokens[j] == "public:")
					fieldAccessMode = AccessMode::Public;
				else if (tokens[j] == "protected:")
					fieldAccessMode = AccessMode::Protected;
				classField.type = fieldType;
				classField.name = fieldName;
				classField.accessMode = fieldAccessMode;
				classFields.push_back(classField);
			}
		}
	}
	for (CppClassField field : classFields) {
		bool exists = false;
		for (CppClassField finField : classFieldsFin) {
			if (finField.name == field.name) {
				exists = true;
				break;
			}
		}
		if (!exists)
			classFieldsFin.push_back(field);
	}
	this->fields = classFieldsFin;
}

void CppClass::GetMethods() {
	std::vector<CppClassMethod> methods = {};
	std::vector<std::string> tokens = this->tokens,
							 types = possibleTypes;
	int tokensSize = tokens.size(),
		typesSize = types.size();
	CppClassMethod newMethod = {};
	for (int i = 0; i < tokensSize; i++) {
		std::vector<std::string> curMethodTokens = {};
		bool isType = false;
		for (int j = 0; j < typesSize && !isType; j++) {
			isType = ((tokens[i] == types[j]) &&
				(i < tokensSize - 2) &&
				(tokens[i + 2] == "("));
		}
		if (isType) {
			int depth = 1,
				k = 0;
			for (k = i; k < tokensSize; k++) {
				if (tokens[k] == "{")
					depth++;
				if (tokens[k] == "}") {
					if (depth > 2)
						depth--;
					else
						break;
				}
			}
			for (int l = i; l < k; l++) {
				curMethodTokens.push_back(tokens[l]);
			}
			curMethodTokens.push_back("}");
			curMethodTokens.push_back("\n");
			curMethodTokens.push_back("\n");
			newMethod = CppClassMethod(curMethodTokens, this->name, this->tokens, this->fields, this->methods, this->constructors);
			newMethod.accessMode = AccessMode::Private;
			for (int j = i; j >= 0; j--) {
				if (tokens[j] == "private:") {
					newMethod.accessMode = AccessMode::Private;
					break;
				}
				else if (tokens[j] == "public:") {
					newMethod.accessMode = AccessMode::Public;
					break;
				}
				else if (tokens[j] == "protected:") {
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
	std::vector<CppClassConstructor> constructors = {};
	std::vector<std::string> tokens = this->tokens,
							 types = possibleTypes;
	int tokensSize = tokens.size(),
		typesSize = types.size();
	CppClassConstructor newConstructor = {};
	for (int i = 0; i < tokensSize; i++) {
		std::vector<std::string> curMethodTokens = {};
		if (i > 0 && 
			tokens[i - 1] != "class" && 
			tokens[i] == name) {
			int depth = 1,
				k = 0;
			for (k = i; k < tokensSize; k++) {
				if (tokens[k] == "{")
					depth++;
				if (tokens[k] == "}") {
					if (depth > 2)
						depth--;
					else
						break;
				}
			}
			for (int l = i; l < k; l++)
				curMethodTokens.push_back(tokens[l]);
			curMethodTokens.push_back("}");
			curMethodTokens.push_back("\n");
			curMethodTokens.push_back("\n");
			newConstructor.accessMode = AccessMode::Private;
			for (int j = i; j >= 0; j--) {
				if (tokens[j] == "private:") {
					newConstructor.accessMode = AccessMode::Private;
					break;
				}
				else if (tokens[j] == "public:") {
					newConstructor.accessMode = AccessMode::Public;
					break;
				}
				else if (tokens[j] == "protected:") {
					newConstructor.accessMode = AccessMode::Protected;
					break;
				}
			}
			i = k;
			newConstructor.tokens = curMethodTokens;
			constructors.push_back(newConstructor);
		}
	}
	this->constructors = constructors;
}

void CppClass::GetDestructor() {
	std::vector<std::string> tokens = this->tokens,
							 types = possibleTypes;
	int tokensSize = tokens.size(),
		typesSize = types.size();
	for (int i = 0; i < tokensSize; i++) {
		std::vector<std::string> curMethodTokens = {};
		if (i > 0 && tokens[i] == name && 
			(tokens[i - 1] == "~")) {
			int depth = 1,
				k = 0;
			for (k = i; k < tokensSize; k++) {
				if (tokens[k] == "{")
					depth++;
				if (tokens[k] == "}") {
					if (depth > 2)
						depth--;
					else
						break;
				}
			}
			for (int l = i; l < k; l++)
				tokens.push_back(tokens[l]);
			tokens.push_back("}");
			tokens.push_back("\n");
			tokens.push_back("\n");
			CppClassDestructor destructor = {};
			destructor.tokens = tokens;
			destructor.accessMode = AccessMode::Private;
			for (int j = i; j >= 0; j--) {
				if (tokens[j] == "private:") {
					destructor.accessMode = AccessMode::Private;
					break;
				}
				else if (tokens[j] == "public:") {
					destructor.accessMode = AccessMode::Public;
					break;
				}
				else if (tokens[j] == "protected:") {
					destructor.accessMode = AccessMode::Protected;
					break;
				}
			}
			this->destructor = destructor;
			break;
		}
	}
}

void CppClass::CreateConstructor() {
	if (this->constructors.size() == 0) {
		CppClassConstructor newConstructor = {};
		newConstructor.accessMode = AccessMode::Public;
		newConstructor.parameters = {};
		for (CppClassField field : fields) {
			Parameter newParameter = Parameter(field.type, field.name);
			newConstructor.parameters.push_back(newParameter);
		}
		newConstructor.tokens = {};
		newConstructor.tokens.push_back(name);
		newConstructor.tokens.push_back("(");
		for (Parameter parameter : newConstructor.parameters) {
			newConstructor.tokens.push_back(parameter.type);
			newConstructor.tokens.push_back(parameter.name);
			newConstructor.tokens.push_back(",");
		}
		newConstructor.tokens.pop_back();
		newConstructor.tokens.push_back(")");
		newConstructor.tokens.push_back("{");
		newConstructor.tokens.push_back("\n");
		for (CppClassField field : fields) {
			newConstructor.tokens.push_back("this->");
			newConstructor.tokens.push_back(field.name);
			newConstructor.tokens.push_back("=");
			newConstructor.tokens.push_back(field.name);
			newConstructor.tokens.push_back(";");
			newConstructor.tokens.push_back("\n");
		}
		newConstructor.tokens.push_back("}");
		newConstructor.tokens.push_back("\n");
		this->constructors.push_back(newConstructor);
	}
}

void CppClass::CreateConstructor(std::string className) {
	if (this->constructors.size() == 0) {
		CppClassConstructor newConstructor = {};
		newConstructor.accessMode = AccessMode::Public;
		newConstructor.parameters = {};
		for (CppClassField field : fields) {
			Parameter newParameter = Parameter(field.type, field.name);
			newConstructor.parameters.push_back(newParameter);
		}
		newConstructor.tokens = {};
		newConstructor.tokens.push_back(name);
		newConstructor.tokens.push_back("(");
		for (Parameter parameter : newConstructor.parameters) {
			newConstructor.tokens.push_back(parameter.type);
			newConstructor.tokens.push_back(parameter.name);
			newConstructor.tokens.push_back(",");
		}
		newConstructor.tokens.pop_back();
		newConstructor.tokens.push_back(")");
		newConstructor.tokens.push_back("{");
		newConstructor.tokens.push_back("\n");
		for (CppClassField field : fields) {
			newConstructor.tokens.push_back("this->");
			newConstructor.tokens.push_back(field.name);
			newConstructor.tokens.push_back("=");
			if (field.type == className) {
				newConstructor.tokens.push_back(className);
				newConstructor.tokens.push_back("(");
				newConstructor.tokens.push_back(field.name);
				newConstructor.tokens.push_back(")");
			}
			else
				newConstructor.tokens.push_back(field.name);
			newConstructor.tokens.push_back(";");
			newConstructor.tokens.push_back("\n");
		}
		newConstructor.tokens.push_back("}");
		newConstructor.tokens.push_back("\n");
		this->constructors.push_back(newConstructor);
	}
}

void CppClass::UpdateClass() {
	tokens = ToTokens();
	GetFields();
	GetMethods();
	GetConstructors();
	GetDestructor();
	classGraph = GetClassGraph();
}

void CppClass::UpdateFields() {
	tokens = ToTokens();
	GetFields();
}

void CppClass::UpdateMethods() {
	tokens = ToTokens();
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

std::vector<std::string> CppClass::GetTokens(AccessMode accessMode) {
	std::vector<std::string> tokens = {};
	for (CppClassField field : fields)
		if (field.accessMode == accessMode) {
			tokens.push_back("\n");
			tokens.push_back(field.type);
			tokens.push_back(field.name);
			if (field.value != "") {
				tokens.push_back("=");
				tokens.push_back(field.value);
			}
			tokens.push_back(";");
		}
	if (fields.size() != 0)
		tokens.push_back("\n");
	for (CppClassConstructor constructor : constructors)
		if (constructor.accessMode == accessMode) {
			for (std::string token : constructor.tokens)
				tokens.push_back(token);
		}
	for (CppClassMethod method : methods)
		if (method.accessMode == accessMode) {
			tokens.push_back(method.type);
			tokens.push_back(method.name);
			tokens.push_back("(");
			for (Parameter parameter : method.parameters) {
				tokens.push_back(parameter.type);
				if (parameter.pointer)
					tokens.push_back("*");
				tokens.push_back(parameter.name);
				if (parameter.defaultValue != "") {
					tokens.push_back("=");
					tokens.push_back(parameter.defaultValue);
				}
				if (parameter.name != method.parameters[method.parameters.size() - 1].name)
					tokens.push_back(",");
			}
			tokens.push_back(")");
			for (std::string token : method.body)
				tokens.push_back(token);
			//tokens.push_back("\n");
		}
	if (destructor.accessMode == accessMode) {
		if (destructor.tokens.size() != 0) {
			tokens.push_back("~");
			tokens.push_back(name);
			tokens.push_back("(");
			tokens.push_back(")");
			for (std::string token : destructor.tokens)
				tokens.push_back(token);
		}
	}
	if (tokens[tokens.size() - 1] == "\n" && tokens[tokens.size() - 2] == "\n")
		tokens.pop_back();
	return tokens;
}

std::vector<std::string> CppClass::ToTokens() {
	std::vector<std::string> tokens = {},
							 newTokens = {};
	tokens.push_back("class");
	tokens.push_back(name);
	tokens.push_back("{");
	tokens.push_back("\n");
	bool publicFlag = GetFlag(AccessMode::Public),
		 privateFlag = GetFlag(AccessMode::Private),
		 protectedFlag = GetFlag(AccessMode::Protected);
	if (publicFlag || privateFlag || protectedFlag) {
		if (publicFlag) {
			newTokens = GetTokens(AccessMode::Public);
			if (newTokens.size() != 0) {
				tokens.push_back("public:");
				for (std::string token : newTokens)
					tokens.push_back(token);
			}
		}
		if (privateFlag) {
			newTokens = GetTokens(AccessMode::Private);
			if (newTokens.size() != 0) {
				tokens.push_back("private:");
				for (std::string token : newTokens)
					tokens.push_back(token);
			}
		}
		if (protectedFlag) {
			newTokens = GetTokens(AccessMode::Protected);
			if (newTokens.size() != 0) {
				tokens.push_back("protected:");
				for (std::string token : newTokens)
					tokens.push_back(token);
			}
		}
	}
	else {
		newTokens = GetTokens(AccessMode::Private);
		if (newTokens.size() != 0) {
			tokens.push_back("private:");
			for (std::string token : newTokens)
				tokens.push_back(token);
		}
	}
	tokens.push_back("}");
	//tokens.push_back(";");
	tokens.push_back("\n");
	return tokens;
}
