#include "fileSystemHeader.h"

int main(){
	while (true)
	{
		printf("\n>");
		char commandLine[1000];
		char command[30];
		checkUniqueString();
		gets(commandLine);
		stripSpaces(commandLine);
		sscanf(commandLine,"%s", command);
		if (_stricmp(commandLine, "FORMAT") == 0){
			formatDatabase();
		}
		else if (_stricmp(commandLine, "LS") == 0){
			displayFiles();
		}
		else if (_stricmp(commandLine, "EXIT") == 0){
			break;
		}
		else if (_stricmp(command, "COPYTOFS") == 0){
			copyToDisk(commandLine);
			
		}
		else if (_stricmp(command, "COPYFROMFS") == 0){
			copyFromDisk(commandLine);
		}
		else if (_stricmp(command, "DELETE") == 0){
			deleleFile(commandLine );
		}
		else if (_stricmp(command, "DEBUG") == 0){
			debugFileStorage();
		}
		else
			printf("invalid command\n");
	}
	return 0;
}

int samp_main(){
	char buffer[BLOCK_SIZE];
	int fileLength = 10;
	FILE *f = fopen("sample.txt", "rb");
	fread(buffer, 1, fileLength%BLOCK_SIZE, f);
	buffer[fileLength%BLOCK_SIZE] = '\0';
	//fwrite(buffer, 1, fileLength% BLOCK_SIZE, database);
	writeBlock(5, buffer);
	return 0;
}