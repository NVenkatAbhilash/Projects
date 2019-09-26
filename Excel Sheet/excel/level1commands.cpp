#include "Header.h"

void runPrint(struct cell *arr[][DEFAULT_DIMENSTION]){
	for (int i = 0; i < DEFAULT_DIMENSTION; i++){
		for (int j = 0; j < DEFAULT_DIMENSTION; j++){
			printValue(arr, i, j);
		}
		printf("\n");
	}
}

void runExportSave(char *command, char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION], FILE* f){
	char fileName[20];
	if (strcmp(command, "SAVE") == 0 && f == NULL){
		printf("file cannot be saved\n");
		return;
	}
	handleFileName(commandLine, fileName);
	bool newFile = isNewFile(fileName);
	if (newFile){
		f = fopen(fileName, "w+");
		exportNewFile(arr, f);
		return;
	}
	FILE *sampleFile = fopen("sample.csv", "w+");
	if (strcmp(command, "EXPORT") == 0)
		f = fopen(fileName, "r");
	fseek(f, 0, SEEK_SET);
	int row = 0, column = 0;
	char buffer[10];
	while (true){
		nextCell(f, buffer);
		if (arr[row][column]->type == -1){
			fprintf(sampleFile, "%s", buffer);
		}
		else if (arr[row][column]->type == _INT){
			fprintf(sampleFile, "%d", arr[row][column]->data);
		}
		else if (arr[row][column]->type == _STRING){
			//fprintf(sampleFile, "%d", evaluateExpression(arr, arr[row][column]->str));
			fprintf(sampleFile, "%d",arr[row][column]->str);
		}
		column++;
		if (column == DEFAULT_DIMENSTION){
			row++;
			column = 0;
			if (row == DEFAULT_DIMENSTION){
				fputc('\n', sampleFile);
				break;
			}
			fputc('\n', sampleFile);
		}
		else
			fputc(',', sampleFile);
	}
	fclose(sampleFile);
	fclose(f);
	copyFile(fileName, "sample.csv");
	initialiseArray(arr);
}


bool isDigit(char *str){
	int position = 0;
	while (str[position] != '\0'){
		if (str[position] > '9' ||str[position] < '0')
			return false;
		position++;
	}
	return true;
}

void nextCell2(FILE *f, char *str, int *row,int *column){
	char ch = fgetc(f);
	int position = 0;
	if(ch == ',' || ch == '\n')
		ch = fgetc(f);
	while (ch != ',' && ch != '\n'){
		str[position] = ch;
		ch = fgetc(f);
		position++;
	}
	if (ch == ',')
		*column = *column + 1;
	if (ch == '\n')
		*row = *row + 1;
	str[position] = '\0';
}


void runImport(char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION]){
	char fileName[20];
	handleFileName(commandLine, fileName);
	int file_row = 0, file_column = 0;
	if (isNewFile(fileName))
	{
		printf("file cannot import\n");
		return;
	}
	int row = 0, column = 0;
	FILE *f = fopen(fileName, "r");
	getCommandLine(arr, true, f);
	fseek(f, 0, SEEK_SET);
	initialiseArray(arr);
	char buffer[20];
	while (row!=DEFAULT_DIMENSTION){
		nextCell(f, buffer);
		/*{
			if (file_row==row && file_column==column)
				nextCell2(f, buffer, &file_row, &file_column); {
				while (file_row < row)
					nextCell2(f, buffer, &file_row, &file_column);
			}
		}*/
		if (isString(buffer)){
			sscanf(buffer, "%s");
			arr[row][column]->type = 1;
			strcpy(arr[row][column]->str, buffer);
		}
		else{
			int integer = -1;
			integer = stringToInteger(buffer);
			arr[row][column]->type = 0;
			arr[row][column]->data = integer;
		}
		column++;
		if (column == DEFAULT_DIMENSTION){
			row++;
			column = 0;
		}
	}
	fclose(f);
	return;
}

