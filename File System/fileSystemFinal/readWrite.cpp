#include "fileSystemHeader.h"

void readBlock(int blockNumber,char *buffer){
	FILE *f = fopen("hardDisk.hdd", "rb");
	fseek(f, blockNumber*BLOCK_SIZE, SEEK_SET);
	fread(buffer, 1, BLOCK_SIZE, f);
	fclose(f);
}

void writeBlock(int blockNumber, char *buffer){
	FILE *f = fopen("hardDisk.hdd", "rb+");
	fseek(f, blockNumber*BLOCK_SIZE, SEEK_SET);
	fwrite(buffer, 1, BLOCK_SIZE, f);
	fclose(f);
}

void stripSpaces(char *commandLine){
	int currentPosition = 0;
	int modifyPosition = 0;
	bool currentIsSpace = true;
	while (commandLine[currentPosition] != '\0'){
		if (commandLine[currentPosition] == '\t')
			commandLine[currentPosition] = ' ';
		if (commandLine[currentPosition] != ' '){
			commandLine[modifyPosition] = commandLine[currentPosition];
			modifyPosition++;
			currentIsSpace = false;
		}
		else{
			if (!currentIsSpace){
				commandLine[modifyPosition] = commandLine[currentPosition];
				modifyPosition++;
				currentIsSpace = true;
			}
		}
		currentPosition++;
	}
}

void moveLastFile(struct metaData *tempMeta,int index){
	int lastIndex = (*tempMeta).fileCount - 1;
	if (lastIndex == 0)
		return;
	(*tempMeta).fileCount = lastIndex;
	(*tempMeta).files[index].length = (*tempMeta).files[lastIndex].length;
	(*tempMeta).files[index].numberOfBlocks = (*tempMeta).files[lastIndex].numberOfBlocks;
	(*tempMeta).files[index].startBlockNumber = (*tempMeta).files[lastIndex].startBlockNumber;
	strcpy((*tempMeta).files[index].fileName, (*tempMeta).files[lastIndex].fileName);
	return;
}

void formatDatabase(){
	char buffer[BLOCK_SIZE];
	char ch = false;
	struct metaData  tempMeta;
	tempMeta.fileCount = 0;
	tempMeta.magicNum = 0x444E524D;
	tempMeta.NumberOfFreeBlocks = 1000000*1024/BLOCK_SIZE - 4;
	for (int i = 0; i < FILE_COUNT; i++){
		tempMeta.files[i].length = 0;
		tempMeta.files[i].numberOfBlocks = 0;
		tempMeta.files[i].startBlockNumber = 0;
		strcpy(tempMeta.files[i].fileName, " ");
	}
	for (int i = 0; i < META_DATA_LAST_BLOCK; i++){
		tempMeta.isAvailable[i] = false;
	}
	for (int i = META_DATA_LAST_BLOCK; i < NUMBER_OF_BLOCKS; i++){
		tempMeta.isAvailable[i] = true;
	}
	memcpy(buffer, &tempMeta, sizeof(struct metaData));
	writeBlock(0, buffer);
}

void displayFiles(){
	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	struct metaData tempMeta;
	memcpy(&tempMeta, buffer,sizeof(struct metaData));
	if (tempMeta.fileCount == 0){
		printf("                no files found\n");
		return;
	}
	printf("            file names        \n");
	printf("------------------------------\n");
	for (int i = 0; i < tempMeta.fileCount; i++)
		printf("%25s \n", tempMeta.files[i].fileName);
}

int getLengthOfFile(char *fileName){
	FILE *f = fopen(fileName, "rb");
	if (f == NULL){
		printf("	file not found\n");
		return -1;
	}
	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	fclose(f);
	return len;
}

void updateAvailability(int head, int countBlocks,bool  isAvailable){
	struct metaData tempMeta;
	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	memcpy(&tempMeta, buffer, sizeof(struct metaData));
	for (int i = head; i < countBlocks+head; i++){
		tempMeta.isAvailable[i] = isAvailable;
	}
	memcpy(buffer, &tempMeta, sizeof(struct metaData));
	writeBlock(0, buffer);
}

int getAvailableBlock(int requiredBlocks){
	struct metaData tempMeta;
	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	memcpy(&tempMeta, buffer, sizeof(struct metaData));
	int availableBlockHead = -1;
	int countBlocks = 0;
	unsigned int i;
	for (i = 0; i < NUMBER_OF_BLOCKS; i++){
		if (tempMeta.isAvailable[i] == false){
			countBlocks = 0;
			availableBlockHead = -1;
		}
		else{
			if (availableBlockHead == -1)
				availableBlockHead = i;
			countBlocks++;
			if (countBlocks == requiredBlocks)
				break;
		}
	}
	if (i != NUMBER_OF_BLOCKS){
		updateAvailability( availableBlockHead, countBlocks, false);
		return i;
	}
	else
		return 0;
}

void addFile(int fileLength,int numberOfBlocks,int startBlock, char *srcFileName){
	struct metaData tempMeta;
	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	memcpy(&tempMeta, buffer, sizeof(struct metaData));
	tempMeta.fileCount = tempMeta.fileCount + 1;
	int index = tempMeta.fileCount - 1;
	tempMeta.files[index].length = fileLength;
	tempMeta.files[index].numberOfBlocks = numberOfBlocks;
	tempMeta.files[index].startBlockNumber = startBlock;
	strcpy(tempMeta.files[index].fileName, srcFileName);
	memcpy(buffer, &tempMeta, sizeof(struct metaData));
	writeBlock(0, buffer);
}

