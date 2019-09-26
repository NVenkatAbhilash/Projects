#include "Header.h"

void beautifyCommandLine(char *commandLine){
	int iterPosition = 0;
	int modifyPosition = 0;
	bool prevIsSpace = false;
	while (commandLine[iterPosition] == ' ' || commandLine[iterPosition] == '\t')
		iterPosition++;
	while (commandLine[iterPosition] != '\0'){
		if (commandLine[iterPosition] == ' ' || commandLine[iterPosition] == '\t'){
			if (prevIsSpace){
				while (commandLine[iterPosition] == ' ' || commandLine[iterPosition] == '\t')
					iterPosition++;
			}
			else{
				prevIsSpace = true;
			}
		}
		if (commandLine[iterPosition] != '\t')
			commandLine[modifyPosition] = commandLine[iterPosition];
		else
			commandLine[modifyPosition] = ' ';
		modifyPosition++;
		iterPosition++;
	}
	commandLine[modifyPosition] = '\0';
	_strupr(commandLine);
}

int performOperation(int value1, char *expression, struct cell * arr[][DEFAULT_DIMENSTION]){
	char oper = expression[0];
	if (oper == '+')
		return value1 + evaluateExpression(arr, expression + 1);
	else if (oper == '*')
		return value1 * evaluateExpression(arr, expression + 1);
	else 
		return value1;
}

int evaluateExpression(struct cell *arr[][DEFAULT_DIMENSTION], char *expression){
	int i = 0;
	if (expression[0] == '\0')
		return 0;
	if (expression[0] >= '0' && expression[0] <= '9'){
		int value = 0;
		while (expression[i] >= '0' && expression[i] <= '9')
		{
			value = value * 10 + expression[0] - '0';
			i++;
		}
		return performOperation(value, expression + i, arr);
	}
	else if (expression[0] >= 'A' && expression[0] <= 'Z'){
		int column = expression[0] - 'A';
		int row = expression[1] - '1';
		char oper = expression[2];
		if (oper == '\0' && arr[row][column]->type == _INT){
			return arr[row][column]->data;
		}
		else if (arr[row][column]->type == _INT)
			return performOperation(arr[row][column]->data, expression + 2, arr);
		else
			return performOperation(evaluateExpression(arr, arr[row][column]->str), expression + 2, arr);
	}
	return 0;
}

//void printValue(struct cell *arr[][DEFAULT_DIMENSTION], int row, int column){
//
//	if (arr[row][column]->type == -1)
//		printf("%6d",0);
//	else if (arr[row][column]->type == _INT)
//		printf("%6d", arr[row][column]->data);
//	else if (arr[row][column]->type == _STRING){
//		if (isString(arr[row][column]->str))
//			printf("%6s", arr[row][column]->str);
//		printf("%6d", evaluateExpression(arr,arr[row][column]->str));
//		//printf("%3d", evalDeep(arr, arr[row][column]->str));
//	}
//}

void printValue(struct cell *arr[][DEFAULT_DIMENSTION], int row, int column){

	if (arr[row][column]->type == -1)
		printf("%6d", 0);
	else if (arr[row][column]->type == _INT)
		printf("%6d", arr[row][column]->data);
	else if (arr[row][column]->type == _STRING){
		if (isString(arr[row][column]->str))
			printf("%6s", arr[row][column]->str);
		else
			printf("%6d", evaluateExpression(arr, arr[row][column]->str));
		//printf("%3d", evalDeep(arr, arr[row][column]->str));
	}
}



bool isExitCommand(char *command){
	if (_stricmp(command, "EXIT") == 0)
		return true;
	return false;
}

bool initialCommand(){
	char command[10];
	printf("$");
	scanf("%s", &command);
	if (strcmp(command, "excel.exe") == 0)
		return true;
	return false;
}

void createArray(struct cell *arr[][DEFAULT_DIMENSTION]){
	for (int i = 0; i < DEFAULT_DIMENSTION; i++){
		for (int j = 0; j < DEFAULT_DIMENSTION; j++){
			arr[i][j] = createCell();
		}
	}
}


void initialiseArray(struct cell *arr[][DEFAULT_DIMENSTION]){
	for (int i = 0; i < DEFAULT_DIMENSTION; i++){
		for (int j = 0; j < DEFAULT_DIMENSTION; j++){
			arr[i][j]->data = 0;
			arr[i][j]->type = -1;
			arr[i][j]->str[0] = '\0';
		}
	}
}

