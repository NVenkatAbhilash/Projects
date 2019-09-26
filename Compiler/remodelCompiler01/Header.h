#define _CRT_SECURE_NO_WARNINGS

#ifndef COMPILER_HEADER01
#define COMPILER_HEADER01

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct blockAddress{
	char blockName[10];
	int address;
};

struct multipleBlockAddress{
	struct blockAddress blockArray[100];
	struct multipleBlockAddress *nextBlock;
};

struct memory{
	char memoryArray[100];
	struct memory *nextMemory;
};

struct symbolTable{
	char symbolName[5];
	int address;
	int size;
};

struct multipleSymbolTable{
	struct symbolTable symbolArray[100];
	struct multipleSymbolTable *nextTable;
};

struct manageStruct{
	int countSymbolRows;
	int countMemoryBlocks;
	int countRowAddress;
	int registerCodes[7];
	char OPcodes[14][5];
};

struct stack {
	int data;
	struct stack *next;
};

void beutifyBuffer(char *buffer);

int isStringInBuffer(char *buffer, char *str);

void updateSymbolTable(char *buffer, struct multipleSymbolTable *symbolData,struct memory *memoryData ,struct manageStruct *manageData,int dataAddress);

void performReadOp(char *parameter, int *interLang);

bool writeToFile(int *interLang,char *outputFileName);

void performMovOp(char *parameters, struct multipleSymbolTable *symbolData, struct memory *memoryData, struct manageStruct *manageData,int *interLang);

void performArthiOp(char *buffer, struct multipleSymbolTable *symbolData, struct memory *memoryData, struct manageStruct *manageData, int *interLang);

void performPrintOp(char *parameter, struct multipleSymbolTable *symbolData, struct manageStruct *manageData, int *interLang);

void performJump(char *parameter, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int *interLang);

int getValueInBraces(char *buffer);

void getParameterInIf(char *buffer, char *parameter, int number);

void initialiseRegister(struct manageStruct *manageData);

void performIfOp(char *expression, struct multipleSymbolTable *symbolData, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int *interLang,int lineNumber);

void performElseOp(char *expression, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int *interLang, int lineLumber);

struct stack* performEndIfOp(struct stack *stackHead,struct multipleBlockAddress *blockData, struct manageStruct *manageData, int lineNumber, char *outputFileName);

void performBlock(char *buffer, struct multipleBlockAddress *blockData, struct manageStruct *manageData, int dataAddress);

struct stack *pushStack(struct stack *stackHead, int lineNumber);

void updateOutputFile(struct stack *stackHead, char *outputFileName);

int getBlockValue(char *parameter, struct manageStruct *manageData, struct multipleBlockAddress *blockAddress);

#endif