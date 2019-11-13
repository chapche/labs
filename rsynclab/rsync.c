#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define BlockSize 512<<3
//local rsync only 
// work pattern: "rsync srcDir dstDir"
int main(int argc, char *argv[]){
	char *srcDir=argv[1];
	char *dstDir=argv[2];
	char *dstCheckSumSet=getCheckSumSet(dstDir);
	char *transferPiece=getAndCheckSumOfSrc(dstCheckSumSet,srcDir);
	int result = transferAndCombine(transferPiece);

	free; 
	return 0;
}