void copyToDisk(char *commandLine){
	char srcFileName[30];
	char destFileName[30];
	char command[25];
	sscanf(commandLine, "%s %s %s", command, srcFileName, destFileName);
	int fileLength = getLengthOfFile(srcFileName);
	if (fileLength == -1){
		return;
	}
	FILE *srcFile = fopen(srcFileName, "rb");
	int numberOfBlocks = fileLength / BLOCK_SIZE + 1;
	if (fileLength%BLOCK_SIZE == 0)
		numberOfBlocks--;
	int availableBlock = getAvailableBlock(numberOfBlocks);
	if (availableBlock == 0){
		printf("	no available space\n");
		return;
	}	
	int i;
	char buffer[BLOCK_SIZE];
	for (i = 0; i < numberOfBlocks-1; i++){
		fread(buffer, 1, BLOCK_SIZE, srcFile);
		writeBlock(availableBlock + i, buffer);
	}
	fread(buffer, 1, fileLength%BLOCK_SIZE, srcFile);
	writeBlock(i + availableBlock, buffer);
	addFile(fileLength, numberOfBlocks,availableBlock , destFileName);
	fclose(srcFile);
}

int getFileIndex(struct fileTable *tempFileTable, int fileCount, char* srcFileName){
	int i;
	for (i = 0; i < fileCount; i++){
		if (strcmp(tempFileTable[i].fileName, srcFileName) == 0)
			return i;
	}
	return -1;
}

void copyFromDisk(char *commandLine){
	char srcFileName[30], destFileName[30],samp[25];
	sscanf(commandLine,"%s %s %s", samp, srcFileName, destFileName);
	FILE *destFile = fopen(destFileName,"wb"); 
	if (destFile == NULL){
		printf("file not found\n");
		return;
	}
	struct metaData tempMeta;
	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	memcpy(&tempMeta, buffer, sizeof(struct metaData));
	int index = getFileIndex(tempMeta.files, tempMeta.fileCount, srcFileName);
	if (index == -1){
		printf("	file not found\n");
		fclose(destFile);
		return;
	}
	int countBlock = tempMeta.files[index].numberOfBlocks;
	int blockNumber = tempMeta.files[index].startBlockNumber;
	int i;
	for (i = blockNumber; i < blockNumber + countBlock - 1;i++){
		readBlock(i, buffer);
		fseek(destFile, (i - blockNumber)*BLOCK_SIZE, SEEK_SET);
		fwrite(buffer, 1, BLOCK_SIZE, destFile);
	}
	readBlock(i, buffer);
	fwrite(buffer, 1, tempMeta.files[index].length%BLOCK_SIZE, destFile);
	fclose(destFile);
}

void deleleFile(char *commandLine){
	char srcFileName[30],samp[25];
	sscanf(commandLine, "%s %s", samp, srcFileName);
	struct metaData tempMeta;
 	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	memcpy(&tempMeta, buffer, sizeof(struct metaData));
	int index = getFileIndex(tempMeta.files, tempMeta.fileCount, srcFileName);
	if (index == -1){
		printf("	file not found\n");
		return;
	}
	tempMeta.NumberOfFreeBlocks = tempMeta.NumberOfFreeBlocks - tempMeta.files[index].numberOfBlocks;
	moveLastFile(&tempMeta, index);
	updateAvailability(index, tempMeta.files[index].numberOfBlocks, true);
	memcpy(buffer, &tempMeta, sizeof(struct metaData));
	writeBlock(0, buffer);
}

void checkUniqueString(){
	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	int expectedCode = 0x444E524D;
	int actualCode;
	memcpy(&actualCode, buffer, sizeof(int));
	if (actualCode == expectedCode)
		return;
	printf("	Looks like your drive is courrupted.\n Would you like to format? yes / no\n");
	char str[10];
	scanf("%s", &str);
	if (_stricmp(str, "yes") == 0)
		formatDatabase;
}

void debugFileStorage(){
	char buffer[BLOCK_SIZE];
	readBlock(0, buffer);
	struct metaData tempMeta;
	memcpy(&tempMeta, buffer, sizeof(struct metaData));
	printf("          file names    file length     number of blocks  start block number\n");
	printf("----------------------------------------------------------------------------\n");
	for (int i = 0; i < tempMeta.fileCount; i++)
		printf("%20s%15d%20d%10d\n", tempMeta.files[i].fileName, tempMeta.files[i].length, tempMeta.files[i].numberOfBlocks, tempMeta.files[i].startBlockNumber);
	int availableSpace = tempMeta.NumberOfFreeBlocks*BLOCK_SIZE;
	printf("\n---------------------------------------------------------------------------\n");
	printf("		AVAILABLE SPACE : %15d\n", availableSpace);
	printf("		OCCUPIED SPACE  : %15d\n", 1000000*1024 - availableSpace);
	printf("		TOTAL SPACE     : %15d\n", 1000000 * 1024);
}