void runImport2(char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION]){
	char fileName[20];
	handleFileName(commandLine, fileName);
	int file_row = 0, file_column = 0;
	if (isNewFile(fileName))
	{
		printf("file cannot import\n");
		return;
	}
	int row = 0, column = 0;
	FILE *f = fopen(fileName, "r");
	getCommandLine(arr, true, f);
	fseek(f, 0, SEEK_SET);
	initialiseArray(arr);
	char buffer[20];
	while (row != DEFAULT_DIMENSTION){
		nextCell(f, buffer);
		if (isString(buffer)){
			sscanf(buffer, "%s");
			arr[row][column]->type = 1;
			strcpy(arr[row][column]->str, buffer);
		}
		else{
			int integer = -1;
			integer = stringToInteger(buffer);
			arr[row][column]->type = 0;
			arr[row][column]->data = integer;
		}
		column++;
		if (column == DEFAULT_DIMENSTION){
			row++;
			column = 0;
		}
	}
	fclose(f);
	return;
}


void handleFileName(char *commandLine, char *fileName){
	sscanf(commandLine, "%s", fileName);
	int fileNameLength = strlen(commandLine);
	if (fileNameLength < 5)
		strcat(fileName, ".csv");
	else if (strcmp(commandLine + fileNameLength - 4, ".csv") != 0)
		strcat(fileName, ".csv");
}


bool isNewFile(char* fileName){
	FILE *file;
	if (file = fopen(fileName, "r")){
		fclose(file);
		return false;
	}
	return true;
}

void nextCell(FILE *f, char *str){
	char ch = fgetc(f);
	int position = 0;
	if (ch == -1)
	{
		str[0] = '\0';
		return;
	}
	while (ch == ',' || ch == '\n')
		ch = fgetc(f);
	while (ch != ',' && ch != '\n'){
		str[position] = ch;
		ch = fgetc(f);
		position++;
	}
	str[position] = '\0';
}


//void runGet(char *commandLine, int parameterPosition, struct cell *arr[][DEFAULT_DIMENSTION]){
//	int column = commandLine[parameterPosition] - 'A';
//	int row = commandLine[parameterPosition + 1] - '1';
//	if (commandLine[parameterPosition + 2] <= '9' && commandLine[parameterPosition + 2] >= '0'){
//		row++;
//		row = row * 10 + commandLine[parameterPosition + 2] - '0';
//		row--;
//	}
//	printValue(arr, row, column);
//}

void runGet(char *commandLine, int parameterPosition, struct cell *arr[][DEFAULT_DIMENSTION]){
	int column = commandLine[parameterPosition] - 'A';
	int row = commandLine[parameterPosition + 1] - '1';
	if (commandLine[parameterPosition + 2] <= '9' && commandLine[parameterPosition + 2] >= '0'){
		row++;
		row = row * 10 + commandLine[parameterPosition + 2] - '0';
		row--;
	}
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

void runSet(char *commandLine, int parameterPosition, struct cell *arr[][DEFAULT_DIMENSTION]){
	int column = commandLine[parameterPosition] - 'A';
	int row = commandLine[parameterPosition + 1] - '1';
	if (commandLine[parameterPosition + 2] <= '9' && commandLine[parameterPosition + 2] >= '0'){
		row++;
		row = row * 10 + commandLine[parameterPosition+2] - '0';
		row--;
	}
	while (commandLine[parameterPosition] != '=')
		parameterPosition++;
	commandLine = commandLine + parameterPosition+1;
	if (commandLine[0] >= '0' && commandLine[0] <= '9'){
		int value;
		sscanf(commandLine, "%d", &value);
		arr[row][column]->data = value;
		arr[row][column]->type = _INT;
	}
	else{
		if (isLoop(commandLine, arr, row, column)){
			printf("loop detected\n");
			return;
		}
		sscanf(commandLine, "%s", arr[row][column]->str);
		arr[row][column]->type = _STRING;
	}
}


void exportNewFile(struct cell *arr[][DEFAULT_DIMENSTION], FILE *f){
	for (int i = 0; i < DEFAULT_DIMENSTION; i++){
		for (int j = 0; j < DEFAULT_DIMENSTION; j++){
			if (arr[i][j]->type != _STRING)
				fprintf(f, "%d,", arr[i][j]->data);
			else
				//fprintf(f, "%s,", evaluateExpression(arr,arr[i][j]->str));
				fprintf(f, "%s,", arr[i][j]->str);
		}
		fputc('\n', f);
	}
	fclose(f);
	initialiseArray(arr);
}