#include "Header.h"

bool getCommandLine(struct cell *arr[][DEFAULT_DIMENSTION], bool fileChange, FILE *newFile){
	FILE *f = NULL;
	if (fileChange){
		f = newFile;
		return true;
	}
	printf("\n>");
	char commandLine[100];
	gets(commandLine);
	if (isExitCommand(commandLine))
		return false;
	beautifyCommandLine(commandLine);
	runCommandLine(commandLine, arr, f);
	return true;
}



int getCommand(char *commandLine, char *command){
	int position = 0;
	while (commandLine[position] != ' ' && commandLine[position] != '\0')
	{
		command[position] = commandLine[position];
		position++;
	}
	command[position] = '\0';
	return position + 1;
}


void runCommandLine(char *commandLine, struct cell *arr[][DEFAULT_DIMENSTION], FILE *f){
	char command[20];
	int parameterPosition = getCommand(commandLine, command);
	runCommand(commandLine, command, parameterPosition, arr, f);
}


void runCommand(char *commandLine, char* command, int parameterPosition, struct cell *arr[][DEFAULT_DIMENSTION], FILE *f){
	if (command[0] == '\0')
		return;
	else if (strcmp(command, "GET") == 0){
		runGet(commandLine, parameterPosition, arr);
	}
	else if (strcmp(command, "SET") == 0)
		runSet(commandLine, parameterPosition, arr);
	else if (strcmp(commandLine, "PRINT HEADER") == 0)
		runPrintHeader(arr);
	else if (strcmp(command, "PRINT") == 0){
		runPrint(arr);
	}
	else if (strcmp(command, "IMPORT") == 0){
		runImport2(commandLine + parameterPosition, arr);
	}
	else if (strcmp(command, "EXPORT") == 0 || strcmp(command, "SAVE") == 0)
		runExportSave(command, commandLine + parameterPosition, arr, f);
	else if (strcmp(command, "SORT") == 0)
		runSort(commandLine + parameterPosition, arr);
	else if (strcmp(command, "EVAL") == 0)
		runEval(commandLine + parameterPosition, arr);
	else
		printf("Invalid command\n");

}

