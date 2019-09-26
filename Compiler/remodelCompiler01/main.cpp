#include "Header.h"

int main(){
	FILE *inputFile = fopen("maxOfThree.txt", "r");
	char outputFileName[] = "outputFile.txt";
	FILE *f = fopen(outputFileName, "w");
	fclose(f);
	char buffer[60];
	char command[15];
	char *ch;
	int lineNumber = 1;
	int dataAddress = 0;
	struct multipleBlockAddress blockData;
	struct multipleSymbolTable symbolData;
	struct memory memoryData;
	struct manageStruct manageData;
	struct stack *stackHead = NULL;
	struct stack *tempStack;
	int interLang[6];
	ch = fgets(buffer, 60, inputFile);
	beutifyBuffer(buffer);
	initialiseRegister(&manageData);
	while (!isStringInBuffer(buffer, "START")){
		updateSymbolTable(buffer,&symbolData,&memoryData,&manageData,dataAddress);
		dataAddress++;
		ch = fgets(buffer, 60, inputFile);
		beutifyBuffer(buffer);
	}
	ch = fgets(buffer, 60, inputFile);
	beutifyBuffer(buffer);
	while (ch != NULL){
		interLang[0] = lineNumber;
		sscanf(buffer, "%s", command);
		if (strcmp(command, "READ")==0){
			performReadOp(buffer + 5, interLang);
		}
		else if (strcmp(command, "MOV")==0){
			performMovOp(buffer + 4, &symbolData, &memoryData, &manageData, interLang);
		}
		else if (strcmp(command, "ADD")==0 || strcmp(command, "SUB")==0 || strcmp(command, "MUL")==0){
			performArthiOp(buffer, &symbolData, &memoryData, &manageData, interLang);
		}
		else if (strcmp(command, "PRINT")==0){
			performPrintOp(buffer + 6, &symbolData, &manageData, interLang);
		}
		else if (strcmp(command, "JUMP")==0){
			performJump(buffer + 5,&blockData,&manageData,interLang);
		}
		else if (strcmp(command, "IF")==0){
			performIfOp(buffer + 3, &symbolData, &blockData, &manageData, interLang,lineNumber);
			stackHead = pushStack(stackHead, lineNumber);
		}
		else if (strcmp(command, "ELSE") == 0){
			performElseOp(buffer + 4, &blockData, &manageData, interLang,lineNumber);
			stackHead = pushStack(stackHead, lineNumber);
		}
		else if (strcmp(command, "ENDIF")==0){
			stackHead = performEndIfOp(stackHead,&blockData,&manageData,lineNumber,outputFileName);
			interLang[0] = -1;
		}
		else if (strcmp(command, "END")==0){
			break;
		}
		else{
			performBlock(buffer, &blockData,&manageData,lineNumber);
			interLang[0] = -1;
		}
		if (writeToFile(interLang, outputFileName)){
			lineNumber++;
		}
		ch = fgets(buffer, 60, inputFile);
		beutifyBuffer(buffer);
	}
}