#include "Header.h"

int main(){
	if (!initialCommand())
		return 0;
	struct cell *arr[DEFAULT_DIMENSTION][DEFAULT_DIMENSTION];
	createArray(arr);
	initialiseArray(arr);
	while (getCommandLine(arr,false,NULL));
	return 0;
}