void freeArray(struct cell *arr[][DEFAULT_DIMENSTION]){
	for (int i = 0; i < DEFAULT_DIMENSTION; i++){
		for (int j = 0; j < DEFAULT_DIMENSTION; j++){
			free(arr[i][j]);
		}
	}
}

bool isExpression(char *expression){
	int oper=0;
	int alphabets = 0;
	int numbers = 0;
	int position = 0;
	char ch = expression[position];
	while ( ch!= '\0'){
		ch = expression[position];
		if (ch == '+' || ch == '-' || ch=='*' || ch=='/' || ch=='%'){
			oper++;
		}
		if (ch <= 'Z' && ch >= 'A')
			alphabets++;
		else
			numbers++;
		position++;
	}
	if (oper > 0 || ((numbers - alphabets) < 6 && (numbers-alphabets)>0))
		return true;
}

bool isString(char *str){
	int position = 0;
	while (str[position] != '\0'){
		if (str[position] < 'A' || str[position]>'Z')
			return false;
		position++;
	}
	return true;
}

bool isLoop(char *expression, struct cell* arr[][DEFAULT_DIMENSTION], int baseRow, int baseColumn){
	int i = 0;
	//bool isExpre=isExpression(expression);
	//if (!isExpre)
	//	return false;
	if (isString(expression))
		return false;
	if (expression[0] == '\0')
		return false;
	if (expression[0] >= '0' && expression[0] <= '9'){
		while (expression[i] >= '0' && expression[i] <= '9')
		{
			i++;
		}
		return isLoop(expression + i, arr, baseRow, baseColumn);

	}
	else if (expression[0] >= 'A' && expression[0] <= 'Z'){
		int column = expression[0] - 'A';
		int row = expression[1] - '1';
		if (row == baseRow && column == baseColumn)
			return true;
		return isLoop(arr[row][column]->str, arr, baseRow, baseColumn) || isLoop(expression+2,arr,baseRow,baseColumn);
	}
	else
		return isLoop(expression + 1, arr, baseRow, baseColumn);
}



struct cell *createCell(){
	struct cell *newCell = (struct cell*)malloc(sizeof(struct cell));
	return newCell;
}

void runSave(FILE *f){
	printf("SORRY! SAVE file is not yet constructed\n");
}

void swap(struct cell *arr[][DEFAULT_DIMENSTION], int row1, int column1, int row2, int column2){
	int tempType = arr[row1][column1]->type;
	int tempData = arr[row1][column1]->data;
	char tempStr[20];
	strcpy(tempStr, arr[row2][column2]->str);
	arr[row1][column1]->type = arr[row2][column2]->type;
	arr[row1][column1]->data = arr[row2][column2]->data;
	strcpy(arr[row1][column1]->str, arr[row2][column2]->str);
	arr[row2][column2]->type = tempType;
	arr[row2][column2]->data = tempData;
	strcpy(arr[row2][column2]->str,tempStr);
}

void sort(struct cell* arr[][DEFAULT_DIMENSTION],int row,int colum){
	int changeRow = 0, changeColumn = 0;
	if (row == -1){
		row = 0;
		changeRow = 1;
	}
	else{
		colum = 0;
		changeColumn = 1;
	}for (int i = 0; i < DEFAULT_DIMENSTION; i++){
		for (int j = i + 1; j < DEFAULT_DIMENSTION; j++){
			if (arr[row + changeRow*i][colum + changeColumn*i]->data>arr[row + changeRow*j][colum + changeColumn*j]->data)
				swap(arr, row + changeRow*i, colum + changeColumn*i, row + changeRow*j, colum + changeColumn*j);
		}
	}

}

void runSort(char *parameters, struct cell *arr[][DEFAULT_DIMENSTION]){
	if (parameters[0] <= 'J' && parameters[0] >= 'A')
		sort(arr, -1, parameters[0] - 'A');
	else if (parameters[0] >= '1' && parameters[0] <= '9')
		sort(arr, parameters[0] - '1', -1);
}




void copyFile(char *destFileName, char* srcFileName){
	FILE *srcFile, *destFile;
	char c;
	srcFile = fopen(srcFileName, "r");
	destFile = fopen(destFileName, "w");
	c = fgetc(srcFile);
	while (c != EOF)
	{
		fputc(c, destFile);
		c = fgetc(srcFile);
	}
	fclose(srcFile);
	fclose(destFile);
}

