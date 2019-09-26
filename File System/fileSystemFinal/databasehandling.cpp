#include "fileSystemHeader.h"
/*


struct fileTable* createDefaultNode(){
	struct fileTable *newNode = (struct fileTable*)malloc(sizeof(struct fileTable));
	strcpy(newNode->fileName, "");
	newNode->length = 0;
	newNode->numberOfBlocks = 0;
	newNode->offsetValue = 0;
	return newNode;
}

//modified2.0
void copyData(struct fileTable *dest, struct fileTable *src){
	strcpy(dest->fileName, src->fileName);
	dest->length = src->length;
	dest->numberOfBlocks = src->numberOfBlocks;
	dest->offsetValue = src->offsetValue;
}

//modified2.0
void getFileTableArray(struct fileTable *fileTableArray[]){
	FILE *database = fopen("harddisk.hdd", "rb");
	fseek(database, 4, SEEK_SET);
	struct fileTable temp;
	int possibleFileTableRows = (META_DATA_LAST_BLOCK*BLOCK_SIZE - 4) / sizeof(struct fileTable);
	if (database == NULL)
		return;
	int index = 0;
	for (int i = 0; i < possibleFileTableRows; i++){
		fread(&temp, sizeof(struct fileTable), 1, database);
		if (temp.offsetValue == 0)
			break;
		struct fileTable *fileTableRow = (struct fileTable*)malloc(sizeof(struct fileTable));
		copyData(fileTableRow, &temp);
		fileTableArray[index] = fileTableRow;
		index++;
	}
	struct fileTable *fileTableRow = (struct fileTable*)malloc(sizeof(struct fileTable));
	fileTableRow->offsetValue = 0;
	fileTableArray[index] = fileTableRow;
	fclose(database);
}

//do modify late
//modified2.0
void displayFileTable(struct fileTable *fileTableArray[]){
	if (fileTableArray[0]->offsetValue == 0){
		printf("no files found\n");
		return;
	}
	printf("           File Name    length    blocks    offset\n");
	int index = 0;
	while (fileTableArray[index]->offsetValue!=0){
		printf("%20s %10d %10d %d10\n", fileTableArray[index]->fileName, fileTableArray[index]->length, fileTableArray[index]->numberOfBlocks, fileTableArray[index]->offsetValue);
		index++;
	}
}

//not used
void copyToDisk(char *srcFileName, int offsetValue){
	FILE *database = fopen("harddisk.hdd", "rb+");
	if (database == NULL)
		return;
	fseek(database, offsetValue, SEEK_SET);
	FILE *srcFile = fopen(srcFileName, "rb");
	if (srcFile == NULL)
		return;
	int fileLength = getLengthOfFile(srcFileName);
	char buffer[1000000];
	fread(buffer, 1, fileLength, srcFile);
	buffer[fileLength] = '\0';
	fwrite(buffer, 1, fileLength, database);
	fclose(srcFile);
	fclose(database);
}

//modified
int getFreeIndex(struct fileTable *fileTableArray[]){
	int possibleFileTableRows = (META_DATA_LAST_BLOCK*BLOCK_SIZE - 4) / sizeof(struct fileTable);
	int i;
	for (i = 0; i < possibleFileTableRows; i++){
		if (fileTableArray[i]->offsetValue == 0){
			break;
		}
	}
	struct fileTable *temp = createDefaultNode();
	fileTableArray[i + 1] = temp;
	return i;
}

//modified addasdfghjk
 int getFreeOffsetValue(int blocksRequired){
	 FILE *database = fopen("hardDisk.hdd", "rb+");
	 unsigned int availabiltyOffset = BLOCK_SIZE*META_DATA_LAST_BLOCK;
	 fseek(database, availabiltyOffset,SEEK_SET);
	 int availableBlockHead=-1;
	 int countBlocks=0;
	 bool isAvailable;
	 for (unsigned int i = 0; i < BLOCK_SIZE; i++){
		 fread(&isAvailable, 1, 1, database);
		 if (isAvailable == false){
			 countBlocks = 0;
			 availableBlockHead = -1;
		 }
		 else{
			 if (availableBlockHead == -1)
				 availableBlockHead = i;
			countBlocks++;
			if (countBlocks == blocksRequired)
				break;
		 }
	 }
	 isAvailable = false;
	 fseek(database, BLOCK_SIZE * 3 + availableBlockHead , SEEK_SET);
	 fwrite(&isAvailable, 1, 1, database);
	 fclose(database);
	 return (availableBlockHead + 1)*BLOCK_SIZE;
 }
 
 //modified
 int getFileIndex(struct fileTable *fileTableArray[], char *fileName){
	 int i;
	 for (i = 0; ; i++){
		 if (fileTableArray[i]->offsetValue == 0){
			 return -1;
		 }
		 else if (strcmp(fileTableArray[i]->fileName, fileName) == 0)
			 return i;
	 }
 }


 //modified
 void displayFile(struct fileTable *fileTableArray[], char *fileName){
	 int fileIndex = getFileIndex(fileTableArray, fileName);
	 if (fileIndex == -1){
		 printf("File not found\n");
		 return;
	 }
	 int offsetValue = fileTableArray[fileIndex]->offsetValue;
	 int fileLength = fileTableArray[fileIndex]->length;
	 FILE *database = fopen("hardDisk.hdd", "rb");
	 if (database == NULL)
		 return;
	 char buffer[10000];
	 fseek(database, offsetValue, SEEK_SET);
	 fread(buffer, fileLength, 1, database);
	 buffer[fileLength] = '\0';
	 printf("%s", buffer);
	 fclose(database);
 }

 //modified
 void placeFileInArray(struct fileTable *fileTableArray[], char*srcFileName,int length,int offset){
	 int index = getFreeIndex(fileTableArray);
	 strcpy(fileTableArray[index]->fileName, srcFileName);
	 fileTableArray[index]->length = length;
	 fileTableArray[index]->numberOfBlocks = length / BLOCK_SIZE + 1;
	 fileTableArray[index]->offsetValue = offset;
 }

 //modified
 int getLengthOfFile(char *fileName){
	 FILE *f = fopen(fileName,"r");
	 fseek(f, 0, SEEK_END);
	 int len = ftell(f);
	 fclose(f);
	 return len;
 }

 //modified
 void runCopyToDisk(struct fileTable *fileTableArray[], char*srcFileName){
	 FILE *srcFile = fopen(srcFileName, "rb");
	 if (srcFile == NULL){
		 printf(" file '%s' not found", srcFileName);
		 return;
	 }
	 int lengthOfFile = getLengthOfFile(srcFileName);
	 int requiredBlockSize = (lengthOfFile / BLOCK_SIZE) +1;
	 int offsetValue = getFreeOffsetValue(requiredBlockSize);
	 FILE *database = fopen("harddisk.hdd", "rb+");
	 fseek(database, offsetValue, SEEK_SET);
	 char buffer[10000];
	 fread(buffer, 1, lengthOfFile, srcFile);
	 buffer[lengthOfFile] = '\0';
	 fwrite(buffer, 1, lengthOfFile, database);
	 placeFileInArray(fileTableArray, srcFileName, lengthOfFile, offsetValue);
	 fclose(srcFile);
	 fclose(database);
 }

 void updateAvailabilityBlock(int offsetValue,int count,bool isAvailable){
	 FILE *database = fopen("hardDisk.hdd", "rb+");
	 fseek(database, offsetValue, SEEK_SET);
	 for (int i = 0; i < count; i++){
		 fwrite(&isAvailable, 1, 1, database);
	 }
	 fclose(database);
 }

 //modified
 void runCopyFromDisk(struct fileTable *fileTableArray[], char* parameters){
	 char srcFileName[30], destFileName[30];
	 sscanf(parameters, "%s %s", srcFileName,destFileName);
	 int fileIndex = getFileIndex(fileTableArray,srcFileName);
	 if (fileIndex == -1){
		 printf("file '%s' not found", &srcFileName);
		 return;
	 }
	 int lengthOfFile = fileTableArray[fileIndex]->length;
	 int offsetValue = fileTableArray[fileIndex]->offsetValue;
	 FILE *database = fopen("harddisk.hdd", "rb+");
	 if (database == NULL)
		 return;
	 fseek(database, offsetValue, SEEK_SET);
	 FILE *destFile = fopen(destFileName, "wb");
	 char buffer[10000];
	 fread(buffer, 1, lengthOfFile, database);
	 buffer[lengthOfFile] = '\0';
	 fwrite(buffer, 1, lengthOfFile, destFile);
	 updateAvailabilityBlock(offsetValue / BLOCK_SIZE - 1 + BLOCK_SIZE * 3, fileTableArray[fileIndex]->numberOfBlocks,true);
	 fclose(destFile);
	 fclose(database);
 }

 //modified
 
 //modified
 void saveDatabase(struct fileTable *fileTableArray[]){
	 FILE *database = fopen("hardDisk.hdd", "rb+");
	 int i = 0;
	 fseek(database, 4, SEEK_SET);
	 while (fileTableArray[i]->offsetValue!=0)
	 {
		 fwrite(fileTableArray[i], sizeof(struct fileTable), 1, database);
		 i++;
	 }
	 struct fileTable temp;
	 strcpy(temp.fileName, "");
	 temp.length = 0;
	 temp.numberOfBlocks = 0;
	 temp.offsetValue = 0;
	 for (int i = 0; i < 20; i++)
		 fwrite(&temp, sizeof(struct fileTable), 1, database);
	 fclose(database);
 }

 int getLastIndex(struct fileTable *fileTableArray[]){
	 int i;
	 for (i = 0;; i++){
		 if (fileTableArray[i]->offsetValue == 0)
			 return i - 1;
	 }
 }

 void deleteRow(struct fileTable *fileTableArray[],int index){
	 int lastIndex = getLastIndex(fileTableArray);
	 copyData(fileTableArray[index], fileTableArray[lastIndex]);
	 fileTableArray[lastIndex]->offsetValue = 0;
 }

 void runDeleteFile(struct fileTable *fileTableArray[], char *parameters){
	 char srcFileName[20];
	 sscanf(parameters, "%s", srcFileName);
	 int index = getFileIndex(fileTableArray, srcFileName);
	 if (index == -1){
		 printf("file %s not found\n",srcFileName);
		 return;
	 }
	 updateAvailabilityBlock(fileTableArray[index]->offsetValue, fileTableArray[index]->numberOfBlocks, true);
	 deleteRow(fileTableArray,index);
 }

 //modified


 void checkDatabase(){
	 FILE *database = fopen("hardDisk.hdd", "rb");
	 int expected = 0x444E524D;
	 int actual;
	 fread(&actual, 4, 1, database);
	 if (actual != expected){
		 printf("lools like database was courrupted. Do you want to format?yes or no\n");
		 char ch[4];
		 scanf("%s", ch);
		 if (_stricmp(ch, "yes") == 0)
			 formatDatabase();
	 }
	 fclose(database);
 }*/