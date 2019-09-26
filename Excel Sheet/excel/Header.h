#ifndef HEADER
#define HEADER

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _INT 0
#define _STRING 1
#define _TITLES 2

#define DEFAULT_DIMENSTION 10

struct cell{
	int type;
	long data;
	char titles[20];
	char str[20];
};

bool isExitCommand(char *command);

struct cell *createCell();

void createArray(struct cell *arr[][DEFAULT_DIMENSTION]);

void initialiseArray(struct cell *arr[][DEFAULT_DIMENSTION]);

void freeArray(struct cell *arr[][DEFAULT_DIMENSTION]);

void nextCell(FILE *f, char* str);

bool initialCommand();

void beautifyCommandLine(char *commandLine);

bool isString(char *str);

bool getCommandLine(struct cell *arr[][DEFAULT_DIMENSTION], bool fileChange, FILE *newFile);

int getCommand(char *commandLine, char *command);

void runCommandLine(char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION], FILE *f);

void runCommand(char *commandLine, char* command, int parameterPosition, struct cell *arr[][DEFAULT_DIMENSTION], FILE *f);

void printValue(struct cell *arr[][DEFAULT_DIMENSTION], int row, int column);

void runPrint(struct cell *arr[][DEFAULT_DIMENSTION]);

void runExportSave(char *command, char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION], FILE* f);

void runImport(char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION]);

void runImport2(char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION]);

void handleFileName(char *commandLine, char *fileName);

bool isNewFile(char* fileName);

void runGet(char *commandLine, int parameterPosition, struct cell *arr[][DEFAULT_DIMENSTION]);

void runSet(char *commandLine, int parameterPosition, struct cell *arr[][DEFAULT_DIMENSTION]);

void exportNewFile(struct cell *arr[][DEFAULT_DIMENSTION], FILE *f);

bool isLoop(char *expression, struct cell* arr[][DEFAULT_DIMENSTION], int baseRow, int baseColumn);

int evaluateExpression(struct cell *arr[][DEFAULT_DIMENSTION], char *expression);

int performOperation(int value1, char *expression, struct cell * arr[][DEFAULT_DIMENSTION]);

void runSort(char *parameters, struct cell *arr[][DEFAULT_DIMENSTION]);

void runEval(char* expression, struct cell *arr[][DEFAULT_DIMENSTION]);

int stringToInteger(char *buffer);

void runPrintHeader(struct cell* arr[][DEFAULT_DIMENSTION]);

void copyFile(char *destFileName, char* srcFileName);

bool isDigit(char *str);

#endif