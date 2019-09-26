#define _CRT_SECURE_NO_WARNINGS

#ifndef FILE_SYSTEM_HEADER
#define FILE_SYSTEM_HEADER

#define BLOCK_SIZE 16384
#define NUMBER_OF_BLOCKS 6000
#define FILE_COUNT 32
#define META_DATA_LAST_BLOCK 3

#define NUMBER_OF_BLOCKS_TO_STORE_FILES 6400

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct fileTable{
	// size 28
	char fileName[20];
	unsigned int startBlockNumber;
	unsigned int numberOfBlocks;
	unsigned int length;
};

struct metaData{
	int magicNum = 0x444E524E44;
	int fileCount;
	int NumberOfFreeBlocks;
	bool isAvailable[NUMBER_OF_BLOCKS];
	struct fileTable files[FILE_COUNT];
};

void readBlock(int blockNumber, char *buffer);

void writeBlock(int blockNumber, char *buffer);

void formatDatabase();

void displayFiles();

int getLengthOfFile(char *fileName);

int getAvailableOffset(int requiredBlocks);

void updateAvailability(int head, int countBlocks, bool  isAvailable);

void copyToDisk(char *parameter);

void copyFromDisk(char *commandLine);

void deleleFile(char *commandLine);

int getCommand(char *commandLine, char *command);

void moveLastFile(struct metaData *tempMeta, int index);

void addFile(int fileLength, int numberOfBlocks, int startBlock, char *srcFileName);

void checkUcheckUniqueString();

void stripSpaces(char *commandLine);

void checkUniqueString();

void debugFileStorage();

#endif