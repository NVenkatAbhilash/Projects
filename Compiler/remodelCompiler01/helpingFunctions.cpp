#include "Header.h"

void beutifyBuffer(char *buffer){
	int modifyPosition = 0;
	int currentPosition = 0;
	while (buffer[currentPosition] == ' ' || buffer[currentPosition] == '\t')
		currentPosition++;
	while (buffer[currentPosition] != '\0' && buffer[currentPosition] != '\n'){
		if (buffer[currentPosition] == ' '&& buffer[currentPosition + 1] == ' ')
			currentPosition++;
		else if (buffer[currentPosition] == ' '&&buffer[currentPosition + 1] == ',')
			currentPosition++;
		else if (currentPosition!=0 && buffer[currentPosition - 1] == ','&& buffer[currentPosition+1] == ' ')
			currentPosition++;
		else if (buffer[currentPosition] == ' '&& buffer[currentPosition + 1] == ' ')
			currentPosition++;
		else{
			buffer[modifyPosition] = buffer[currentPosition];
			modifyPosition++;
			currentPosition++;
		}
	}
	buffer[modifyPosition] = '\0';
}

int isStringInBuffer(char *buffer, char search[30]){
	int count1, count2, i, j, flag;
	count2 = strlen(search);
	count1 = strlen(buffer);
	for (i = 0; i <= count1 - count2; i++)
	{
		for (j = i; j < i + count2; j++)
		{
			flag = 1;
			if (buffer[j] != search[j - i])
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	return flag;
}

void updateSymbolTable(char *buffer, struct multipleSymbolTable *symbolData, struct memory *memoryData, struct manageStruct *manageData,  int dataAddress){
	char command[10],parameter[5];
	int position = 0;
	char name[5];
	int size = 1;
	sscanf(buffer, "%s %s", command, parameter);
	if (strcmp(command, "CONST") == 0){
		memoryData->memoryArray[dataAddress] = 0;
		size = 0;
	}
	else if (isStringInBuffer(buffer, "[")){
		size = getValueInBraces(buffer);
	}
	//not considered more than 100 values
	int index = manageData->countSymbolRows - 1;
	if (index < 0)
		index = 0;
	manageData->countSymbolRows = index + 2;
	while (parameter[position] != '[' &&parameter[position] != '\n' && parameter[position]!='\0')
	{
		name[position] = parameter[position];
		position++;
	}
	name[position] = '\0';
	strcpy(symbolData->symbolArray[index].symbolName, name);
	symbolData->symbolArray[index].size = size;
	symbolData->symbolArray[index].address = dataAddress;
}


int getValueInBraces(char *buffer){
	int position = 0;
	while (buffer[position] != '[')
		position++;
	//only for single digit
	return buffer[position + 1] - '0';
}


void initialiseRegister(struct manageStruct *manageData){
	int i;
	manageData->countMemoryBlocks = 0;
	manageData->countRowAddress = 0;
	manageData->countSymbolRows = 0;
	for (i = 0; i < 7; i++){
		manageData->registerCodes[i] = i + 1;
	}
}

void performReadOp(char *parameter,  int *interLang){
	interLang[1] = 14;
	if (parameter[1] == 'X'){
		interLang[2] = parameter[0] - 'A';
	}
	interLang[3] = -1;
}

bool writeToFile(int *interLang, char *outputFileName){
	FILE *f = fopen(outputFileName, "a");
	int position = 0;
	fseek(f, 0, SEEK_END);
	if (interLang[0] == -1)
		return false;
	while (interLang[position] != -1){
		fprintf(f, "%4d", interLang[position]);
		position++;
	}
	fprintf(f, "\n");
	fclose(f);
	return true;
}

bool isRegister(char *identifier){
	int len = strlen(identifier);
	if (identifier[len - 1] == 'X')
		return true;
	return false;
}

int getIdentifierValue(char *parameter, struct multipleSymbolTable *symbolData, struct manageStruct *manageData){
	int i;
	int position = 0;
	if (isRegister(parameter))
		return parameter[0] - 'A';
	while (position != '[' && position!='\0')
		position++;
	for (i = 0; i < manageData->countSymbolRows; i++){
		if (strcmp(parameter, symbolData->symbolArray[i].symbolName) == 0)
			return i;
	}
}

void getParameterInIf(char *buffer, char *parameter, int number){
	int position = 0;
	while (number > 1){
		if (buffer[0] == ' ')
			number--;
		buffer = buffer + 1;
	}
	while (buffer[0] == ' ')
		buffer = buffer + 1;
	while (buffer[0] != ' '  && buffer[0] != '\n'&& buffer[0] != '\0'&&buffer[0] != ','){
		parameter[position] = buffer[0];
		buffer = buffer + 1;
		position++;
	}
	if (parameter[position - 1] == ' ')
		position--;
	parameter[position] = '\0';
}

void getParameter(char *buffer, char *parameter,int number){
	int position = 0;
	while (number > 1){
		if (buffer[0] == ',')
			number--;
		buffer = buffer + 1;
	}
	while (buffer[0] == ' ')
		buffer = buffer + 1;
	while (buffer[0] != ' '  && buffer[0] != '\n'&& buffer[0] != '\0'&&buffer[0] != ','){
		parameter[position] = buffer[0];
		buffer = buffer + 1;
		position++;
	}
	if (parameter[position - 1] == ' ')
		position--;
	parameter[position] = '\0';
}

void performMovOp(char *parameters, struct multipleSymbolTable *symbolData, struct memory *memoryData, struct manageStruct *manageData, int *interLang){
	char parameterOne[5], parameterTwo[5];
	getParameter(parameters, parameterOne, 1);
	getParameter(parameters, parameterTwo, 2);
	if (isRegister(parameterOne)){
		interLang[1] = 2;
	}
	else
		interLang[1] = 1;
	interLang[2] = getIdentifierValue(parameterOne, symbolData, manageData);
	interLang[3] = getIdentifierValue(parameterTwo, symbolData, manageData);
	interLang[4] = -1;
}

void performArthiOp(char *buffer, struct multipleSymbolTable *symbolData, struct memory *memoryData, struct manageStruct *manageData, int *interLang){
	char command[40], parameterOne[5], parameterTwo[5], parameterThree[5];
	//interLang[2] = getIdentifierValue(buffer, symbolData, manageData);
	sscanf(buffer, "%s", command);
	buffer = buffer + 4;
	if (strcmp(command, "ADD") == 0){
		interLang[1] = 3;
	}
	else if (strcmp(command, "MUL") == 0){
		interLang[1] = 5;
	}
	else{
		interLang[1] = 4;
	}
	getParameter(buffer, parameterOne, 1);
	getParameter(buffer, parameterTwo, 2);
	getParameter(buffer, parameterThree, 3);
	interLang[2] = parameterOne[0] - 'A';
	interLang[3] = parameterTwo[0] - 'A';
	interLang[4] = parameterThree[0] - 'A';
	interLang[5] = -1;
}

void performPrintOp(char *parameter, struct multipleSymbolTable *symbolData, struct manageStruct *manageData, int *interLang){
	parameter[1] = '\0';
	interLang[1] = 13;
	interLang[2] = getIdentifierValue(parameter, symbolData, manageData);
	interLang[3] = -1;
}

int getBlockValue(char *parameter, struct manageStruct *manageData, struct multipleBlockAddress *blockAddress){
	int i;
	char blockName[20];
	sscanf(parameter, "%s", blockName);
	for (i = 0; i < manageData->countMemoryBlocks; i++){
		if (strcmp(blockName, blockAddress->blockArray[i].blockName) == 0){
			return blockAddress->blockArray[i].address;
		}
	}
}

void performJump(char *parameter, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int *interLang){
	interLang[1] = 6;
	interLang[2] = getBlockValue(parameter,manageData, blockData);
	interLang[3] = -1;
}

void performIfOp(char *expression, struct multipleSymbolTable *symbolData, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int *interLang,int lineNumber){
	char parameterOne[5], logicalOp[7],parameterTwo[5];
	getParameterInIf(expression, parameterOne, 1);
	getParameterInIf(expression, logicalOp, 2);
	getParameterInIf(expression, parameterTwo, 3);
	interLang[1] = 7;
	interLang[2] = getIdentifierValue(parameterOne, symbolData, manageData);
	if (strcmp(logicalOp, "EQ") == 0)
		interLang[3] = 8;
	else if (strcmp(logicalOp, "LT") == 0)
		interLang[3] = 9;
	else if (strcmp(logicalOp, "GT") == 0)
		interLang[3] = 10;
	else if (strcmp(logicalOp, "LTEQ") == 0)
		interLang[3] = 11;
	else if(strcmp(logicalOp, "GTEQ") == 0)
		interLang[3] = 12;
	performBlock("IF", blockData, manageData, lineNumber);
	interLang[4] = getIdentifierValue(parameterTwo, symbolData, manageData);
	interLang[5] = 0;
	interLang[6] = -1;
}

void performElseOp(char *expression, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int *interLang, int lineNumber){
	char command[10] = "ELSE";
	int duplicate[6];
	interLang[1] = 6;
	interLang[2] = 0;
	performBlock(command, blockData, manageData,lineNumber);
}

void performBlock(char *name, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int dataAddress){
	int index;
	int len = strlen(name);
	while (name[len - 1] == ' ' || name[len - 1] == ':')
		len--;
	if (len != strlen(name))
		name[len] = '\0';
	index = manageData->countMemoryBlocks - 1;
	if (index < 0)
		index = 0;
	strcpy(blockData->blockArray[index].blockName, name);
	blockData->blockArray[index].address = dataAddress;
	manageData->countMemoryBlocks = index + 1;
}

struct stack * pushStack(struct stack *stackHead, int lineNumber){
	struct stack *newStack = (struct stack*)malloc(sizeof(struct stack));
	newStack->data = lineNumber;
	newStack->next = stackHead;
	return newStack;
}

void updateOutputFile(int lineToUpdate,int updateValue, char *outputFileName){
	FILE *outputFile = fopen(outputFileName, "r+");
	char buffer[40];
	char lineInString[8];
	int length;
	int lineNumber = 0;
	while (lineToUpdate != lineNumber){
		fgets(buffer, 40, outputFile);
		lineNumber++;
	}
	length = 0;
	while (buffer[length] != '\n')
		length++;
	buffer[length - 2] = '\0';
	sprintf(lineInString, "%d\n", updateValue);
	strcat(buffer, lineInString);
	fwrite(buffer, 40, 1, outputFile);
	fclose(outputFile);
}

struct stack *performEndIfOp(struct stack *stackHead, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int lineNumber, char *outputFileName){
	performBlock("ENDIF", blockData, manageData, lineNumber);
	//not considered nested if loop
	for (int i = 0; i < 1 && stackHead!=NULL;i++){
		updateOutputFile(stackHead->data, lineNumber, outputFileName);
		stackHead = stackHead->next;
	}
	return stackHead;
}