bool isOperator(char ch){
	if (ch == '(' || ch == ')' || ch == '+' || ch == '-' || ch == '*' || ch == '%' || ch == '/')
		return true;
	return false;
}

int priorValue(char ch){
	if (ch == ')')
		return 4;
	if (ch == '^')
		return 3;
	if (ch == '*' || ch == '/')
		return 2;
	if (ch == '+' || ch == '-')
		return 1;
	return 0;
}

int getData(char *expression, struct cell *arr[][DEFAULT_DIMENSTION]){
	if (expression[0] >= 'A'&&expression[0] <= 'J'){
		int column = expression[0] - 'A';
		int row = expression[1] - '1';
		return arr[row][column]->data;
	}
	int position = 0;
	int integer = 0;
	while (expression[position] <= '9'&& expression[position] >= '0'){
		integer = expression[position] - '0'+integer*10;
		position++;
	}
	return integer;
}

char popChar(char *stack){
	int len = strlen(stack);
	char ch = stack[len-1];
	stack[len - 1] = '\0';
	return ch;
}

void pushChar(char *stack, char ch){
	int len = strlen(stack);
	stack[len] = ch;
	stack[len + 1] = '\0';
}

void pushToStack(char *stack, char* postfix, char oper){
	int lenStack = strlen(stack);
	if (priorValue(stack[lenStack - 1]) < priorValue(oper))
		pushChar(stack, oper);
	else{
		pushChar(postfix,popChar(stack));
		pushToStack(stack, postfix, oper);
	}
}

void dataToStr(char *str, int data){
	int position = 0;
	do
	{
		str[position] = data % 10;
		position++;
		data = data / 10;
	} while (data != 0);
	_strrev(str);
}

float operation(int a, int b, char oper){
	if (oper == '+')
		return a + b;
	if (oper == '-')
		return b - a;
	if (oper == '*')
		return a*b;
	if (oper == '/')
		return a / b;
	if (oper == '%')
		return a%b;
}

int stackToIntStack(char *stack,float* integerStack,int intLen,char oper){
	int stackLen = strlen(stack);
	if (priorValue(stack[stackLen - 1]) >= priorValue(oper)){
		char op = popChar(stack);
		float data = operation(integerStack[stackLen - 2], integerStack[stackLen - 1], op);
		integerStack[intLen - 2] = data;
		intLen = intLen - 1;
		intLen = stackToIntStack(stack, integerStack, intLen, oper);
	}
	else
		pushChar(stack, oper);
	return intLen;
}

int comingOperator(char *expression){
	int position = 0;
	while (true){
		if (expression[position] == '+' || expression[position] == '-' || expression[position] == '*' || expression[position] == '%' || expression[position] == '/')
			break;
		position++;
	}
	return position;
}


void runEval(char* expression, struct cell *arr[][DEFAULT_DIMENSTION] ){
	char stack[20];
	stack[0] = '(';
	stack[1] = '\0';
	float integerStack[20];
	int intLen = 0;
	int position = 0;
	pushChar(expression, ')');
	while (expression[position] != '\0'){
		char ch = expression[position];
		if (!isOperator(ch)){
			float data = getData(expression + position, arr);
			position+=comingOperator(expression+position);
			integerStack[intLen] = data;
			intLen++;
			continue;
		}
		else if (ch == ')'){
			char popedChar = popChar(stack);
			if (popedChar == '('){
				position++;
				continue;
			}
			do{
				float data = operation(integerStack[intLen - 1], integerStack[intLen - 2], popedChar);
				integerStack[intLen - 2] = data;
				intLen--;
				popedChar = popChar(stack);
			} while (popedChar != '(');
		}
		else if (ch == '(')
			pushChar(stack, ch);
		else
			intLen = stackToIntStack(stack, integerStack, intLen, ch);
		position++;
	}
	printf("%d\n", (int)integerStack[0]);
}

int stringToInteger(char *buffer){
	int position = 0;
	int integer = 0;
	while (buffer[position] != '\0'){
		if (buffer[position] <= '9' && buffer[position] >= '0'){
			integer = integer * 10 + buffer[position] - '0';
		}
		else
			return integer;
		position++;
	}
	return integer;
}

void runPrintHeader(struct cell* arr[][DEFAULT_DIMENSTION]){
	for (int i = 0; i < DEFAULT_DIMENSTION; i++){
		if (arr[0][i]->type == 1)
			printf("%s, ", arr[0][i]->str);
		else
			printf("%s, ", "0");
	}
}