#include <stdio.h>

#define MAX_WORD_LENGTH 30

int main(int argc, char *argv[]){
	
	//declare a file pointer
	FILE *fp;

	//initialize the word count to zero and define the max word length
	int count = 0;
	char word[MAX_WORD_LENGTH];	

	//open the file (using read mode) and assign it to the file pointer
	fp = fopen(argv[1],"r");

	//iterate through strings in the file using fscanf...increment the count for each string until EOF
	while(fscanf(fp, "%s", word) != EOF){
			count++;
	}

	//close the file
	fclose(fp);

	//print the word count to the terminal
	printf("%d total words\n", count);

	return 0;

}
